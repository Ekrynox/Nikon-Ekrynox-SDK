#include "nek_usb.hpp"

#include <stdexcept>



DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED);


namespace nek {
	namespace usb {

		int UsbManager::countNikonCameras() {
			return getNikonCameras().size();
		}

		std::vector<std::wstring> UsbManager::getNikonCameras() {
			HDEVINFO devices = SetupDiGetClassDevs(&GUID_DEVINTERFACE_USB_DEVICE, NULL, 0, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
			if (devices == INVALID_HANDLE_VALUE) {
				throw std::runtime_error("Failed to get USB devices list.");
			}

			std::vector<std::wstring> nikonCameras;

			SP_DEVINFO_DATA deviceInfoData = {};
			deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
			for (DWORD i = 0; SetupDiEnumDeviceInfo(devices, i, &deviceInfoData); ++i) {
				TCHAR buffer[1024];
				DWORD buffersize = sizeof(buffer);

				//Is vendor Nikon ?
				if (SetupDiGetDeviceRegistryProperty(devices, &deviceInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)buffer, buffersize, NULL)) {
					std::wstring hwid(buffer);
					if (hwid.find(L"VID_04B0") != std::wstring::npos) {

						//Is device PTP/MTP
						if (SetupDiGetDeviceRegistryProperty(devices, &deviceInfoData, SPDRP_CLASSGUID, NULL, (PBYTE)buffer, buffersize, NULL)) {
							std::wstring guid(buffer);
							if (guid == L"{eec5ad98-8080-425f-922a-dabf3de3f69a}") {
								SP_DEVICE_INTERFACE_DATA interfaceData = {};
								interfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
								SetupDiEnumDeviceInterfaces(devices, &deviceInfoData, &GUID_DEVINTERFACE_USB_DEVICE, 0, &interfaceData);

								DWORD requiredSize = 0;
								SetupDiGetDeviceInterfaceDetail(devices, &interfaceData, NULL, 0, &requiredSize, NULL);

								PSP_DEVICE_INTERFACE_DETAIL_DATA detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(requiredSize);
								detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
								if (SetupDiGetDeviceInterfaceDetail(devices, &interfaceData, detailData, requiredSize, NULL, NULL)) {
									nikonCameras.push_back(detailData->DevicePath);
								}
								free(detailData);
							}
						}
					}
				}
			}

			SetupDiDestroyDeviceInfoList(devices);
			return nikonCameras;
		}

	}
}