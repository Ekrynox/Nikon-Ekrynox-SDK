#include "nek_mtp.hpp"

#include <algorithm>
#include <stdexcept>



using namespace nek::mtp;


//MtpManager
MtpManager::MtpManager() {
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to init COM: " + hr);
	}

	//Device Manager
	hr = CoCreateInstance(CLSID_PortableDeviceManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceManager_));
	if (FAILED(hr)) {
		CoUninitialize();
		throw std::runtime_error("Impossible to create the Portable Device Manager: " + hr);
	}

	//Device Client
	hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceClient_));
	if (FAILED(hr)) {
		CoUninitialize();
		throw std::runtime_error("Impossible to create the Portable Device Client: " + hr);
	}

	hr = deviceClient_->SetStringValue(WPD_CLIENT_NAME, CLIENT_NAME);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to set Client Name" + hr);
	}

	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MAJOR_VERSION, CLIENT_MAJOR_VER);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to set Client Major Version" + hr);
	}

	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MINOR_VERSION, CLIENT_MINOR_VER);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to set Client Minor Version" + hr);
	}

	hr = deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_REVISION, CLIENT_REVISION);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to set Client Revision" + hr);
	}

	/*hr = (*clientInformation)->SetUnsignedIntegerValue(WPD_CLIENT_SECURITY_QUALITY_OF_SERVICE, SECURITY_IMPERSONATION);*/
}

MtpManager::~MtpManager() {
	CoUninitialize();
}

MtpManager& MtpManager::Instance() {
	static MtpManager instance;
	return instance;
}


std::vector<std::wstring> MtpManager::listNikonCameras() {
	std::vector<std::wstring> nikonCameras;

	DWORD devicesNb = 0;
	PWSTR* devices = nullptr;

	//Get the number of WPD devices
	HRESULT hr = deviceManager_->GetDevices(NULL, &devicesNb);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to retreive the number of devices");
	}

	//At least one device
	if (devicesNb > 0) {
		devices = new PWSTR[devicesNb] ();
		HRESULT hr = deviceManager_->GetDevices(devices, &devicesNb);
		if (FAILED(hr)) {
			delete [] devices;
			throw std::runtime_error("Failed to retreive the list of devices");
		}

		for (DWORD i = 0; i < devicesNb; i++) {
			if (devices[i] != nullptr) {
				//Check if Nikon
				std::wstring id(devices[i]);
				std::transform(id.begin(), id.end(), id.begin(), ::towlower);
				if (id.find(L"vid_04b0") != std::wstring::npos) {
					nikonCameras.push_back(devices[i]);
				}
						
				CoTaskMemFree(devices[i]);
			}
		}

		delete[] devices;
	}

	return nikonCameras;
}

int MtpManager::countNikonCameras() {
	return listNikonCameras().size();
}

CComPtr<IPortableDevice> MtpManager::openDevice(PWSTR deviceId) {
	CComPtr<IPortableDevice> device;
	HRESULT hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create device instance: " + hr);
	}

	hr = device->Open(deviceId, deviceClient_);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to open device: " + hr);
	}

	return device;
}


//MtpDevice
MtpDevice::MtpDevice(PWSTR deviceId) {
	deviceManager_ = &MtpManager::Instance();
	device_ = deviceManager_->openDevice(deviceId);
}


HRESULT MtpDevice::SendCommand(WORD operationCode, IPortableDevicePropVariantCollection* operationParams, CComPtr<IPortableDeviceValues>& results) {
	CComPtr<IPortableDeviceValues> command;
	HRESULT hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(hr)) return hr;

	// Set command category and ID
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.pid);

	// Set operation code and parameters
	PROPVARIANT opCode;
	PropVariantInit(&opCode);
	opCode.vt = VT_UI2;
	opCode.uiVal = operationCode;
	command->SetValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, &opCode);
	command->SetIUnknownValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, operationParams);

	// Send command
	hr = device_->SendCommand(0, command, &results);
	if (FAILED(hr)) return hr;

	return hr;
}

HRESULT MtpDevice::GetIUnknownValue(IPortableDeviceValues& results, PROPVARIANT& responseCode, CComPtr<IPortableDevicePropVariantCollection>& responseParams) {
    // Extract response code
    HRESULT hr = results.GetValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, &responseCode);
    if (FAILED(hr)) return hr;

    // Extract response parameters
    CComPtr<IUnknown> unk;
    hr = results.GetIUnknownValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &unk);
    if (SUCCEEDED(hr) && unk) {
        hr = unk->QueryInterface(IID_PPV_ARGS(&responseParams));
    }
    return hr;
}