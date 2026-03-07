#include "mtp/nek_mtp.hpp"

#include <iostream>
#include <fstream>


using namespace std;


int main() {
	auto mgr = nek::mtp::MtpManager();
	size_t nbNikonCamera = mgr.countAllDevices();
	cout << "Nikon Camera detected: " << nbNikonCamera << endl;
	if (nbNikonCamera == 0) return 0;

	auto nikonCameras = nek::mtp::MtpManager().getAllDevices();
	for (auto& cam : nikonCameras) {
		cam.Connect();
		auto cameraInfo = cam.GetDeviceInfo();
		wcout << cameraInfo.Manufacture << " " << cameraInfo.Model << " " << cameraInfo.SerialNumber << endl;

		bool wait = true;
		while (wait) cin >> wait;

		cam.Disconnect();
	}
	if (nikonCameras.size() == 0) return 0;


	/*auto camT = nek::mtp::MtpDevice(std::make_unique<nek::mtp::backend::wpd::WpdMtpTransport>(nikonCameras.begin()->first));
	
	auto res = camT.GetDeviceInfo();

	res;*/

	return 0;
}