#include "nek_mtp.hpp"

#include <stdexcept>



namespace nek {
	namespace mtp {

		MtpDevice::MtpDevice() {
			HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (FAILED(hr)) {
				throw std::runtime_error("Failed to init COM");
			}

			hr = CoCreateInstance(CLSID_PortableDeviceManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceManager_));
			if (FAILED(hr))
			{
				CoUninitialize();
				throw std::runtime_error("Impossible to create the Portable Device Manager");
			}
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

			return nikonCameras;
		}

		int MtpDevice::countNikonCameras() {
			return listNikonCameras().size();
		}

	}
}