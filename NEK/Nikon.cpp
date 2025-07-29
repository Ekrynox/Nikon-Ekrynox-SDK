#include "nikon.hpp"
#include "nek_mtp.hpp"

#include <stdexcept>



int countNikonCameras() {
	return nek::mtp::MtpManager::Instance().countNikonCameras();
}


std::string test() {
	nek::mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto nikonCameras = deviceManager->listNikonCameras();
	if (nikonCameras.empty()) {
		return "";
	}

	auto device = nek::mtp::MtpDevice((PWSTR)nikonCameras[0].c_str());

	nek::mtp::MtpParams params;
	params.addUint32(0x5008);
	//nek::mtp::MtpResponse response = device.SendNoData(0x100E, params);
	nek::mtp::MtpResponse response = device.SendOutData(0x1015, params);
	
	if (response.responseCode == 0x2001) {
		return "Yes";
	}


	return "";
}