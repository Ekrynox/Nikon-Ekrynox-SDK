#include "nikon.hpp"
#include "nek_mtp.hpp"

#include <stdexcept>



using namespace nek;



std::vector <mtp::MtpDeviceInfo> NikonCamera::listNikonCameras() {
	mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto cameras = deviceManager->listMtpDevices();
	std::vector<mtp::MtpDeviceInfo> nikonCameras;

	for (auto& camera : cameras) {
		//Check if Nikon
		std::wstring id(camera.devicePath);
		std::transform(id.begin(), id.end(), id.begin(), ::towlower);
		if (id.find(L"vid_04b0") != std::wstring::npos) {
			nikonCameras.push_back(camera);
		}
	}

	return nikonCameras;
}

size_t NikonCamera::countNikonCameras() {
	return listNikonCameras().size();
}


NikonCamera::NikonCamera(std::wstring devicePath) : camera_((PWSTR)devicePath.c_str()) {}

NikonCamera::NikonCamera(nek::mtp::MtpDeviceInfo deviceInfo) : NikonCamera(deviceInfo.devicePath) {};