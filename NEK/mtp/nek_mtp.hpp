#pragma once
#include "../nek.hpp"
#include "nek_mtp_utils.hpp"
#include "nek_mtp_enum.hpp"
#include "nek_mtp_struct.hpp"
#include "nek_mtp_except.hpp"

#include <atomic>
#include <future>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
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

		private:
			MtpManager& operator= (const MtpManager&) = delete;
			MtpManager(const MtpManager&) = delete;
			MtpManager();
			~MtpManager();
			void threadTask();

			void sendTaskAsync(std::function<void()> task);
			void sendTask(std::function<void()> task);
			template<typename T> T sendTaskWithResult(std::function<T()> task);

			CComPtr<IPortableDeviceManager> deviceManager_;
			std::queue<std::function<void()>> tasks_;
			std::thread thread_;
			std::atomic<bool> running_;
			std::mutex mutexDevice_;
			std::mutex mutexTasks_;
			std::condition_variable cvTasks_;
		};




		class NEK_API MtpDevice {
		public:
			MtpDevice(const PWSTR devicePath);
			~MtpDevice();

			MtpResponse* SendNoData(WORD operationCode, MtpParams& params);
			MtpResponse* SendReadData(WORD operationCode, MtpParams& params);
			MtpResponse* SendWriteData(WORD operationCode, MtpParams& params, std::vector<BYTE> data);

			size_t RegisterCallback(std::function<void(IPortableDeviceValues*)> callback);
			void UnregisterCallback(size_t id);


			MtpDeviceInfoDS GetDeviceInfo();


		private:
			CComPtr<IPortableDeviceValues> deviceClient_;
			CComPtr<IPortableDevice> device_;
			CComPtr<MtpEventCallback> eventCallback_;
			PWSTR eventCookie;
		};

	}
}