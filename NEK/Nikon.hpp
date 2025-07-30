#pragma once
#include "nek.hpp"
#include "nek_mtp.hpp"
#include "nikon_enum.hpp"
#include "nikon_struct.hpp"

#include <string>
#include <vector>



namespace nek {

	class NEK_API NikonCamera {
	public:
		static std::map<std::wstring, NikonDeviceInfoDS> listNikonCameras();
		static size_t countNikonCameras();

		NikonCamera(std::wstring devicePath);
		
	private:
		nek::mtp::MtpDevice camera_;
	};

}