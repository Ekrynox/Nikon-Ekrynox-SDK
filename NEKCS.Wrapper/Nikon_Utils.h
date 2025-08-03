#pragma once
#include "mtp/nek_mtp_utils.hpp"



namespace NEKCS {

	public ref class MtpReponseParams {
	internal:
		nek::mtp::MtpReponseParams* m_nativeClass;
		MtpReponseParams(nek::mtp::MtpReponseParams responseParams);
		MtpReponseParams();

	public:
		~MtpReponseParams();
		!MtpReponseParams();
	};

	
	public ref class MtpParams {
	internal:
		nek::mtp::MtpParams* m_nativeClass;
		MtpParams(nek::mtp::MtpParams params);

	public:
		MtpParams();
		~MtpParams();
		!MtpParams();

		void addUint32(System::UInt32 param);
		void addUint16(System::UInt16 param);
		void addInt32(System::Int32 param);
		void addInt16(System::Int16 param);
	};


	public ref class MtpResponse {
	internal:
		MtpResponse();
		MtpResponse(nek::mtp::MtpResponse response);

	public:
		~MtpResponse();
		!MtpResponse();

		MtpReponseParams^ GetParams();

		System::Int32 hr;
		System::UInt32 responseCode;
		System::Collections::Generic::List<System::Byte> data;
	internal:
		MtpReponseParams^ responseParams_;
	};


	public ref class MtpEvent {
	internal:
		MtpEvent(nek::mtp::MtpEvent event);

	public:
		System::UInt16 eventCode;
		System::Collections::Generic::List<System::UInt32> eventParams;
	};

}