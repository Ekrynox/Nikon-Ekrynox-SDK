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
	}
}