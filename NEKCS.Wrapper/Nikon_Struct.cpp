#include "pch.h"

#include "Nikon_Struct.h"


using namespace NEKCS;


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
