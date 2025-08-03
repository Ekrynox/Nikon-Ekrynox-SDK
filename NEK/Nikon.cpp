#include "nikon.hpp"

#include <chrono>
#include <time.h>
#include <stdexcept>



using namespace nek;



std::map<std::wstring, NikonDeviceInfoDS> NikonCamera::listNikonCameras() {
	mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto cameras = deviceManager->listMtpDevices();
	std::map<std::wstring, NikonDeviceInfoDS> nikonCameras;

	for (auto &camera : cameras) {
		//Check if Nikon
		std::wstring id(camera.first);
		std::transform(id.begin(), id.end(), id.begin(), ::towlower);
		if (id.find(L"vid_04b0") != std::wstring::npos) {
			nikonCameras.insert(camera);
		}
	}

	return nikonCameras;
}

size_t NikonCamera::countNikonCameras() {
	return listNikonCameras().size();
}


NikonCamera::NikonCamera(std::wstring devicePath) : nek::mtp::MtpDevice::MtpDevice() {
	devicePath_ = (PWSTR)devicePath.c_str();

	threads_.push_back(std::thread([this] { this->mainThreadTask(); }));
	std::unique_lock lk(mutexTasks_);
	cvTasks_.wait(lk);

	threads_.push_back(std::thread([this] { this->eventThreadTask(); }));
	cvTasks_.wait(lk, [this] { return this->eventCallback_ != nullptr; });
}


void NikonCamera::mainThreadTask() {
	mutexTasks_.lock();
	mutexDevice_.lock();

	//Com context
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (hr == RPC_E_CHANGED_MODE) {
		throw std::runtime_error("Failed to init COM: " + hr);
	}

	//Device Client
	hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceClient_));
	if (FAILED(hr)) throw std::runtime_error("Impossible to create the Portable Device Client: " + hr);
	hr = deviceClient_->SetStringValue(WPD_CLIENT_NAME, CLIENT_NAME);
	if (FAILED(hr)) throw std::runtime_error("Failed to set Client Name" + hr);
	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MAJOR_VERSION, CLIENT_MAJOR_VER);
	if (FAILED(hr)) throw std::runtime_error("Failed to set Client Major Version" + hr);
	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MINOR_VERSION, CLIENT_MINOR_VER);
	if (FAILED(hr)) throw std::runtime_error("Failed to set Client Minor Version" + hr);
	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_REVISION, CLIENT_REVISION);
	if (FAILED(hr)) throw std::runtime_error("Failed to set Client Revision" + hr);

	//Device
	hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device_));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create device instance: " + hr);
	}

	hr = device_->Open(devicePath_, deviceClient_);
	if (FAILED(hr)) {
		device_.Release();
		throw std::runtime_error("Failed to open device: " + hr);
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
	if (std::find(info.OperationsSupported.begin(), info.OperationsSupported.end(), NikonMtpOperationCode::GetEventEx) != info.OperationsSupported.end()) {
		//Device
		CComPtr<IPortableDevice> device;
		hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device));
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create device instance: " + hr);
		}

		hr = device->Open(devicePath_, deviceClient_);
		if (FAILED(hr)) {
			device.Release();
			throw std::runtime_error("Failed to open device: " + hr);
		}

		eventCallback_ = new nek::mtp::MtpEventCallback();
		cvTasks_.notify_all();

		//Event Loop
		nek::mtp::MtpParams params = nek::mtp::MtpParams();
		size_t offset;
		uint32_t count;
		uint16_t count_params;
		uint16_t eventCode;
		std::vector<uint32_t> eventParams;
		while (running_) {
			nek::mtp::MtpResponse result = SendCommandAndRead_(device, NikonMtpOperationCode::GetEventEx, params);
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
					eventCallback_->OnEvent(nek::mtp::MtpEvent(eventCode, eventParams));
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		//Uninit
		eventCallback_.Release();
		device.Release();
	}
	else if (std::find(info.OperationsSupported.begin(), info.OperationsSupported.end(), NikonMtpOperationCode::GetEvent) != info.OperationsSupported.end()) {
		//Device
		CComPtr<IPortableDevice> device;
		hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device));
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to create device instance: " + hr);
		}

		hr = device->Open(devicePath_, deviceClient_);
		if (FAILED(hr)) {
			device.Release();
			throw std::runtime_error("Failed to open device: " + hr);
		}

		eventCallback_ = new nek::mtp::MtpEventCallback();
		cvTasks_.notify_all();

		//Event Loop
		nek::mtp::MtpParams params = nek::mtp::MtpParams();
		uint16_t count;
		uint16_t eventCode;
		uint32_t eventParam;
		while (running_) {
			nek::mtp::MtpResponse result = SendCommandAndRead_(device, NikonMtpOperationCode::GetEvent, params);
			if (result.responseCode == NikonMtpResponseCode::OK) {
				count = *(uint16_t*)(result.data.data());
				for (uint16_t i = 0; i < count; i++) {
					eventCode = *(uint16_t*)(result.data.data() + 2 + i * 6);
					eventParam = *(uint32_t*)(result.data.data() + 4 + i * 6);
					eventCallback_->OnEvent(nek::mtp::MtpEvent(eventCode, eventParam));
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		//Uninit
		eventCallback_.Release();
		device.Release();
	}
	else {
		sendTask([this] {
			this->eventCallback_ = new nek::mtp::MtpEventCallback();
			this->device_->Advise(0, this->eventCallback_, nullptr, &this->eventCookie_);
			});
	}

	CoUninitialize();
}