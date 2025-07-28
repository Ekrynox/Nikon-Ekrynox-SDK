#pragma once

#include <string>
#include <vector>

#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>



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

		private:
			MtpManager& operator= (const MtpManager&) = delete;
			MtpManager(const MtpManager&) = delete;

			CComPtr<IPortableDeviceManager> deviceManager_;
			CComPtr<IPortableDeviceValues> deviceClient_;

			MtpManager();
			~MtpManager();
		};

	}
}