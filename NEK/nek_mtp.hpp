#pragma once

#include <string>
#include <vector>

#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
#include <WpdMtpExtensions.h>



#define CLIENT_NAME			L"Nikon Ekrynox SDK"
#define CLIENT_MAJOR_VER	1
#define CLIENT_MINOR_VER	0
#define CLIENT_REVISION		0



namespace nek {
	namespace mtp {

		class MtpManager {
		public:
			static MtpManager& Instance();

			std::vector<std::wstring> listNikonCameras();
			int countNikonCameras();
			CComPtr<IPortableDevice> openDevice(PWSTR deviceId);

		private:
			CComPtr<IPortableDeviceManager> deviceManager_;
			CComPtr<IPortableDeviceValues> deviceClient_;

			MtpManager& operator= (const MtpManager&) = delete;
			MtpManager(const MtpManager&) = delete;
			MtpManager();
			~MtpManager();
		};


		class MtpDevice {
		public:
			MtpDevice(PWSTR deviceId);

			HRESULT SendCommand(WORD operationCode, IPortableDevicePropVariantCollection* operationParams, CComPtr<IPortableDeviceValues>& results);
			HRESULT GetIUnknownValue(IPortableDeviceValues& results, PROPVARIANT& responseCode, CComPtr<IPortableDevicePropVariantCollection>& responseParams);

		private:
			MtpManager *deviceManager_;
			CComPtr<IPortableDevice> device_;
		};

	}
}