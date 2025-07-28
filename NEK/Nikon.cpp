#include "nikon.hpp"
#include "nek_mtp.hpp"

#include <stdexcept>



int countNikonCameras() {
	return nek::mtp::MtpManager::Instance().countNikonCameras();
}


std::string test() {
	nek::mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto nikonCameras = deviceManager->listNikonCameras();
	if (nikonCameras.empty()) {
		return "";
	}

	auto device = nek::mtp::MtpDevice((PWSTR)nikonCameras[0].c_str());

	CComPtr<IPortableDevicePropVariantCollection> params;
	HRESULT hr = CoCreateInstance(CLSID_PortableDevicePropVariantCollection, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&params));
	if (SUCCEEDED(hr)) {
		PROPVARIANT param;
		PropVariantInit(&param);
		param.vt = VT_UI4;
		param.ulVal = 3;
		//params->Add(&param);

		CComPtr<IPortableDeviceValues> results;
		hr = device.SendCommand(0x100E, params, results);
		
		PROPVARIANT responseCode;
		CComPtr<IPortableDevicePropVariantCollection> responseParams;
		device.GetIUnknownValue(*results, responseCode, responseParams);
		return "" + std::to_string(responseCode.intVal);
	}

	return "";
}