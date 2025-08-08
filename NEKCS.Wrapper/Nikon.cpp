#include "pch.h"

#include "Nikon.h"



using namespace NEKCS;



//NikonCamera
System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ NikonCamera::listNikonCameras(System::Boolean onlyOn) {
	System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ result = gcnew System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>();
	for (auto& camera : nek::NikonCamera::listNikonCameras(onlyOn)) {
		result->Add(gcnew System::String(camera.first.c_str()), gcnew NikonDeviceInfoDS(camera.second));
	}
	return result;
}
System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ NikonCamera::listNikonCameras() { return listNikonCameras(true); }

size_t NikonCamera::countNikonCameras(System::Boolean onlyOn) { return nek::NikonCamera::countNikonCameras(onlyOn); }
size_t NikonCamera::countNikonCameras() { return countNikonCameras(true); }



NikonCamera::NikonCamera(System::String^ devicePath, System::Byte additionThreads) {
	std::wstring str;
	for each (auto c in devicePath) {
		str += c;
	}
	m_nativeClass = new nek::NikonCamera(str, additionThreads);
};
NikonCamera::NikonCamera(System::String^ devicePath) : NikonCamera(devicePath, 0) {};
NikonCamera::~NikonCamera() { this->!NikonCamera(); };
NikonCamera::!NikonCamera() {
	delete m_nativeClass;
	m_nativeClass = nullptr;
};


System::Boolean NikonCamera::isConnected() {
	return m_nativeClass->isConnected();
}



MtpResponse^ NikonCamera::SendCommand(NikonMtpOperationCode operationCode, MtpParams^ params) { return gcnew MtpResponse(m_nativeClass->SendCommand((System::UInt16)operationCode, *params->m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndRead(NikonMtpOperationCode operationCode, MtpParams^ params) { return gcnew MtpResponse(m_nativeClass->SendCommandAndRead((System::UInt16)operationCode, *params->m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndWrite(NikonMtpOperationCode operationCode, MtpParams^ params, array<System::Byte>^ data) {
	std::vector<BYTE> datac = std::vector<BYTE>();
	datac.resize(data->Length);
	if (data->Length > 0) {
		pin_ptr<System::Byte> dataptr = &data[0];
		std::memcpy(datac.data(), dataptr, data->Length);
	}
	return gcnew MtpResponse(m_nativeClass->SendCommandAndWrite((System::UInt16)operationCode, *params->m_nativeClass, datac));
}



NikonDeviceInfoDS^ NikonCamera::GetDeviceInfo() { return gcnew NikonDeviceInfoDS(m_nativeClass->GetDeviceInfo()); }