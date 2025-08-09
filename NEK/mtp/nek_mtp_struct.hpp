#pragma once

#include <stdint.h>
#include <string>
#include <vector>



namespace nek::mtp {

	struct MtpDeviceInfoDS_ {
		uint16_t StandardVersion = 0;
		uint32_t VendorExtensionID = 0;
		uint16_t VendorExtensionVersion = 0;

		//uint8_t VendorExtensionDescLength;
		std::wstring VendorExtensionDesc; //char16_t

		uint16_t FunctionalMode = 0;

		//uint32_t OperationsSupportedLength;
		std::vector<uint16_t> OperationsSupported;

		//uint32_t EventsSupportedLength;
		std::vector<uint16_t> EventsSupported;

		//uint32_t DevicePropertiesSupportedLength;
		std::vector<uint16_t> DevicePropertiesSupported;

		//uint32_t CaptureFormatsLength;
		std::vector<uint16_t> CaptureFormats;

		//uint32_t ImageFormatsLength;
		std::vector<uint16_t> ImageFormats;

		//uint8_t ManufactureLength;
		std::wstring Manufacture; //char16_t

		//uint8_t ModelLength;
		std::wstring Model; //char16_t

		//uint8_t DeviceVersionLength;
		std::wstring DeviceVersion; //char16_t

		//uint8_t SerialNumberLength;
		std::wstring SerialNumber; //char16_t
	};
	typedef struct MtpDeviceInfoDS_ MtpDeviceInfoDS;



	//TOIMPROVE
	struct MtpDevicePropDescDS_ {
		uint32_t DevicePropertyCode = 0; //uint16_t for the GetDevicePropDesc operation //uint32_t for the Nikon GetDevicePropDescEx operation
		uint16_t DataType = 0;
		uint8_t GetSet = 0; //0x00 read-only, 0x01 read-write
		std::vector<uint8_t> FactoryDefaultValue; //See DataType for the type of data stored here
		std::vector<uint8_t> CurrentValue; //See DataType for the type of data stored here
		uint8_t FormFlag = 0; //0x00 normal, 0x01 range, 0x02 enum
		std::vector<uint8_t> FORM; //See DataType and FormFlag for the type of data stored here
	};

}