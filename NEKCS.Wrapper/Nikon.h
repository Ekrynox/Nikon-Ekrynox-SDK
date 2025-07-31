#pragma once
#include "Nikon_Struct.h"


namespace nek { class NikonCamera; }


namespace NEKCS {

	public ref class NikonCamera {
	private:
		nek::NikonCamera* m_nativeClass;

	public:
		static System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ listNikonCameras();
		static size_t countNikonCameras();

		NikonCamera(std::wstring devicePath);
		~NikonCamera();
		!NikonCamera();
	};

}