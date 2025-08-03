#include "pch.h"

#include "Nikon_Struct.h"


using namespace NEKCS;


NikonDeviceInfoDS::NikonDeviceInfoDS() {};

NikonDeviceInfoDS::NikonDeviceInfoDS(const nek::NikonDeviceInfoDS& native) {
	StandardVersion = native.StandardVersion;
	VendorExtensionID = native.VendorExtensionID;
	VendorExtensionVersion = native.VendorExtensionVersion;
	VendorExtensionDesc = gcnew System::String(native.VendorExtensionDesc.c_str());
	FunctionalMode = native.FunctionalMode;

	OperationsSupported = gcnew System::Collections::Generic::List<System::UInt16>();
	for (auto el : native.OperationsSupported) {
		OperationsSupported->Add(el);
	}

	EventsSupported = gcnew System::Collections::Generic::List<System::UInt16>();
	for (auto el : native.EventsSupported) {
		EventsSupported->Add(el);
	}

	DevicePropertiesSupported = gcnew System::Collections::Generic::List<System::UInt16>();
	for (auto el : native.DevicePropertiesSupported) {
		DevicePropertiesSupported->Add(el);
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
