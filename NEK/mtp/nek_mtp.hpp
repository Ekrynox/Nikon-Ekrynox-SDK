#pragma once
#include "../nek.hpp"
#include "../utils/nek_threading.hpp"
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



namespace nek::mtp {

	class NEK_API MtpManager : protected nek::utils::ThreadedClass {
	public:
		static MtpManager& Instance();

		std::map<std::wstring, MtpDeviceInfoDS> listMtpDevices();
		size_t countMtpDevices();

	private:
		MtpManager& operator= (const MtpManager&) = delete;
		MtpManager(const MtpManager&) = delete;
		MtpManager();
		~MtpManager() { nek::utils::ThreadedClass::~ThreadedClass(); };
		void threadTask();

		CComPtr<IPortableDeviceManager> deviceManager_;
		std::mutex mutexDevice_;
	};



	class NEK_API MtpDevice : protected nek::utils::MultiThreadedClass {
	public:
		MtpDevice(const PWSTR devicePath);
		~MtpDevice() { nek::utils::MultiThreadedClass::~MultiThreadedClass(); };

		MtpResponse SendCommand(WORD operationCode, MtpParams params);
		MtpResponse SendCommandAndRead(WORD operationCode, MtpParams params);
		MtpResponse SendCommandAndWrite(WORD operationCode, MtpParams params, std::vector<BYTE> data);

		size_t RegisterCallback(std::function<void(MtpEvent)> callback);
		void UnregisterCallback(size_t id);


		MtpDeviceInfoDS GetDeviceInfo();


	protected:
		MtpDevice();
		void mainThreadTask();

		MtpResponse SendCommand_(CComPtr<IPortableDevice> device, WORD operationCode, MtpParams params);
		MtpResponse SendCommandAndRead_(CComPtr<IPortableDevice> device, WORD operationCode, MtpParams params);
		MtpResponse SendCommandAndWrite_(CComPtr<IPortableDevice> device, WORD operationCode, MtpParams params, std::vector<BYTE> data);

		PWSTR devicePath_;
		CComPtr<IPortableDeviceValues> deviceClient_;
		CComPtr<IPortableDevice> device_;
		CComPtr<MtpEventCallback> eventCallback_;
		PWSTR eventCookie_;
		std::mutex mutexDevice_;
	};

}