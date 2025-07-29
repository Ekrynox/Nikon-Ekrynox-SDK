#pragma once
#include "nek.hpp"
#include "nek_mtp.hpp"

#include <string>
#include <vector>



namespace nek {

	class NikonCamera {
	public:
		NEK_API static std::vector<nek::mtp::MtpDeviceInfo> listNikonCameras();
		NEK_API static size_t countNikonCameras();

		NEK_API NikonCamera(std::wstring devicePath);
		NEK_API NikonCamera(nek::mtp::MtpDeviceInfo deviceInfo);
		
	private:
		nek::mtp::MtpDevice camera_;
	};

}