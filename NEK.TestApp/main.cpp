#include <nikon.hpp>
#include <iostream>


int main() {
	std::cout << "Nikon devices found: " << countUsbDevices() << std::endl;

	return 0;
}