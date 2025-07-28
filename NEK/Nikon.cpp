#include "nikon.hpp"
#include "nek_mtp.hpp"

#include <stdexcept>



int countNikonCameras() {
	return nek::mtp::MtpDevice::Instance().countNikonCameras();
}


std::string test() {
	return "";
}