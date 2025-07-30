#pragma once

#include <stdint.h>



namespace nek {

	struct DeviceInfoDataSet {
		uint16_t StandardVersion;
		uint32_t VendorExtensionID;
		uint16_t VendorExtensionVersion;

		uint8_t VendorExtensionDescLength;
		char16_t VendorExtensionDesc;

		uint16_t FunctionalMode;

		uint32_t OperationsSupportedLength;
		uint16_t OperationsSupported;

		uint32_t EventsSupportedLength;
		uint16_t EventsSupported;

		uint32_t DevicePropertiesSupportedLength;
		uint16_t DevicePropertiesSupported;

		uint32_t CaptureFormatsLength;
		uint16_t CaptureFormats;

		uint32_t ImageFormatsLength;
		uint16_t ImageFormats;

		uint8_t ManufactureLength;
		char16_t Manufacture;

		uint8_t ModelLength;
		char16_t Model;

		uint8_t DeviceVersionLength;
		char16_t DeviceVersion;

		uint8_t SerialNumberLength;
		char16_t SerialNumber;
	};

}