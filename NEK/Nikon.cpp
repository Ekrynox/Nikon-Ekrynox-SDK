#include "nikon.hpp"

#include <stdexcept>



using namespace nek;



std::map<std::wstring, NikonDeviceInfoDS> NikonCamera::listNikonCameras() {
	mtp::MtpManager* deviceManager = &nek::mtp::MtpManager::Instance();
	auto cameras = deviceManager->listMtpDevices();
	std::map<std::wstring, NikonDeviceInfoDS> nikonCameras;

	for (auto &camera : cameras) {
		//Check if Nikon
		std::wstring id(camera.first);
		std::transform(id.begin(), id.end(), id.begin(), ::towlower);
		if (id.find(L"vid_04b0") != std::wstring::npos) {
			nikonCameras.insert(camera);
		}
	}

	return nikonCameras;
}

size_t NikonCamera::countNikonCameras() {
	return listNikonCameras().size();
}


NikonCamera::NikonCamera(std::wstring devicePath) : nek::mtp::MtpDevice((PWSTR)devicePath.c_str()) {}