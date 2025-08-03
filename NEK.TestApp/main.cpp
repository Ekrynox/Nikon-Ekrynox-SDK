#include <nikon.hpp>
#include <iostream>


using namespace std;



void eventFunc (nek::mtp::MtpEvent event) {
	cout << "event: " << std::hex << event.eventCode << std::endl;
	for (uint32_t p : event.params) {
		cout << "\t: " << std::hex << p << std::endl;
	}
	return;
}


int main() {
	size_t nbNikonCamera = nek::NikonCamera::countNikonCameras();
	cout << "Nikon Camera detected: " << nbNikonCamera << endl;
	if (nbNikonCamera == 0) return 0;

	auto nikonCameras = nek::NikonCamera::listNikonCameras();
	for (auto& cameraInfo : nikonCameras) {
		wcout << cameraInfo.second.Manufacture << " " << cameraInfo.second.Model << " " << cameraInfo.second.SerialNumber << endl;
	}

	auto camera = nek::NikonCamera(nikonCameras.begin()->first);
	camera.RegisterCallback(eventFunc);


	int wait = 1;
	while (wait > 0) {
		cin >> wait;
		auto params = nek::mtp::MtpParams();
		params.addUint32(0xFFFFFFFF);
		nek::mtp::MtpResponse result = camera.SendCommand(nek::NikonMtpOperationCode::InitiateCaptureRecInSdram, params);
	} 

	return 0;
}