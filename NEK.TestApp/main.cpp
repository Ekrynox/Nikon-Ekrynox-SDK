#include <nikon.hpp>
#include <iostream>
#include <fstream>


using namespace std;


static nek::NikonCamera *camera;


void eventFunc (nek::mtp::MtpEvent event) {
	cout << "event: " << std::hex << event.eventCode << " [ ";
	for (uint32_t p : event.eventParams) {
		cout << std::hex << p << " ";
	}
	cout << "]" << std::endl;

	if (event.eventCode == nek::NikonMtpEventCode::ObjectAddedInSdram) {
		auto params = nek::mtp::MtpParams();
		if (event.eventParams.size() > 0 && event.eventParams[0] != 0) {
			params.addUint32(event.eventParams[0]);
		}
		else {
			params.addUint32(0xFFFF0001);
		}
		//nek::mtp::MtpResponse result = camera->SendCommandAndRead(nek::NikonMtpOperationCode::GetObjectInfo, params);
		//result = camera->SendCommandAndRead(nek::NikonMtpOperationCode::GetThumb, params);
		nek::mtp::MtpResponse result = camera->SendCommandAndRead(nek::NikonMtpOperationCode::GetObject, params);
		if (result.responseCode == nek::NikonMtpResponseCode::OK) {
			ofstream MyFile("image.NEF", ios::out | ios::binary);
			MyFile.write((char*)result.data.data(), result.data.size());
			MyFile.close();
		}
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

	camera = new nek::NikonCamera(nikonCameras.begin()->first);
	camera->RegisterCallback(eventFunc);


	int wait = 1;
	while (true) {
		cin >> wait;
		if (wait == 0) break;

		auto params = nek::mtp::MtpParams();
		//params.addUint32(0xFFFFFFFF);
		nek::mtp::MtpResponse result = camera->SendCommand(nek::NikonMtpOperationCode::GetDeviceInfo, params);
	}

	delete camera;

	return 0;
}