#include "nek_mtp_except.hpp"



using namespace nek::mtp;


MtpException::MtpException(uint32_t operationCode, uint32_t responseCode) {
	operationCode_ = operationCode;
	responseCode_ = responseCode;
}

uint32_t MtpException::operationCode() const { return operationCode_; }
uint32_t MtpException::responseCode() const { return responseCode_; }