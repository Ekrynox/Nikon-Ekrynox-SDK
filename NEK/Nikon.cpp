#include "nikon.hpp"
#include "nek_mtp.hpp"

#include <stdexcept>



int countNikonCameras() {
	return nek::mtp::MtpManager::Instance().countNikonCameras();
}


std::string test() {
	return "";
}