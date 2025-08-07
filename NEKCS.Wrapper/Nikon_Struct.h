#pragma once
#include "nikon_struct.hpp"
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
		NikonDeviceInfoDS(const nek::NikonDeviceInfoDS& native);
	};

}