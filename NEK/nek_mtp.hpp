#pragma once
#include "nek.hpp"
#include "nek_mtp_utils.hpp"

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
			NEK_API static MtpManager& Instance();

			NEK_API std::vector<MtpDeviceInfo> listMtpDevices();
			NEK_API size_t countMtpDevices();
			NEK_API CComPtr<IPortableDevice> openDevice(PWSTR deviceId);

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
			NEK_API MtpDevice(PWSTR devicePath);
			NEK_API ~MtpDevice();

			NEK_API MtpResponse SendNoData(WORD operationCode, MtpParams& params);
			NEK_API MtpResponse SendReadData(WORD operationCode, MtpParams& params);
			NEK_API MtpResponse SendWriteData(WORD operationCode, MtpParams& params, std::vector<BYTE> data);

			NEK_API size_t RegisterCallback(std::function<void(IPortableDeviceValues*)> callback);
			NEK_API void UnregisterCallback(size_t id);

		private:
			CComPtr<IPortableDevice> device_;
			CComPtr<MtpEventCallback> eventCallback_;
			PWSTR eventCookie;
		};

	}
}