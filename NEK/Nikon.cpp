#include "nikon.hpp"
#include "mtp/nek_mtp_except.hpp"

#include <chrono>
#include <time.h>
#include <stdexcept>



using namespace nek;



std::map<std::wstring, NikonDeviceInfoDS> NikonCamera::listNikonCameras(bool onlyOn) {
	mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto cameras = deviceManager->listMtpDevices();
	std::map<std::wstring, NikonDeviceInfoDS> nikonCameras;

	for (auto &camera : cameras) {
		//Check if Nikon
		std::wstring id(camera.first);
		std::transform(id.begin(), id.end(), id.begin(), ::towlower);
		if (id.find(L"vid_04b0") != std::wstring::npos) {
			if (onlyOn == false) {
				nikonCameras.insert(camera);
			}
			else if (std::find(camera.second.OperationsSupported.begin(), camera.second.OperationsSupported.end(), NikonMtpOperationCode::InitiateCaptureRecInSdram) != camera.second.OperationsSupported.end()) {
				nikonCameras.insert(camera);
			}
		}
	}

	return nikonCameras;
}

size_t NikonCamera::countNikonCameras(bool onlyOn) {
	return listNikonCameras(onlyOn).size();
}


NikonCamera::NikonCamera(std::wstring devicePath, byte additionalThread) : nek::mtp::MtpDevice::MtpDevice() {
	devicePath_ = (PWSTR)devicePath.c_str();

	//Start main thread
	threads_.push_back(std::thread([this] { this->mainThreadTask(); }));
	std::unique_lock lk(mutexTasks_);
	cvTasks_.wait(lk);

	//Event Thread
	threads_.push_back(std::thread([this] { this->eventThreadTask(); }));
	cvTasks_.wait(lk);

	//Additional Threads
	for (size_t i = 0; i < additionalThread; i++) {
		threads_.push_back(std::thread([this] { this->additionalThreadTask(); }));
	}
}


void NikonCamera::mainThreadTask() {
	mutexTasks_.lock();
	mutexDevice_.lock();

	//Com context
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr == RPC_E_CHANGED_MODE) {
		mutexDevice_.unlock();
		mutexTasks_.unlock();
		throw nek::mtp::MtpDeviceException(nek::mtp::MtpExPhase::COM_INIT, hr);
	}

	//Device Client
	hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceClient_));
	if (FAILED(hr)) {
		mutexDevice_.unlock();
		mutexTasks_.unlock();
		throw nek::mtp::MtpDeviceException(nek::mtp::MtpExPhase::DEVICECLIENT_INIT, hr);
	}
	deviceClient_->SetStringValue(WPD_CLIENT_NAME, CLIENT_NAME);
	deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MAJOR_VERSION, CLIENT_MAJOR_VER);
	deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MINOR_VERSION, CLIENT_MINOR_VER);
	deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_REVISION, CLIENT_REVISION);

	//Device
	hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device_));
	if (FAILED(hr)) {
		mutexDevice_.unlock();
		mutexTasks_.unlock();
		throw nek::mtp::MtpDeviceException(nek::mtp::MtpExPhase::DEVICE_INIT, hr);
	}

	hr = device_->Open(devicePath_, deviceClient_);
	if (FAILED(hr)) {
		device_.Release();
		mutexDevice_.unlock();
		mutexTasks_.unlock();
		throw nek::mtp::MtpDeviceException(nek::mtp::MtpExPhase::DEVICE_INIT, hr);
	}

	mutexDevice_.unlock();
	mutexTasks_.unlock();
	cvTasks_.notify_all();

	//Thread Loop
	threadTask();

	//Uninit
	mutexTasks_.lock();
	mutexDevice_.lock();

	if (eventCookie_ != nullptr) {
		device_->Unadvise(eventCookie_);
	}
	device_.Release();
	device_.p = nullptr;
	CoUninitialize();

	mutexDevice_.unlock();
	mutexTasks_.unlock();
	cvTasks_.notify_all();
}

void NikonCamera::eventThreadTask() {
	//Com context
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr == RPC_E_CHANGED_MODE) {
		throw std::runtime_error("Failed to init COM: " + hr);
	}

	//Event Handler Detection
	NikonDeviceInfoDS info = GetDeviceInfo();
	if (std::find(info.OperationsSupported.begin(), info.OperationsSupported.end(), NikonMtpOperationCode::GetEventEx) != info.OperationsSupported.end()) { //GetEventEX
		cvTasks_.notify_all();

		//Event Loop
		nek::mtp::MtpParams params = nek::mtp::MtpParams();
		size_t offset;
		uint32_t count;
		uint16_t count_params;
		uint16_t eventCode;
		std::vector<uint32_t> eventParams;
		while (running_) {
			mutexDevice_.lock();
			nek::mtp::MtpResponse result = SendCommandAndRead_(device_, NikonMtpOperationCode::GetEventEx, params);
			mutexDevice_.unlock();
			if (result.responseCode == NikonMtpResponseCode::OK) {
				count = *(uint32_t*)(result.data.data());
				offset = sizeof(uint32_t);
				for (uint32_t i = 0; i < count; i++) {
					eventCode = *(uint16_t*)(result.data.data() + offset);
					offset += sizeof(uint16_t);
					count_params = *(uint16_t*)(result.data.data() + offset);
					offset += sizeof(uint16_t);
					eventParams.clear();
					for (uint32_t j = 0; j < count_params; j++) {
						eventParams.push_back(*(uint32_t*)(result.data.data() + offset));
						offset += sizeof(uint32_t);
					}
					mutexTasks_.lock();
					tasksEvent_.push([this, eventCode, eventParams] { eventCallback_->OnEvent(nek::mtp::MtpEvent(eventCode, eventParams)); });
					mutexTasks_.unlock();
					cvTasks_.notify_one();
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	else if (std::find(info.OperationsSupported.begin(), info.OperationsSupported.end(), NikonMtpOperationCode::GetEvent) != info.OperationsSupported.end()) { //GetEvent
		cvTasks_.notify_all();

		//Event Loop
		nek::mtp::MtpParams params = nek::mtp::MtpParams();
		uint16_t count;
		uint16_t eventCode;
		uint32_t eventParam;
		while (running_) {
			mutexDevice_.lock();
			nek::mtp::MtpResponse result = SendCommandAndRead_(device_, NikonMtpOperationCode::GetEvent, params);
			mutexDevice_.unlock();
			if (result.responseCode == NikonMtpResponseCode::OK) {
				count = *(uint16_t*)(result.data.data());
				for (uint16_t i = 0; i < count; i++) {
					eventCode = *(uint16_t*)(result.data.data() + 2 + i * 6);
					eventParam = *(uint32_t*)(result.data.data() + 4 + i * 6);
					mutexTasks_.lock();
					tasksEvent_.push([this, eventCode, eventParam] { eventCallback_->OnEvent(nek::mtp::MtpEvent(eventCode, eventParam)); });
					mutexTasks_.unlock();
					cvTasks_.notify_one();
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
	else { //Default Mtp event system (Incomplete: missing event code, ...)
		mutexDevice_.lock();

		device_->Advise(0, this->eventCallback_, nullptr, &this->eventCookie_);

		mutexDevice_.unlock();
		cvTasks_.notify_all();
	}

	CoUninitialize();
}

void NikonCamera::threadTask() {
	while (running_) {
		mutexTasks_.lock();
		if (tasksEvent_.size() > 0) {
			auto task = tasksEvent_.front();
			tasksEvent_.pop();
			mutexTasks_.unlock();

			task();
		}
		else if (tasks_.size() > 0) {
			auto task = tasks_.front();
			tasks_.pop();
			mutexTasks_.unlock();

			task();
		}
		else {
			mutexTasks_.unlock();

			std::unique_lock lk(mutexTasks_);
			cvTasks_.wait(lk, [this] { return !this->running_ || (this->tasks_.size() + tasksEvent_.size() > 0); });
			lk.unlock();
		}
	}
}