#include "pch.h"

#include "Nikon_Struct.h"

#include <assert.h>


using namespace NEKCS;


//NikonDeviceInfoDS
NikonDeviceInfoDS::NikonDeviceInfoDS() {
	StandardVersion = 0;
	VendorExtensionID = 0;
	VendorExtensionVersion = 0;
	VendorExtensionDesc = gcnew System::String("");
	FunctionalMode = 0;
	OperationsSupported = gcnew System::Collections::Generic::List<NikonMtpOperationCode>();
	EventsSupported = gcnew System::Collections::Generic::List<NikonMtpEventCode>();
	DevicePropertiesSupported = gcnew System::Collections::Generic::List<NikonMtpDevicePropCode>();
	CaptureFormats = gcnew System::Collections::Generic::List<System::UInt16>();
	ImageFormats = gcnew System::Collections::Generic::List<System::UInt16>();
	Manufacture = gcnew System::String("");
	Model = gcnew System::String("");
	DeviceVersion = gcnew System::String("");
	SerialNumber = gcnew System::String("");
};

NikonDeviceInfoDS::NikonDeviceInfoDS(const nek::mtp::MtpDeviceInfoDS& native) {
	StandardVersion = native.StandardVersion;
	VendorExtensionID = native.VendorExtensionID;
	VendorExtensionVersion = native.VendorExtensionVersion;
	VendorExtensionDesc = gcnew System::String(native.VendorExtensionDesc.c_str());
	FunctionalMode = native.FunctionalMode;

	OperationsSupported = gcnew System::Collections::Generic::List<NikonMtpOperationCode>();
	for (auto el : native.OperationsSupported) {
		OperationsSupported->Add((NikonMtpOperationCode)el);
	}
	EventsSupported = gcnew System::Collections::Generic::List<NikonMtpEventCode>();
	for (auto el : native.EventsSupported) {
		EventsSupported->Add((NikonMtpEventCode)el);
	}
	DevicePropertiesSupported = gcnew System::Collections::Generic::List<NikonMtpDevicePropCode>();
	for (auto el : native.DevicePropertiesSupported) {
		DevicePropertiesSupported->Add((NikonMtpDevicePropCode)el);
	}
	CaptureFormats = gcnew System::Collections::Generic::List<System::UInt16>();
	for (auto el : native.CaptureFormats) {
		CaptureFormats->Add(el);
	}
	ImageFormats = gcnew System::Collections::Generic::List<System::UInt16>();
	for (auto el : native.ImageFormats) {
		ImageFormats->Add(el);
	}

	Manufacture = gcnew System::String(native.Manufacture.c_str());
	Model = gcnew System::String(native.Model.c_str());
	DeviceVersion = gcnew System::String(native.DeviceVersion.c_str());
	SerialNumber = gcnew System::String(native.SerialNumber.c_str());
};



//MtpDatatypeVariant
MtpDatatypeVariant::MtpDatatypeVariant(const nek::mtp::MtpDatatypeVariant& data) {
	if (data.index() == 1) {
		m_type = NikonMtpDatatypeCode::Int8;
		m_value = gcnew System::SByte(std::get<int8_t>(data));
	}
	else if (data.index() == 2) {
		m_type = NikonMtpDatatypeCode::UInt8;
		m_value = gcnew System::Byte(std::get<uint8_t>(data));
	}
	else if (data.index() == 3) {
		m_type = NikonMtpDatatypeCode::Int16;
		m_value = gcnew System::Int16(std::get<int16_t>(data));
	}
	else if (data.index() == 4) {
		m_type = NikonMtpDatatypeCode::UInt16;
		m_value = gcnew System::UInt16(std::get<uint16_t>(data));
	}
	else if (data.index() == 5) {
		m_type = NikonMtpDatatypeCode::Int32;
		m_value = gcnew System::Int32(std::get<int32_t>(data));
	}
	else if (data.index() == 6) {
		m_type = NikonMtpDatatypeCode::UInt32;
		m_value = gcnew System::UInt32(std::get<uint32_t>(data));
	}
	else if (data.index() == 7) {
		m_type = NikonMtpDatatypeCode::Int64;
		m_value = gcnew System::Int64(std::get<int64_t>(data));
	}
	else if (data.index() == 8) {
		m_type = NikonMtpDatatypeCode::UInt64;
		m_value = gcnew System::UInt64(std::get<uint64_t>(data));
	}
	else if (data.index() == 9) {
		m_type = NikonMtpDatatypeCode::Int128;
		throw gcnew System::NotImplementedException();
	}
	else if (data.index() == 10) {
		m_type = NikonMtpDatatypeCode::UInt128;
		throw gcnew System::NotImplementedException();
	}
	else if (data.index() == 11) {
		assert(std::get<std::vector<int8_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayInt8;
		auto value = gcnew array<System::SByte>(static_cast<int>(std::get<std::vector<int8_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::SByte> dataptr = &value[0];
			std::memcpy((int8_t*)dataptr, (int8_t*)std::get<std::vector<int8_t>>(data).data(), sizeof(int8_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 12) {
		assert(std::get<std::vector<uint8_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayUInt8;
		auto value = gcnew array<System::Byte>(static_cast<int>(std::get<std::vector<uint8_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::Byte> dataptr = &value[0];
			std::memcpy((uint8_t*)dataptr, (uint8_t*)std::get<std::vector<uint8_t>>(data).data(), sizeof(uint8_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 13) {
		assert(std::get<std::vector<int16_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayInt16;
		auto value = gcnew array<System::Int16>(static_cast<int>(std::get<std::vector<int16_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::Int16> dataptr = &value[0];
			std::memcpy((int16_t*)dataptr, (int16_t*)std::get<std::vector<int16_t>>(data).data(), sizeof(int16_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 14) {
		assert(std::get<std::vector<uint16_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayUInt16;
		auto value = gcnew array<System::UInt16>(static_cast<int>(std::get<std::vector<uint16_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::UInt16> dataptr = &value[0];
			std::memcpy((uint16_t*)dataptr, (uint16_t*)std::get<std::vector<uint16_t>>(data).data(), sizeof(uint16_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 15) {
		assert(std::get<std::vector<int32_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayInt32;
		auto value = gcnew array<System::Int32>(static_cast<int>(std::get<std::vector<int32_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::Int32> dataptr = &value[0];
			std::memcpy((int32_t*)dataptr, (int32_t*)std::get<std::vector<int32_t>>(data).data(), sizeof(int32_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 16) {
		assert(std::get<std::vector<uint32_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayUInt32;
		auto value = gcnew array<System::UInt32>(static_cast<int>(std::get<std::vector<uint32_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::UInt32> dataptr = &value[0];
			std::memcpy((uint32_t*)dataptr, (uint32_t*)std::get<std::vector<uint32_t>>(data).data(), sizeof(uint32_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 17) {
		assert(std::get<std::vector<int64_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayInt64;
		auto value = gcnew array<System::Int64>(static_cast<int>(std::get<std::vector<int64_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::Int64> dataptr = &value[0];
			std::memcpy((int64_t*)dataptr, (int64_t*)std::get<std::vector<int64_t>>(data).data(), sizeof(int64_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 18) {
		assert(std::get<std::vector<uint64_t>>(data).size() <= static_cast<size_t>(INT_MAX));
		m_type = NikonMtpDatatypeCode::ArrayUInt64;
		auto value = gcnew array<System::UInt64>(static_cast<int>(std::get<std::vector<uint64_t>>(data).size()));
		if (value->Length > 0) {
			pin_ptr<System::UInt64> dataptr = &value[0];
			std::memcpy((uint64_t*)dataptr, (uint64_t*)std::get<std::vector<uint64_t>>(data).data(), sizeof(uint64_t) * value->Length);
		}
		m_value = value;
	}
	else if (data.index() == 19) {
		m_type = NikonMtpDatatypeCode::ArrayInt128;
		throw gcnew System::NotImplementedException();
	}
	else if (data.index() == 20) {
		m_type = NikonMtpDatatypeCode::ArrayUInt128;
		throw gcnew System::NotImplementedException();
	}
	else if (data.index() == 21) {
		m_type = NikonMtpDatatypeCode::String;
		m_value = gcnew System::String(std::get<std::wstring>(data).c_str());
	}
	else {
		m_type = NikonMtpDatatypeCode::Undefined;
		m_value = nullptr;
	}
}