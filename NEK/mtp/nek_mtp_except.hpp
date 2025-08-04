#pragma once
#include "../nek.hpp"

#include <stdlib.h>
#include <exception>
#include <string>

#include <atlbase.h>



namespace nek::mtp {

	enum MtpExPhase {
		COM_INIT = 0,
		MANAGER_INIT = 1,
	};

	enum MtpExCode {
		NO_ERR = 0,
		UNKNOW_ERR = 1,
		UNEXPECTED = 2,
		MEMORY = 3,
		ALREADY_INITIALIZED = 4,
		INVALIDARG = 5,
	};



	class NEK_API MtpDeviceException {
	public:
		MtpDeviceException(MtpExPhase phase, HRESULT hr);

		const MtpExPhase phase;
		const MtpExCode code;

	private:
		static MtpExCode computeCode(MtpExPhase phase, HRESULT hr);
	};



	class NEK_API MtpException : public std::exception {
	public:
		MtpException(uint32_t operationCode, uint32_t responseCode);
		const char* what() const noexcept override;

		const uint32_t operationCode;
		const uint32_t responseCode;
	};

}