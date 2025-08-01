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

}