#pragma once
#include "nikon.hpp"
#include "Nikon_Utils.h"
#include "Nikon_Enum.h"
#include "Nikon_Struct.h"

#include <vcclr.h>



namespace NEKCS {

	public ref class NikonCamera {
	private:
		nek::NikonCamera* m_nativeClass;
		System::Collections::Generic::Dictionary<MtpEventHandler^, size_t> _callbackIds;

	public:
		static System::Collections::Generic::Dictionary<System::String^, NikonDeviceInfoDS^>^ listNikonCameras();
		static size_t countNikonCameras();

		NikonCamera(System::String^ devicePath, System::Byte additionThreads);
		NikonCamera(System::String^ devicePath);
		~NikonCamera();
		!NikonCamera();

		MtpResponse^ SendCommand(NikonMtpOperationCode operationCode, MtpParams^ params);
		MtpResponse^ SendCommandAndRead(NikonMtpOperationCode operationCode, MtpParams^ params);
		MtpResponse^ SendCommandAndWrite(NikonMtpOperationCode operationCode, MtpParams^ params, System::Collections::Generic::List<System::Byte>^ data);


		event MtpEventHandler^ OnMtpEvent {
			void add(MtpEventHandler^ handler) {
				if (_callbackIds.ContainsKey(handler)) {
					return;
				}
				auto dispatcher = new gcroot<MtpEventHandlerHelper^>(gcnew MtpEventHandlerHelper(this, handler));
				size_t callbackId = m_nativeClass->RegisterCallback(MtpEventHandlerHelper::createCallback(dispatcher));
				_callbackIds[handler] = callbackId;
			}
			void remove(MtpEventHandler^ handler) {
				if (_callbackIds.ContainsKey(handler)) {
					this->m_nativeClass->UnregisterCallback(_callbackIds[handler]);
					_callbackIds.Remove(handler);
				}
			}
		}


		NikonDeviceInfoDS^ GetDeviceInfo();
	};

}