#pragma once
#include "nek.hpp"
#include "nek_mtp_utils.hpp"
#include "nek_mtp_enum.hpp"
#include "nek_mtp_struct.hpp"

#include <map>
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

		class NEK_API MtpManager {
		public:
			static MtpManager& Instance();

			std::map<std::wstring, MtpDeviceInfoDS> listMtpDevices();
			size_t countMtpDevices();
			CComPtr<IPortableDevice> openDevice(PWSTR devicePath);

		private:
			CComPtr<IPortableDeviceManager> deviceManager_;
			CComPtr<IPortableDeviceValues> deviceClient_;

			MtpManager& operator= (const MtpManager&) = delete;
			MtpManager(const MtpManager&) = delete;
			MtpManager();
			~MtpManager();
		};


		class NEK_API MtpDevice {
		public:
			MtpDevice(PWSTR devicePath);
			~MtpDevice();

			MtpResponse SendNoData(WORD operationCode, MtpParams& params);
			MtpResponse SendReadData(WORD operationCode, MtpParams& params);
			MtpResponse SendWriteData(WORD operationCode, MtpParams& params, std::vector<BYTE> data);

			size_t RegisterCallback(std::function<void(IPortableDeviceValues*)> callback);
			void UnregisterCallback(size_t id);


			MtpDeviceInfoDS GetDeviceInfo();


		private:
			CComPtr<IPortableDevice> device_;
			CComPtr<MtpEventCallback> eventCallback_;
			PWSTR eventCookie;
		};

	}
}