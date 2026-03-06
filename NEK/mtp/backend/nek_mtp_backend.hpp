#pragma once

#include "../nek_mtp_utils.hpp"
#include "../nek_mtp_struct.hpp"

#include <map>
#include <optional>



namespace nek::mtp::backend {

	class IMtpTransport {
	public:
		virtual ~IMtpTransport() = default;

		virtual void connect() = 0;
		virtual void disconnect() = 0;
		virtual bool isConnected() const = 0;

		virtual MtpResponse sendCommand(uint16_t operationCode, const MtpParams& params) = 0;
		virtual MtpResponse sendCommandAndRead(uint16_t operationCode, const MtpParams& params) = 0;
		virtual MtpResponse sendCommandAndWrite(uint16_t operationCode, const MtpParams& params, const std::vector<uint8_t>& data) = 0;
	};


	struct MtpConnectionInfo_ {
		std::optional<std::wstring> usbPath = std::nullopt;

		std::unique_ptr<IMtpTransport> transport = nullptr;
	};
	typedef struct MtpConnectionInfo_ MtpConnectionInfo;


	class IMtpBackendProvider {
	public:
		virtual ~IMtpBackendProvider() = default;

		virtual std::vector<MtpConnectionInfo> listDevices() = 0;
		virtual size_t countDevices() = 0;
	};

}