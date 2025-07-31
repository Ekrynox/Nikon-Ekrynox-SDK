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
	System::Collections::Generic::List<System::UInt16> OperationsSupported;
	System::Collections::Generic::List<System::UInt16> EventsSupported;
	System::Collections::Generic::List<System::UInt16> DevicePropertiesSupported;
	System::Collections::Generic::List<System::UInt16> CaptureFormats;
	System::Collections::Generic::List<System::UInt16> ImageFormats;
	Manufacture = gcnew System::String(native.Manufacture.c_str());
	Model = gcnew System::String(native.Model.c_str());
	DeviceVersion = gcnew System::String(native.DeviceVersion.c_str());
	SerialNumber = gcnew System::String(native.SerialNumber.c_str());
};
