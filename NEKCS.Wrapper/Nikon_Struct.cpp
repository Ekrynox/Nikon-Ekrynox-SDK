#include "pch.h"

#include "Nikon_Struct.h"

#include <assert.h>


using namespace System::Runtime::InteropServices;


using namespace NEKCS;


//NikonDeviceInfoDS
NikonDeviceInfoDS::NikonDeviceInfoDS() {
	_standardVersion = 0;
	_vendorExtensionID = 0;
	_vendorExtensionVersion = 0;
	_vendorExtensionDesc = gcnew System::String("");
	_functionalMode = 0;
	_operationsSupported = gcnew System::Collections::Generic::List<NikonMtpOperationCode>(0);
	_eventsSupported = gcnew System::Collections::Generic::List<NikonMtpEventCode>(0);
	_devicePropertiesSupported = gcnew System::Collections::Generic::List<NikonMtpDevicePropCode>(0);
	_captureFormats = gcnew System::Collections::Generic::List<System::UInt16>(0);
	_imageFormats = gcnew System::Collections::Generic::List<System::UInt16>(0);
	_manufacture = gcnew System::String("");
	_model = gcnew System::String("");
	_deviceVersion = gcnew System::String("");
	_serialNumber = gcnew System::String("");
};

NikonDeviceInfoDS::NikonDeviceInfoDS(const nek::NikonDeviceInfoDS& native) {
	_standardVersion = native.StandardVersion;
	_vendorExtensionID = native.VendorExtensionID;
	_vendorExtensionVersion = native.VendorExtensionVersion;
	_vendorExtensionDesc = gcnew System::String(native.VendorExtensionDesc.c_str());
	_functionalMode = native.FunctionalMode;

	_operationsSupported = gcnew System::Collections::Generic::List<NikonMtpOperationCode>(0);
	for (auto el : native.OperationsSupported) {
		_operationsSupported->Add((NikonMtpOperationCode)el);
	}
	_eventsSupported = gcnew System::Collections::Generic::List<NikonMtpEventCode>(0);
	for (auto el : native.EventsSupported) {
		_eventsSupported->Add((NikonMtpEventCode)el);
	}
	_devicePropertiesSupported = gcnew System::Collections::Generic::List<NikonMtpDevicePropCode>(0);
	for (auto el : native.DevicePropertiesSupported) {
		_devicePropertiesSupported->Add((NikonMtpDevicePropCode)el);
	}
	_captureFormats = gcnew System::Collections::Generic::List<System::UInt16>(0);
	for (auto el : native.CaptureFormats) {
		_captureFormats->Add(el);
	}
	_imageFormats = gcnew System::Collections::Generic::List<System::UInt16>(0);
	for (auto el : native.ImageFormats) {
		_imageFormats->Add(el);
	}

	_manufacture = gcnew System::String(native.Manufacture.c_str());
	_model = gcnew System::String(native.Model.c_str());
	_deviceVersion = gcnew System::String(native.DeviceVersion.c_str());
	_serialNumber = gcnew System::String(native.SerialNumber.c_str());
};



//NikonObjectInfoDS
NikonObjectInfoDS::NikonObjectInfoDS() {
	StorageID = 0;
	ObjectFormat = (NikonMtpObjectFormatCode)0;
	ProtectionStatus = 0;
	ObjectCompressedSize = 0;
	ThumbFormat = (NikonMtpObjectFormatCode)0;
	ThumbCompressedSize = 0;
	ThumbPixWidth = 0;
	ThumbPixHeight = 0;
	ImagePixWidth = 0;
	ImagePixHeight = 0;
	ImageBitDepth = 0;
	ParentObject = 0;
	AssociationType = 0;
	AssociationDesc = 0;
	SequenceNumber = 0;
	Filename = "";
	CaptureDate = "";
	ModificationDate = "";
	Keywords = 0;
};

NikonObjectInfoDS::NikonObjectInfoDS(const nek::mtp::MtpObjectInfoDS& native) {
	StorageID = native.StorageID;
	ObjectFormat = (NikonMtpObjectFormatCode)native.ObjectFormat;
	ProtectionStatus = native.ProtectionStatus;
	ObjectCompressedSize = native.ObjectCompressedSize;
	ThumbFormat = (NikonMtpObjectFormatCode)native.ThumbFormat;
	ThumbCompressedSize = native.ThumbCompressedSize;
	ThumbPixWidth = native.ThumbPixWidth;
	ThumbPixHeight = native.ThumbPixHeight;
	ImagePixWidth = native.ImagePixWidth;
	ImagePixHeight = native.ImagePixHeight;
	ImageBitDepth = native.ImageBitDepth;
	ParentObject = native.ParentObject;
	AssociationType = native.AssociationType;
	AssociationDesc = native.AssociationDesc;
	SequenceNumber = native.SequenceNumber;
	Filename = gcnew System::String(native.Filename.c_str());
	CaptureDate = gcnew System::String(native.CaptureDate.c_str());
	ModificationDate = gcnew System::String(native.ModificationDate.c_str());
	Keywords = native.Keywords;
};



//NikonDevicePropDescDS_Variant
NikonDevicePropDescDS_Variant::NikonDevicePropDescDS_Variant(const nek::mtp::MtpDevicePropDescDSV& desc) {
	DevicePropertyCode = (NikonMtpDevicePropCode)desc.DevicePropertyCode;
	DataType = (NikonMtpDatatypeCode)desc.DataType;
	GetSet = desc.GetSet;

	FactoryDefaultValue = gcnew MtpDatatypeVariant(desc.FactoryDefaultValue);
	CurrentValue = gcnew MtpDatatypeVariant(desc.CurrentValue);
	FormFlag = NikonMtpFormtypeCode(desc.FormFlag);

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		auto form = std::get<nek::mtp::MtpRangeFormV>(desc.FORM);
		RangeFORM = gcnew NikonMtpRangeForm<MtpDatatypeVariant^>();
		RangeFORM->min = gcnew MtpDatatypeVariant(form.min);
		RangeFORM->max = gcnew MtpDatatypeVariant(form.max);
		RangeFORM->step = gcnew MtpDatatypeVariant(form.step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		auto form = std::get<nek::mtp::MtpEnumFormV>(desc.FORM);
		assert(form.size() <= static_cast<int>(INT_MAX));
		EnumFORM = gcnew array<MtpDatatypeVariant^>(static_cast<int>(form.size()));
		for (int i = 0; i < form.size(); i++) {
			EnumFORM[i] = gcnew MtpDatatypeVariant(form[i]);
		}
	}
}

template<typename T> System::Boolean NikonDevicePropDescDS_Variant::TryGet([Out] NikonDevicePropDescDS<T>^% desc) {
	desc = gcnew NikonDevicePropDescDS<T>();
	if (DataType != MtpDatatypeVariant::GetCodeFromType(T::typeid)) {
		return false;
	}
	System::Boolean res = true;

	desc->DevicePropertyCode = DevicePropertyCode;
	desc->DataType = DataType;
	desc->GetSet = GetSet;

	res &= FactoryDefaultValue->TryGet(desc->FactoryDefaultValue);
	res &= CurrentValue->TryGet(desc->CurrentValue);
	desc->FormFlag = FormFlag;

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		desc->RangeFORM = gcnew NikonMtpRangeForm<T>();
		res &= RangeFORM->min->TryGet(desc->RangeFORM->min);
		res &= RangeFORM->max->TryGet(desc->RangeFORM->max);
		res &= RangeFORM->step->TryGet(desc->RangeFORM->step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		desc->EnumFORM = gcnew array<T>(EnumFORM->Length);
		for (int i = 0; i < EnumFORM->Length; i++) {
			res &= EnumFORM[i]->TryGet(desc->EnumFORM[i]);
		}
	}

	return res;
}

System::Boolean NikonDevicePropDescDS_Variant::TryGetInteger([Out] NikonDevicePropDescDS<System::Int64>^% desc) {
	desc = gcnew NikonDevicePropDescDS<System::Int64>();
	if ((DataType != NikonMtpDatatypeCode::Int8) && (DataType != NikonMtpDatatypeCode::Int16) && (DataType != NikonMtpDatatypeCode::Int32) && (DataType != NikonMtpDatatypeCode::Int64)) {
		return false;
	}
	System::Boolean res = true;

	desc->DevicePropertyCode = DevicePropertyCode;
	desc->DataType = DataType;
	desc->GetSet = GetSet;

	res &= FactoryDefaultValue->TryGetInteger(desc->FactoryDefaultValue);
	res &= CurrentValue->TryGetInteger(desc->CurrentValue);
	desc->FormFlag = FormFlag;

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		desc->RangeFORM = gcnew NikonMtpRangeForm<System::Int64>();
		res &= RangeFORM->min->TryGetInteger(desc->RangeFORM->min);
		res &= RangeFORM->max->TryGetInteger(desc->RangeFORM->max);
		res &= RangeFORM->step->TryGetInteger(desc->RangeFORM->step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		desc->EnumFORM = gcnew array<System::Int64>(EnumFORM->Length);
		for (int i = 0; i < EnumFORM->Length; i++) {
			res &= EnumFORM[i]->TryGetInteger(desc->EnumFORM[i]);
		}
	}

	return res;
}
System::Boolean NikonDevicePropDescDS_Variant::TryGetUInteger([Out] NikonDevicePropDescDS<System::UInt64>^% desc) {
	desc = gcnew NikonDevicePropDescDS<System::UInt64>();
	if ((DataType != NikonMtpDatatypeCode::UInt8) && (DataType != NikonMtpDatatypeCode::UInt16) && (DataType != NikonMtpDatatypeCode::UInt32) && (DataType != NikonMtpDatatypeCode::UInt64)) {
		return false;
	}
	System::Boolean res = true;

	desc->DevicePropertyCode = DevicePropertyCode;
	desc->DataType = DataType;
	desc->GetSet = GetSet;

	res &= FactoryDefaultValue->TryGetUInteger(desc->FactoryDefaultValue);
	res &= CurrentValue->TryGetUInteger(desc->CurrentValue);
	desc->FormFlag = FormFlag;

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		desc->RangeFORM = gcnew NikonMtpRangeForm<System::UInt64>();
		res &= RangeFORM->min->TryGetUInteger(desc->RangeFORM->min);
		res &= RangeFORM->max->TryGetUInteger(desc->RangeFORM->max);
		res &= RangeFORM->step->TryGetUInteger(desc->RangeFORM->step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		desc->EnumFORM = gcnew array<System::UInt64>(EnumFORM->Length);
		for (int i = 0; i < EnumFORM->Length; i++) {
			res &= EnumFORM[i]->TryGetUInteger(desc->EnumFORM[i]);
		}
	}

	return res;
}
System::Boolean NikonDevicePropDescDS_Variant::TryGetArrayInteger([Out] NikonDevicePropDescDS<array<System::Int64>^>^% desc) {
	desc = gcnew NikonDevicePropDescDS<array<System::Int64>^>();
	if ((DataType != NikonMtpDatatypeCode::ArrayInt8) && (DataType != NikonMtpDatatypeCode::ArrayInt16) && (DataType != NikonMtpDatatypeCode::ArrayInt32) && (DataType != NikonMtpDatatypeCode::ArrayInt64)) {
		return false;
	}
	System::Boolean res = true;

	desc->DevicePropertyCode = DevicePropertyCode;
	desc->DataType = DataType;
	desc->GetSet = GetSet;

	res &= FactoryDefaultValue->TryGetArrayInteger(desc->FactoryDefaultValue);
	res &= CurrentValue->TryGetArrayInteger(desc->CurrentValue);
	desc->FormFlag = FormFlag;

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		desc->RangeFORM = gcnew NikonMtpRangeForm<array<System::Int64>^>();
		res &= RangeFORM->min->TryGetArrayInteger(desc->RangeFORM->min);
		res &= RangeFORM->max->TryGetArrayInteger(desc->RangeFORM->max);
		res &= RangeFORM->step->TryGetArrayInteger(desc->RangeFORM->step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		desc->EnumFORM = gcnew array<array<System::Int64>^>(EnumFORM->Length);
		for (int i = 0; i < EnumFORM->Length; i++) {
			res &= EnumFORM[i]->TryGetArrayInteger(desc->EnumFORM[i]);
		}
	}

	return res;
}
System::Boolean NikonDevicePropDescDS_Variant::TryGetArrayUInteger([Out] NikonDevicePropDescDS<array<System::UInt64>^>^% desc) {
	desc = gcnew NikonDevicePropDescDS<array<System::UInt64>^>();
	if ((DataType != NikonMtpDatatypeCode::ArrayUInt8) && (DataType != NikonMtpDatatypeCode::ArrayUInt16) && (DataType != NikonMtpDatatypeCode::ArrayUInt32) && (DataType != NikonMtpDatatypeCode::ArrayUInt64)) {
		return false;
	}
	System::Boolean res = true;

	desc->DevicePropertyCode = DevicePropertyCode;
	desc->DataType = DataType;
	desc->GetSet = GetSet;

	res &= FactoryDefaultValue->TryGetArrayUInteger(desc->FactoryDefaultValue);
	res &= CurrentValue->TryGetArrayUInteger(desc->CurrentValue);
	desc->FormFlag = FormFlag;

	if (FormFlag == NikonMtpFormtypeCode::Range) {
		desc->RangeFORM = gcnew NikonMtpRangeForm<array<System::UInt64>^>();
		res &= RangeFORM->min->TryGetArrayUInteger(desc->RangeFORM->min);
		res &= RangeFORM->max->TryGetArrayUInteger(desc->RangeFORM->max);
		res &= RangeFORM->step->TryGetArrayUInteger(desc->RangeFORM->step);
	}
	else if (FormFlag == NikonMtpFormtypeCode::Enum) {
		desc->EnumFORM = gcnew array<array<System::UInt64>^>(EnumFORM->Length);
		for (int i = 0; i < EnumFORM->Length; i++) {
			res &= EnumFORM[i]->TryGetArrayUInteger(desc->EnumFORM[i]);
		}
	}

	return res;
}