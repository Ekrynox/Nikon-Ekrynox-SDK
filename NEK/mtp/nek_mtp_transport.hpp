#pragma once

#include "nek_mtp_utils.hpp"



namespace nek::mtp {

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

}