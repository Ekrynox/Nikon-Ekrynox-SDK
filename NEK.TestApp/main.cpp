#include <nikon.hpp>
#include <iostream>


int main() {
	std::cout << "Nikon cameras found: " << countNikonCameras() << std::endl;

	std::cout << test() << std::endl;
	return 0;
}