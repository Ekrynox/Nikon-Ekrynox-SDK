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

		class MtpDevice {
		public:
			static MtpDevice& Instance();

			std::vector<std::wstring> listNikonCameras();
			int countNikonCameras();

		private:
			MtpDevice& operator= (const MtpDevice&) = delete;
			MtpDevice(const MtpDevice&) = delete;

			CComPtr<IPortableDeviceManager> deviceManager_;

			MtpDevice();
			~MtpDevice();
		};

	}
}