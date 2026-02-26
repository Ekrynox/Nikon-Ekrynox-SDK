#pragma once
#include "../../nek.hpp"
#include "nek_mtp_backend.hpp"

#include <atomic>
#include <mutex>
#include <thread>


#define NOMINMAX
#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
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

		NEK_API MtpResponse sendCommand(uint16_t operationCode, const MtpParams& params) override;
		NEK_API MtpResponse sendCommandAndRead(uint16_t operationCode, const MtpParams& params) override;
		NEK_API MtpResponse sendCommandAndWrite(uint16_t operationCode, const MtpParams& params, const std::vector<uint8_t>& data) override;

	private:
		void initCom();
		void initDevice();
		void commandLoop();

		MtpResponse sendCommand_(uint16_t operationCode, const MtpParams& params);
		MtpResponse sendCommandAndRead_(uint16_t operationCode, const MtpParams& params);
		MtpResponse sendCommandAndWrite_(uint16_t operationCode, const MtpParams& params, const std::vector<uint8_t>& data);

		std::atomic_bool running_;
		std::mutex commandMutex_;
		std::condition_variable commandCV_;
		std::thread commandThread_;
		std::function<void()>* command_;

		std::wstring devicePath_;
		CComPtr<IPortableDeviceValues> deviceClient_;
		CComPtr<IPortableDevice> device_;
	};

}