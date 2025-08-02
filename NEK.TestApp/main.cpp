#include <nikon.hpp>
#include <iostream>


using namespace std;



void test (IPortableDeviceValues* event) {
	int a;
	cout << "event" << std::endl;
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

	auto camera = nek::mtp::MtpDevice((PWSTR)nikonCameras.begin()->first.c_str());
	auto params = nek::mtp::MtpParams();
	params.addUint32(0xFFFFFFFF);
	nek::mtp::MtpResponse result = camera.SendCommand(nek::NikonMtpOperationCode::InitiateCaptureRecInSdram, params);
	camera.RegisterCallback(test);

	while (true) {}

	return 0;
}