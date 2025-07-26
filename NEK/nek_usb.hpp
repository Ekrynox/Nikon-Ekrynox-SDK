#pragma once
#include "nek.hpp"
#include <libusb-1.0/libusb.h>


namespace nek {
	namespace usb {

		class UsbManager {
			public:
				UsbManager();
				~UsbManager();
				ssize_t countDevices();

			private:
				libusb_context* context_;
		};

	}
}