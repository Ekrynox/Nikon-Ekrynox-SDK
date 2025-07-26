#include "nikon.hpp"
#include "nek_usb.hpp"


int countUsbDevices() {
	nek::usb::UsbManager usbManager;
	return usbManager.countDevices();
}