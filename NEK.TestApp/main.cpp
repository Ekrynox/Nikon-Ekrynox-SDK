#include <nikon.hpp>
#include <iostream>


using namespace std;


int main() {
	size_t nbNikonCamera = nek::NikonCamera::countNikonCameras();
	cout << "Nikon Camera detected: " << nbNikonCamera << endl;
	if (nbNikonCamera == 0) return 0;

	auto nikonCameras = nek::NikonCamera::listNikonCameras();
	auto camera = nek::NikonCamera(nikonCameras[0]);

	return 0;
}