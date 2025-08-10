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
MtpDatatypeVariant::MtpDatatypeVariant(System::SByte data) {
	m_type = NikonMtpDatatypeCode::Int8;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::Byte data) {
	m_type = NikonMtpDatatypeCode::UInt8;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::Int16 data) {
	m_type = NikonMtpDatatypeCode::Int16;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::UInt16 data) {
	m_type = NikonMtpDatatypeCode::UInt16;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::Int32 data) {
	m_type = NikonMtpDatatypeCode::Int32;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::UInt32 data) {
	m_type = NikonMtpDatatypeCode::UInt32;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::Int64 data) {
	m_type = NikonMtpDatatypeCode::Int64;
	m_value = data;
}
MtpDatatypeVariant::MtpDatatypeVariant(System::UInt64 data) {
	m_type = NikonMtpDatatypeCode::UInt64;
	m_value = data;
}

MtpDatatypeVariant::MtpDatatypeVariant(array<System::SByte>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayInt8;
	m_value = gcnew array<System::SByte>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::SByte>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::Byte>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayUInt8;
	m_value = gcnew array<System::Byte>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::Byte>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::Int16>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayInt16;
	m_value = gcnew array<System::Int16>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::Int16>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::UInt16>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayUInt16;
	m_value = gcnew array<System::UInt16>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::UInt16>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::Int32>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayInt32;
	m_value = gcnew array<System::Int32>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::Int32>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::UInt32>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayUInt32;
	m_value = gcnew array<System::UInt32>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::UInt32>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::Int64>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayInt64;
	m_value = gcnew array<System::Int64>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::Int64>^)m_value, 0, data->Length);
}
MtpDatatypeVariant::MtpDatatypeVariant(array<System::UInt64>^ data) {
	m_type = NikonMtpDatatypeCode::ArrayUInt64;
	m_value = gcnew array<System::UInt64>(data->Length);
	System::Buffer::BlockCopy(data, 0, (array<System::UInt64>^)m_value, 0, data->Length);
}

MtpDatatypeVariant::MtpDatatypeVariant(System::String^ data) {
	m_type = NikonMtpDatatypeCode::String;
	m_value = data->Clone();
}

MtpDatatypeVariant::MtpDatatypeVariant(const nek::mtp::MtpDatatypeVariant& data) {
	switch (data.index()) {
	case 1:
		SetInt8(std::get<int8_t>(data));
		break;
	case 2:
		SetUInt8(std::get<uint8_t>(data));
		break;
	case 3:
		SetInt16(std::get<int16_t>(data));
		break;
	case 4:
		SetUInt16(std::get<uint16_t>(data));
		break;
	case 5:
		SetInt32(std::get<int32_t>(data));
		break;
	case 6:
		SetUInt32(std::get<uint32_t>(data));
		break;
	case 7:
		SetInt64(std::get<int64_t>(data));
		break;
	case 8:
		SetUInt64(std::get<uint64_t>(data));
		break;
	case 9:
		m_type = NikonMtpDatatypeCode::Int128;
		m_value = nullptr;
		throw gcnew System::NotImplementedException();
		break;
	case 10:
		m_type = NikonMtpDatatypeCode::UInt128;
		m_value = nullptr;
		throw gcnew System::NotImplementedException();
		break;
	case 11:
		SetArrayInt8(std::get<std::vector<int8_t>>(data));
		break;
	case 12:
		SetArrayUInt8(std::get<std::vector<uint8_t>>(data));
		break;
	case 13:
		SetArrayInt16(std::get<std::vector<int16_t>>(data));
		break;
	case 14:
		SetArrayUInt16(std::get<std::vector<uint16_t>>(data));
		break;
	case 15:
		SetArrayInt32(std::get<std::vector<int32_t>>(data));
		break;
	case 16:
		SetArrayUInt32(std::get<std::vector<uint32_t>>(data));
		break;
	case 17:
		SetArrayInt64(std::get<std::vector<int64_t>>(data));
		break;
	case 18:
		SetArrayUInt64(std::get<std::vector<uint64_t>>(data));
		break;
	case 19:
		m_type = NikonMtpDatatypeCode::ArrayInt128;
		m_value = nullptr;
		throw gcnew System::NotImplementedException();
		break;
	case 20:
		m_type = NikonMtpDatatypeCode::ArrayUInt128;
		m_value = nullptr;
		throw gcnew System::NotImplementedException();
		break;
	case 21:
		SetString(std::get<std::wstring>(data));
		break;
	default:
		m_type = NikonMtpDatatypeCode::Undefined;
		m_value = nullptr;
		break;
	}
}

void MtpDatatypeVariant::SetInt8(int8_t data) {
	m_type = NikonMtpDatatypeCode::Int8;
	m_value = data;
}
void MtpDatatypeVariant::SetUInt8(uint8_t data) {
	m_type = NikonMtpDatatypeCode::UInt8;
	m_value = data;
}
void MtpDatatypeVariant::SetInt16(int16_t data) {
	m_type = NikonMtpDatatypeCode::Int16;
	m_value = data;
}
void MtpDatatypeVariant::SetUInt16(uint16_t data) {
	m_type = NikonMtpDatatypeCode::UInt16;
	m_value = data;
}
void MtpDatatypeVariant::SetInt32(int32_t data) {
	m_type = NikonMtpDatatypeCode::Int32;
	m_value = data;
}
void MtpDatatypeVariant::SetUInt32(uint32_t data) {
	m_type = NikonMtpDatatypeCode::UInt32;
	m_value = data;
}
void MtpDatatypeVariant::SetInt64(int64_t data) {
	m_type = NikonMtpDatatypeCode::Int64;
	m_value = data;
}
void MtpDatatypeVariant::SetUInt64(uint64_t data) {
	m_type = NikonMtpDatatypeCode::UInt64;
	m_value = data;
}

void MtpDatatypeVariant::SetArrayInt8(const std::vector<int8_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayInt8;
	auto value = gcnew array<System::SByte>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::SByte> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(int8_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayUInt8(const std::vector<uint8_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayUInt8;
	auto value = gcnew array<System::Byte>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::Byte> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(uint8_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayInt16(const std::vector<int16_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayInt16;
	auto value = gcnew array<System::Int16>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::Int16> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(int16_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayUInt16(const std::vector<uint16_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayUInt16;
	auto value = gcnew array<System::UInt16>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::UInt16> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(uint16_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayInt32(const std::vector<int32_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayInt32;
	auto value = gcnew array<System::Int32>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::Int32> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(int32_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayUInt32(const std::vector<uint32_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayUInt32;
	auto value = gcnew array<System::UInt32>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::UInt32> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(uint32_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayInt64(const std::vector<int64_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayInt64;
	auto value = gcnew array<System::Int64>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::Int64> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(int64_t) * value->Length);
	}
	m_value = value;
}
void MtpDatatypeVariant::SetArrayUInt64(const std::vector<uint64_t>& data) {
	assert(data.size() <= static_cast<size_t>(INT_MAX));
	m_type = NikonMtpDatatypeCode::ArrayUInt64;
	auto value = gcnew array<System::UInt64>(static_cast<int>(data.size()));
	if (value->Length > 0) {
		pin_ptr<System::UInt64> dataptr = &value[0];
		std::memcpy(dataptr, data.data(), sizeof(uint64_t) * value->Length);
	}
	m_value = value;
}

void MtpDatatypeVariant::SetString(const std::wstring& data) {
	m_type = NikonMtpDatatypeCode::String;
	m_value = gcnew System::String(data.c_str());
}

nek::mtp::MtpDatatypeVariant MtpDatatypeVariant::getVariant() {
	if(m_type == NikonMtpDatatypeCode::Int8) {
		auto native = static_cast<int8_t>(safe_cast<System::SByte>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::UInt8) {
		auto native = static_cast<uint8_t>(safe_cast<System::Byte>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::Int16) {
		auto native = static_cast<int16_t>(safe_cast<System::Int16>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::UInt16) {
		auto native = static_cast<uint16_t>(safe_cast<System::UInt16>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::Int32) {
		auto native = static_cast<int32_t>(safe_cast<System::Int32>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::UInt32) {
		auto native = static_cast<uint32_t>(safe_cast<System::UInt32>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::Int64) {
		auto native = static_cast<int64_t>(safe_cast<System::Int64>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::UInt64) {
		auto native = static_cast<uint64_t>(safe_cast<System::UInt64>(m_value));
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::Int128) {
		throw gcnew System::NotImplementedException();
	}
	else if (m_type == NikonMtpDatatypeCode::UInt128) {
		throw gcnew System::NotImplementedException();
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayInt8) {
		auto native = std::vector<int8_t>();
		auto arr = static_cast<array<System::SByte>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::SByte> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(int8_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayUInt8) {
		auto native = std::vector<uint8_t>();
		auto arr = static_cast<array<System::Byte>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::Byte> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(uint8_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayInt16) {
		auto native = std::vector<int16_t>();
		auto arr = static_cast<array<System::Int16>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::Int16> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(int16_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayUInt16) {
		auto native = std::vector<uint16_t>();
		auto arr = static_cast<array<System::UInt16>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::UInt16> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(uint16_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayInt32) {
		auto native = std::vector<int32_t>();
		auto arr = static_cast<array<System::Int32>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::Int32> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(int32_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayUInt32) {
		auto native = std::vector<uint32_t>();
		auto arr = static_cast<array<System::UInt32>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::UInt32> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(uint32_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayInt64) {
		auto native = std::vector<int64_t>();
		auto arr = static_cast<array<System::Int64>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::Int64> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(int64_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::ArrayUInt64) {
		auto native = std::vector<uint64_t>();
		auto arr = static_cast<array<System::UInt64>^>(m_value);
		if (arr->Length > 0) {
			native.resize(arr->Length);
			pin_ptr<System::UInt64> dataptr = &arr[0];
			std::memcpy(native.data(), dataptr, sizeof(uint64_t) * arr->Length);
		}
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	else if (m_type == NikonMtpDatatypeCode::String) {
		auto str = static_cast<System::String^>(m_value)->ToCharArray();
		pin_ptr<wchar_t> prstr = &str[0];
		auto native = std::wstring(prstr);
		nek::mtp::MtpDatatypeVariant v = native;
		return v;
	}
	
	return nek::mtp::MtpDatatypeVariant();
}


System::Boolean MtpDatatypeVariant::TryGetInt8(System::SByte% data) {
	if (m_type == NikonMtpDatatypeCode::Int8) {
		data = safe_cast<System::SByte>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetUInt8(System::Byte% data) {
	if (m_type == NikonMtpDatatypeCode::UInt8) {
		data = safe_cast<System::Byte>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetInt16(System::Int16% data) {
	if (m_type == NikonMtpDatatypeCode::Int16) {
		data = safe_cast<System::Int16>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetUInt16(System::UInt16% data) {
	if (m_type == NikonMtpDatatypeCode::UInt16) {
		data = safe_cast<System::UInt16>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetInt32(System::Int32% data) {
	if (m_type == NikonMtpDatatypeCode::Int32) {
		data = safe_cast<System::Int32>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetUInt32(System::UInt32% data) {
	if (m_type == NikonMtpDatatypeCode::UInt32) {
		data = safe_cast<System::UInt32>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetInt64(System::Int64% data) {
	if (m_type == NikonMtpDatatypeCode::Int64) {
		data = safe_cast<System::Int64>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetUInt64(System::UInt64% data) {
	if (m_type == NikonMtpDatatypeCode::UInt64) {
		data = safe_cast<System::UInt64>(m_value);
		return true;
	}

	return false;
}

System::Boolean MtpDatatypeVariant::TryGetArrayInt8(array<System::SByte>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayInt8) {
		data = static_cast<array<System::SByte>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayUInt8(array<System::Byte>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayUInt8) {
		data = static_cast<array<System::Byte>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayInt16(array<System::Int16>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayInt16) {
		data = static_cast<array<System::Int16>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayUInt16(array<System::UInt16>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayUInt16) {
		data = static_cast<array<System::UInt16>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayInt32(array<System::Int32>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayInt32) {
		data = static_cast<array<System::Int32>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayUInt32t(array<System::UInt32>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayUInt32) {
		data = static_cast<array<System::UInt32>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayInt64(array<System::Int64>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayInt64) {
		data = static_cast<array<System::Int64>^>(m_value);
		return true;
	}

	return false;
}
System::Boolean MtpDatatypeVariant::TryGetArrayUInt64(array<System::UInt64>^% data) {
	if (m_type == NikonMtpDatatypeCode::ArrayUInt64) {
		data = static_cast<array<System::UInt64>^>(m_value);
		return true;
	}

	return false;
}

System::Boolean MtpDatatypeVariant::TryGetString(System::String^% data) {
	if (m_type == NikonMtpDatatypeCode::String) {
		data = static_cast<System::String^>(m_value);
		return true;
	}

	return false;
}