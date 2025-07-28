#include "nek_mtp.hpp"

#include <algorithm>
#include <stdexcept>



namespace nek {
	namespace mtp {

		MtpDevice::MtpDevice() {
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

		MtpDevice::~MtpDevice() {
			CoUninitialize();
		}

		MtpDevice& MtpDevice::Instance() {
			static MtpDevice instance;
			return instance;
		}



		std::vector<std::wstring> MtpDevice::listNikonCameras() {
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

		int MtpDevice::countNikonCameras() {
			return listNikonCameras().size();
		}

	}
}