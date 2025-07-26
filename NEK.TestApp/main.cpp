#include <nikon.hpp>
#include <iostream>


int main() {
	std::cout << "USB devices found: " << countUsbDevices() << std::endl;
	std::cout << "Nikon cameras found: " << countNikonCameras() << std::endl;

	return 0;
}