#pragma once
#include "nek.hpp"
#include "mtp/nek_mtp.hpp"
#include "nikon_enum.hpp"
#include "nikon_struct.hpp"

#include <string>
#include <vector>



namespace nek {

	class NEK_API NikonCamera : public nek::mtp::MtpDevice {
	public:
		static std::map<std::wstring, NikonDeviceInfoDS> listNikonCameras();
		static size_t countNikonCameras();

		NikonCamera(std::wstring devicePath);
	};

}