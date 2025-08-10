#pragma once
#include "mtp/nek_mtp_struct.hpp"
#include "Nikon_Enum.h"



namespace NEKCS {

	public ref struct NikonDeviceInfoDS {
		System::UInt16 StandardVersion = 0;
		System::UInt32 VendorExtensionID = 0;
		System::UInt16 VendorExtensionVersion = 0;
		System::String^ VendorExtensionDesc = "";
		System::UInt16 FunctionalMode = 0;
		System::Collections::Generic::List<NikonMtpOperationCode>^ OperationsSupported;
		System::Collections::Generic::List<NikonMtpEventCode>^ EventsSupported;
		System::Collections::Generic::List<NikonMtpDevicePropCode>^ DevicePropertiesSupported;
		System::Collections::Generic::List<System::UInt16>^ CaptureFormats;
		System::Collections::Generic::List<System::UInt16>^ ImageFormats;
		System::String^ Manufacture = "";
		System::String^ Model = "";
		System::String^ DeviceVersion = "";
		System::String^ SerialNumber = "";

		NikonDeviceInfoDS();
		NikonDeviceInfoDS(const nek::mtp::MtpDeviceInfoDS& native);
	};


	public ref class MtpDatatypeVariant {
	private:
		NikonMtpDatatypeCode m_type;
		Object^ m_value;

	public:
		MtpDatatypeVariant(System::SByte data);
		MtpDatatypeVariant(System::Byte data);
		MtpDatatypeVariant(System::Int16 data);
		MtpDatatypeVariant(System::UInt16 data);
		MtpDatatypeVariant(System::Int32 data);
		MtpDatatypeVariant(System::UInt32 data);
		MtpDatatypeVariant(System::Int64 data);
		MtpDatatypeVariant(System::UInt64 data);

		MtpDatatypeVariant(array<System::SByte>^ data);
		MtpDatatypeVariant(array<System::Byte>^ data);
		MtpDatatypeVariant(array<System::Int16>^ data);
		MtpDatatypeVariant(array<System::UInt16>^ data);
		MtpDatatypeVariant(array<System::Int32>^ data);
		MtpDatatypeVariant(array<System::UInt32>^ data);
		MtpDatatypeVariant(array<System::Int64>^ data);
		MtpDatatypeVariant(array<System::UInt64>^ data);

		MtpDatatypeVariant(System::String^ data);

		MtpDatatypeVariant(const nek::mtp::MtpDatatypeVariant& data);

		System::Boolean TryGetInt8(System::SByte% data);
		System::Boolean TryGetUInt8(System::Byte% data);
		System::Boolean TryGetInt16(System::Int16% data);
		System::Boolean TryGetUInt16(System::UInt16% data);
		System::Boolean TryGetInt32(System::Int32% data);
		System::Boolean TryGetUInt32(System::UInt32% data);
		System::Boolean TryGetInt64(System::Int64% data);
		System::Boolean TryGetUInt64(System::UInt64% data);

		System::Boolean TryGetArrayInt8(array<System::SByte>^% data);
		System::Boolean TryGetArrayUInt8(array<System::Byte>^% data);
		System::Boolean TryGetArrayInt16(array<System::Int16>^% data);
		System::Boolean TryGetArrayUInt16(array<System::UInt16>^% data);
		System::Boolean TryGetArrayInt32(array<System::Int32>^% data);
		System::Boolean TryGetArrayUInt32t(array<System::UInt32>^% data);
		System::Boolean TryGetArrayInt64(array<System::Int64>^% data);
		System::Boolean TryGetArrayUInt64(array<System::UInt64>^% data);

		System::Boolean TryGetString(System::String^% data);

	internal:
		nek::mtp::MtpDatatypeVariant getVariant();

	private:
		void SetInt8(int8_t data);
		void SetUInt8(uint8_t data);
		void SetInt16(int16_t data);
		void SetUInt16(uint16_t data);
		void SetInt32(int32_t data);
		void SetUInt32(uint32_t data);
		void SetInt64(int64_t data);
		void SetUInt64(uint64_t data);

		void SetArrayInt8(const std::vector<int8_t>& data);
		void SetArrayUInt8(const std::vector<uint8_t>& data);
		void SetArrayInt16(const std::vector<int16_t>& data);
		void SetArrayUInt16(const std::vector<uint16_t>& data);
		void SetArrayInt32(const std::vector<int32_t>& data);
		void SetArrayUInt32(const std::vector<uint32_t>& data);
		void SetArrayInt64(const std::vector<int64_t>& data);
		void SetArrayUInt64(const std::vector<uint64_t>& data);

		void SetString(const std::wstring& data);
	};

}