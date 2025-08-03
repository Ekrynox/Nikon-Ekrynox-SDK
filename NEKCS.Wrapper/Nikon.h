#pragma once
#include "nikon.hpp"
#include "Nikon_Utils.h"
#include "Nikon_Struct.h"



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

		MtpResponse^ SendCommand(System::UInt16 operationCode, MtpParams params);
		MtpResponse^ SendCommandAndRead(System::UInt16 operationCode, MtpParams params);
		MtpResponse^ SendCommandAndWrite(System::UInt16 operationCode, MtpParams params, System::Collections::Generic::List<System::Byte> data);

		/*size_t RegisterCallback(std::function<void(MtpEvent)> callback);
		void UnregisterCallback(size_t id);*/


		NikonDeviceInfoDS GetDeviceInfo();
	};

}