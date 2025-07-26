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
				ssize_t countNikonCameras();

			private:
				bool isNikonCamera(libusb_device* device);

				libusb_context* context_;
		};

	}
}