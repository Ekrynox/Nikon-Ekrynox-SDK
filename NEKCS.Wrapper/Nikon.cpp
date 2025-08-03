#include "pch.h"

#include "Nikon.h"



using namespace NEKCS;



System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ NikonCamera::listNikonCameras() {
	System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ result = gcnew System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>();
	for (auto& camera : nek::NikonCamera::listNikonCameras()) {
		result->Add(gcnew System::String(camera.first.c_str()), gcnew NikonDeviceInfoDS(camera.second));
	}
	return result;
}

size_t NikonCamera::countNikonCameras() { return nek::NikonCamera::countNikonCameras(); }

NikonCamera::NikonCamera(std::wstring devicePath) { m_nativeClass = new nek::NikonCamera(devicePath); };
NikonCamera::~NikonCamera() { this->!NikonCamera(); };
NikonCamera::!NikonCamera() { delete m_nativeClass; };

MtpResponse^ NikonCamera::SendCommand(System::UInt16 operationCode, MtpParams params) { return gcnew MtpResponse(m_nativeClass->SendCommand(operationCode, *params.m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndRead(System::UInt16 operationCode, MtpParams params) { return gcnew MtpResponse(m_nativeClass->SendCommandAndRead(operationCode, *params.m_nativeClass)); }
MtpResponse^ NikonCamera::SendCommandAndWrite(System::UInt16 operationCode, MtpParams params, System::Collections::Generic::List<System::Byte> data) { 
	std::vector<BYTE> datac = std::vector<BYTE>();
	for each (auto item in data) {
		datac.push_back(item);
	}
	return gcnew MtpResponse(m_nativeClass->SendCommandAndWrite(operationCode, *params.m_nativeClass, datac));
}

/*size_t RegisterCallback(std::function<void(MtpEvent)> callback);
void UnregisterCallback(size_t id);*/


NikonDeviceInfoDS NikonCamera::GetDeviceInfo() { return NikonDeviceInfoDS(m_nativeClass->GetDeviceInfo()); }