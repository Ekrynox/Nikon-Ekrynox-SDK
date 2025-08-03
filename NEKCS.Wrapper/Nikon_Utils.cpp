#include "pch.h"

#include "Nikon_Utils.h"


using namespace NEKCS;



//MtpReponseParams
MtpReponseParams::MtpReponseParams() { m_nativeClass = new nek::mtp::MtpReponseParams(); }
MtpReponseParams::MtpReponseParams(nek::mtp::MtpReponseParams responseParams) : MtpReponseParams() { *m_nativeClass = responseParams; }
MtpReponseParams::~MtpReponseParams() { this->!MtpReponseParams(); }
MtpReponseParams::!MtpReponseParams() { delete m_nativeClass; }



//MtpParams
MtpParams::MtpParams() { m_nativeClass = new nek::mtp::MtpParams(); }
MtpParams::MtpParams(nek::mtp::MtpParams params) : MtpParams() { *m_nativeClass = params; }
MtpParams::~MtpParams() { this->!MtpParams(); }
MtpParams::!MtpParams() { delete m_nativeClass; }

void MtpParams::addUint32(System::UInt32 param) { m_nativeClass->addUint32(param); }
void MtpParams::addUint16(System::UInt16 param) { m_nativeClass->addUint16(param); }
void MtpParams::addInt32(System::Int32 param) { m_nativeClass->addInt32(param); }
void MtpParams::addInt16(System::Int16  param) { m_nativeClass->addInt16(param); }



//MtpResponse
MtpResponse::MtpResponse() {
	hr = E_FAIL;
	responseCode = 0;
	responseParams_ = gcnew MtpReponseParams();
}
MtpResponse::MtpResponse(nek::mtp::MtpResponse response) {
	hr = response.hr;
	responseCode = response.responseCode;
	responseParams_ = gcnew MtpReponseParams(response.GetParams());
	for (auto d : response.data) {
		data.Add(d);
	}
}
MtpResponse::~MtpResponse() { this->!MtpResponse(); }
MtpResponse::!MtpResponse() { delete responseParams_; }

MtpReponseParams^ MtpResponse::GetParams() { return responseParams_; }



//MtpEvent
MtpEvent::MtpEvent(nek::mtp::MtpEvent event) {
	eventCode = event.eventCode;
	for (auto p : event.eventParams) {
		eventParams.Add(p);
	}
}