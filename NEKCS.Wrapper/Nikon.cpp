#include "pch.h"

#include "Nikon.h"

#pragma comment(lib, "delayimp")
#pragma comment(linker, "/DELAYLOAD:nikon.dll")
#include "nikon.hpp"



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