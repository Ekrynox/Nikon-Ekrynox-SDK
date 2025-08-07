#include "pch.h"

#include "Nikon.h"



using namespace NEKCS;



//NikonCamera
System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ NikonCamera::listNikonCameras() {
	System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ result = gcnew System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>();
	for (auto& camera : nek::NikonCamera::listNikonCameras()) {
		result->Add(gcnew System::String(camera.first.c_str()), gcnew NikonDeviceInfoDS(camera.second));
	}
	return result;
}

size_t NikonCamera::countNikonCameras() { return nek::NikonCamera::countNikonCameras(); }



NikonCamera::NikonCamera(System::String^ devicePath) {
	std::wstring str;
	for each (auto c in devicePath) {
		str += c;
	}
	m_nativeClass = new nek::NikonCamera(str);
};
NikonCamera::~NikonCamera() { this->!NikonCamera(); };
NikonCamera::!NikonCamera() {
	delete m_nativeClass;
	m_nativeClass = nullptr;
};

MtpResponse^ NikonCamera::SendCommand(NikonMtpOperationCode operationCode, MtpParams^ params) { return gcnew MtpResponse(m_nativeClass->SendCommand((System::UInt16)operationCode, *params->m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndRead(NikonMtpOperationCode operationCode, MtpParams^ params) { return gcnew MtpResponse(m_nativeClass->SendCommandAndRead((System::UInt16)operationCode, *params->m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndWrite(NikonMtpOperationCode operationCode, MtpParams^ params, System::Collections::Generic::List<System::Byte>^ data) {
	std::vector<BYTE> datac = std::vector<BYTE>();
	for each (auto item in data) {
		datac.push_back(item);
	}
	return gcnew MtpResponse(m_nativeClass->SendCommandAndWrite((System::UInt16)operationCode, *params->m_nativeClass, datac));
}



NikonDeviceInfoDS^ NikonCamera::GetDeviceInfo() { return gcnew NikonDeviceInfoDS(m_nativeClass->GetDeviceInfo()); }