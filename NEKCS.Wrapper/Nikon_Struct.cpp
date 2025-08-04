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

	for (auto el : native.OperationsSupported) {
		OperationsSupported.Add(el);
	}
	for (auto el : native.EventsSupported) {
		EventsSupported.Add(el);
	}
	for (auto el : native.DevicePropertiesSupported) {
		DevicePropertiesSupported.Add(el);
	}
	for (auto el : native.CaptureFormats) {
		CaptureFormats.Add(el);
	}
	for (auto el : native.ImageFormats) {
		ImageFormats.Add(el);
	}

	Manufacture = gcnew System::String(native.Manufacture.c_str());
	Model = gcnew System::String(native.Model.c_str());
	DeviceVersion = gcnew System::String(native.DeviceVersion.c_str());
	SerialNumber = gcnew System::String(native.SerialNumber.c_str());
};
