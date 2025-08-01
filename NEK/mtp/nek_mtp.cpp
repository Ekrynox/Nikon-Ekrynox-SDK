#include "nek_mtp.hpp"

#include <algorithm>
#include <stdexcept>



using namespace nek::mtp;


//MtpManager
MtpManager::MtpManager() {
	HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
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
	deviceManager_.Release();
	deviceClient_.Release();
	CoUninitialize();
}

MtpManager& MtpManager::Instance() {
	static MtpManager instance;
	return instance;
}


std::map<std::wstring, MtpDeviceInfoDS> MtpManager::listMtpDevices() {
	std::map<std::wstring, MtpDeviceInfoDS> nikonCameras;

	DWORD devicesNb = 0;
	PWSTR* devices = nullptr;
	HRESULT hr;

	//Update WPD devices list
	hr = deviceManager_->RefreshDeviceList();
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to refresh the device list");
	}

	//Get the number of WPD devices
	hr = deviceManager_->GetDevices(NULL, &devicesNb);
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
				nikonCameras.insert(std::pair(std::wstring(devices[i]), MtpDevice(devices[i]).GetDeviceInfo()));
				CoTaskMemFree(devices[i]);
			}
		}

		delete[] devices;
	}

	return nikonCameras;
}

size_t MtpManager::countMtpDevices() {
	return listMtpDevices().size();
}

CComPtr<IPortableDevice> MtpManager::openDevice(const PWSTR devicePath) {
	CComPtr<IPortableDevice> device;
	HRESULT hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create device instance: " + hr);
	}

	hr = device->Open(devicePath, deviceClient_);
	if (FAILED(hr)) {
		device.Release();
		throw std::runtime_error("Failed to open device: " + hr);
	}

	return device;
}


//MtpDevice
MtpDevice::MtpDevice(const PWSTR devicePath) {
	device_ = MtpManager::Instance().openDevice(devicePath);
	eventCallback_ = new MtpEventCallback();
	device_->Advise(0, eventCallback_, nullptr, &eventCookie);
}

MtpDevice::~MtpDevice() {
	device_->Unadvise(eventCookie);
	eventCallback_.Release();
	device_.Release();
}


MtpResponse* MtpDevice::SendNoData(WORD operationCode, MtpParams& params) {
	MtpResponse* result = new MtpResponse();

	CComPtr<IPortableDeviceValues> command;
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		return result;
	}

	// Set command category and ID
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.pid);

	// Set operation code and parameters
	command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
	command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());

	// Send command
	CComPtr<IPortableDeviceValues> commandResult;
	result->hr = device_->SendCommand(0, command, &commandResult);
	if (FAILED(result->hr)) {
		return result;
	}

	// Extract response code
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, reinterpret_cast<ULONG*>(& result->responseCode));
	if (FAILED(result->hr)) {
		return result;
	}

	// Extract response parameters
	CComPtr<IUnknown> unk;
	result->hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &result->GetParams().GetCollection());
	if (FAILED(result->hr)) {
		return result;
	}
	
	return result;
}


MtpResponse* MtpDevice::SendReadData(WORD operationCode, MtpParams& params) {
	MtpResponse *result = new MtpResponse();

	CComPtr<IPortableDeviceValues> command;
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		return result;
	}

	// Set command category and ID
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_READ.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_READ.pid);

	// Set operation code and parameters
	command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
	command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());

	// Send command
	CComPtr<IPortableDeviceValues> commandResult;
	result->hr = device_->SendCommand(0, command, &commandResult);
	if (FAILED(result->hr)) {
		return result;
	}

	LPWSTR context;
	result->hr = commandResult->GetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, &context);
	if (FAILED(result->hr)) {
		return result;
	}

	ULONG totalSize;
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_TOTAL_DATA_SIZE, &totalSize);
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}

	ULONG optimalSize;
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPTIMAL_TRANSFER_BUFFER_SIZE, &optimalSize);
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}

	command.Release();
	commandResult.Release();


	// Start Data Transfert
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_READ_DATA.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_READ_DATA.pid);
	command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_NUM_BYTES_TO_READ, totalSize);
	BYTE* buffer = new BYTE[optimalSize];
	command->SetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, buffer, optimalSize);

	BYTE* b = nullptr;
	DWORD bNb = 0;
	do {
		result->hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(result->hr)) {
			CoTaskMemFree(context);
			return result;
		}
		commandResult->GetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, &b, &bNb);
		result->data.insert(result->data.end(), b, b + bNb);
		commandResult.Release();
	} while (bNb > 0);

	delete[] buffer;
	command.Release();


	// End Data Transfert
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.pid);
	command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
	result->hr = device_->SendCommand(0, command, &commandResult);
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}

	// Extract response code
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, reinterpret_cast<ULONG*>(&result->responseCode));

	// Extract response parameters
	result->hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &result->GetParams().GetCollection());

	CoTaskMemFree(context);
	return result;
}


MtpResponse* MtpDevice::SendWriteData(WORD operationCode, MtpParams& params, std::vector<BYTE> data) {
	MtpResponse* result = new MtpResponse();

	CComPtr<IPortableDeviceValues> command;
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		return result;
	}

	// Set command category and ID
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_WRITE.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_WRITE.pid);

	// Set operation code and parameters
	command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
	command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());
	command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_TOTAL_DATA_SIZE, data.size());

	// Send command
	CComPtr<IPortableDeviceValues> commandResult;
	result->hr = device_->SendCommand(0, command, &commandResult);
	if (FAILED(result->hr)) {
		return result;
	}

	LPWSTR context;
	result->hr = commandResult->GetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, &context);
	if (FAILED(result->hr)) {
		return result;
	}

	ULONG optimalSize;
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPTIMAL_TRANSFER_BUFFER_SIZE, &optimalSize);
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}

	command.Release();
	commandResult.Release();


	// Start Data Transfert
	DWORD offset = 0;
	while (offset < data.size()) {
		result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(result->hr)) {
			CoTaskMemFree(context);
			return result;
		}

		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_WRITE_DATA.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_WRITE_DATA.pid);
		command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);

		DWORD chunkSize = min(optimalSize, data.size() - offset);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_NUM_BYTES_TO_WRITE, chunkSize);
		command->SetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, &data[offset], chunkSize);

		result->hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(result->hr)) {
			CoTaskMemFree(context);
			return result;
		}

		offset += chunkSize;
		commandResult.Release();
		command.Release();
	}


	// End Data Transfert
	result->hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}
	command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.fmtid);
	command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.pid);
	command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
	result->hr = device_->SendCommand(0, command, &commandResult);
	if (FAILED(result->hr)) {
		CoTaskMemFree(context);
		return result;
	}

	// Extract response code
	result->hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, reinterpret_cast<ULONG*>(&result->responseCode));

	// Extract response parameters
	result->hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &result->GetParams().GetCollection());

	CoTaskMemFree(context);
	return result;
}


size_t MtpDevice::RegisterCallback(std::function<void(IPortableDeviceValues*)> callback) { return eventCallback_->RegisterCallback(callback); }
void MtpDevice::UnregisterCallback(size_t id) { return eventCallback_->UnregisterCallback(id); }


MtpDeviceInfoDS MtpDevice::GetDeviceInfo() {
	MtpParams params;
	MtpResponse* response = SendReadData(MtpOperationCode::GetDeviceInfo, params);
	if (FAILED(response->hr)) {
		throw std::runtime_error("Failed to execute GetDeviceInfo: " + response->hr);
	}

	if (response->responseCode != MtpResponseCode::OK) {
		throw MtpException(MtpOperationCode::GetDeviceInfo, response->responseCode);
	}

	MtpDeviceInfoDS deviceInfo;
	size_t offset = 0;
	uint32_t len;

	deviceInfo.StandardVersion = *(uint16_t *)(response->data.data() + offset);
	offset += sizeof(uint16_t);
	deviceInfo.VendorExtensionID = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	deviceInfo.VendorExtensionVersion = *(uint16_t*)(response->data.data() + offset);
	offset += sizeof(uint16_t);

	len = *(uint8_t*)(response->data.data() + offset);
	offset += sizeof(uint8_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.VendorExtensionDesc += *(char16_t*)(response->data.data() + offset);
		offset += sizeof(char16_t);
	}

	deviceInfo.FunctionalMode = *(uint16_t*)(response->data.data() + offset);
	offset += sizeof(uint16_t);

	len = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.OperationsSupported.push_back(*(uint16_t*)(response->data.data() + offset));
		offset += sizeof(uint16_t);
	}

	len = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.EventsSupported.push_back(*(uint16_t*)(response->data.data() + offset));
		offset += sizeof(uint16_t);
	}

	len = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.DevicePropertiesSupported.push_back(*(uint16_t*)(response->data.data() + offset));
		offset += sizeof(uint16_t);
	}

	len = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.CaptureFormats.push_back(*(uint16_t*)(response->data.data() + offset));
		offset += sizeof(uint16_t);
	}

	len = *(uint32_t*)(response->data.data() + offset);
	offset += sizeof(uint32_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.ImageFormats.push_back(*(uint16_t*)(response->data.data() + offset));
		offset += sizeof(uint16_t);
	}

	len = *(uint8_t*)(response->data.data() + offset);
	offset += sizeof(uint8_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.Manufacture += *(char16_t*)(response->data.data() + offset);
		offset += sizeof(char16_t);
	}

	len = *(uint8_t*)(response->data.data() + offset);
	offset += sizeof(uint8_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.Model += *(char16_t*)(response->data.data() + offset);
		offset += sizeof(char16_t);
	}

	len = *(uint8_t*)(response->data.data() + offset);
	offset += sizeof(uint8_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.DeviceVersion += *(char16_t*)(response->data.data() + offset);
		offset += sizeof(char16_t);
	}

	len = *(uint8_t*)(response->data.data() + offset);
	offset += sizeof(uint8_t);
	for (uint32_t i = 0; i < len; i++) {
		deviceInfo.SerialNumber += *(char16_t*)(response->data.data() + offset);
		offset += sizeof(char16_t);
	}

	delete response;
	return deviceInfo;
}