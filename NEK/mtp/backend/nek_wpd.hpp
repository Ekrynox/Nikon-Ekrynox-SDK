#pragma once
#include "../../nek.hpp"
#include "nek_mtp_backend.hpp"

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>


#define NOMINMAX
#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
#include <propvarutil.h>
#include <WpdMtpExtensions.h>

#define CLIENT_NAME			L"Nikon Ekrynox SDK"
#define CLIENT_MAJOR_VER	1
#define CLIENT_MINOR_VER	0
#define CLIENT_REVISION		0



namespace nek::mtp::backend::wpd {

	class WpdMtpTransport : public IMtpTransport {
	public:
		NEK_API WpdMtpTransport(const std::wstring& devicePath);
		NEK_API ~WpdMtpTransport();

		NEK_API void connect() override;
		NEK_API void disconnect() override;
		NEK_API bool isConnected() const override;

		NEK_API MtpResponse sendCommand(uint16_t operationCode, const std::vector<uint32_t>& params) override;
		NEK_API MtpResponse sendCommandAndRead(uint16_t operationCode, const std::vector<uint32_t>& params) override;
		NEK_API MtpResponse sendCommandAndWrite(uint16_t operationCode, const std::vector<uint32_t>& params, const std::vector<uint8_t>& data) override;

		NEK_API void registerEventCallback(std::function<void(MtpEvent)>* eventCallback) override;
		NEK_API void unregisterEventCallback() override;

	private:
		void initCom();
		void initDevice();
		void commandLoop();

		MtpResponse sendCommand_(uint16_t operationCode, const std::vector<uint32_t>& params);
		MtpResponse sendCommandAndRead_(uint16_t operationCode, const std::vector<uint32_t>& params);
		MtpResponse sendCommandAndWrite_(uint16_t operationCode, const std::vector<uint32_t>& params, const std::vector<uint8_t>& data);

		std::atomic_bool running_;
		std::mutex commandMutex_;
		std::condition_variable commandCV_;
		std::thread commandThread_;
		std::function<void()>* command_;

		std::wstring devicePath_;
		CComPtr<IPortableDeviceValues> deviceClient_;
		CComPtr<IPortableDevice> device_;

		class WpdMtpEventManager : public IPortableDeviceEventCallback {
		public:
			HRESULT QueryInterface(REFIID riid, void** ppvObject) override;
			ULONG AddRef(void) override;
			ULONG Release(void) override;
			HRESULT OnEvent(IPortableDeviceValues* pEventParameters) override;
		private:
			ULONG ref_;
		};
		PWSTR eventCookie_;
		CComPtr<WpdMtpEventManager> eventManager_;
		std::function<void(MtpEvent)>* eventCallback_;

	};



	class WpdMtpBackendProvider : public IMtpBackendProvider {
	public:
		NEK_API WpdMtpBackendProvider() {};
		NEK_API ~WpdMtpBackendProvider() {};

		NEK_API std::vector<MtpConnectionInfo> listDevices();
		NEK_API size_t countDevices();
	};

}