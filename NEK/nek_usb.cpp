#include "nek_usb.hpp"
#include <libusb-1.0/libusb.h>
#include <system_error>



namespace nek {
	namespace usb {
		UsbManager::UsbManager() {
			if (libusb_init_context(&context_, nullptr, 0) > 0) {
				throw std::runtime_error("Failed to initialize libusb context");
			}
		}

		UsbManager::~UsbManager() {
			if (context_) {
				libusb_exit(context_);
			}
		}


		ssize_t UsbManager::countDevices() {
			libusb_device **devices;

			ssize_t count = libusb_get_device_list(context_, &devices);
			if (count < 0) {
				throw std::system_error(static_cast<int>(count), std::generic_category(), "Failed to get libusb device list");
			}

			libusb_free_device_list(devices, 1);

			return count;
		}

		ssize_t UsbManager::countNikonCameras() {
			libusb_device** devices;

			ssize_t count = libusb_get_device_list(context_, &devices);
			if (count < 0) {
				throw std::system_error(static_cast<int>(count), std::generic_category(), "Failed to get libusb device list");
			}

			ssize_t countNikon = 0;
			for (ssize_t i = 0; i < count; ++i) {
				if (isNikonCamera(devices[i])) {
					++countNikon;
				}
			}

			libusb_free_device_list(devices, 1);

			return countNikon;
		}


		bool UsbManager::isNikonCamera(libusb_device* device) {
			libusb_device_descriptor desc;

			int res = libusb_get_device_descriptor(device, &desc);
			if (res != 0) {
				throw std::system_error(static_cast<int>(res), std::generic_category(), "Failed to get libusb device descriptor");
			}

			// Nikon vendor ID
			if (desc.idVendor != 0x04b0) {
				return false;
			}

			return true;
		}
	}
}