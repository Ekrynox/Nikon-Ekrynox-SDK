#include "nek_mtp_except.hpp"

#include <sstream>



using namespace nek::mtp;



//MtpDeviceException
MtpDeviceException::MtpDeviceException(MtpExPhase phase, HRESULT hr) : phase(phase), code(computeCode(phase, hr)) {};

MtpExCode MtpDeviceException::computeCode(MtpExPhase phase, HRESULT hr) {
	if (hr == S_OK) return NO_ERR;
	if (hr == E_OUTOFMEMORY) return MEMORY;
	if (hr == E_UNEXPECTED) return UNEXPECTED;
	if (hr == E_INVALIDARG) return INVALIDARG;
	if (hr == E_POINTER) return INVALIDARG;

	switch (phase) {
	case COM_INIT:
		switch (hr) {
		case S_FALSE:
		case RPC_E_CHANGED_MODE:
			return ALREADY_INITIALIZED;
		default:
			return UNKNOW_ERR;
		}

	case MANAGER_INIT:
		switch (hr) {
		case E_NOINTERFACE:
		case CLASS_E_NOAGGREGATION:
		case REGDB_E_CLASSNOTREG:
			return INVALIDARG;
		default:
			return UNKNOW_ERR;
		}

	default:
		return UNKNOW_ERR;
	}
}



//MtpException
MtpException::MtpException(uint32_t operationCode, uint32_t responseCode) : operationCode(operationCode), responseCode(responseCode) {}

const char* MtpException::what() const noexcept {
	std::ostringstream message;
	message << "MTP Error - Operation: 0x" << std::hex << operationCode;
	message << ", Response: 0x" << std::hex << responseCode;
	return message.str().c_str();
}