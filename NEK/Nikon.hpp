#pragma once
#include "nek.hpp"
#include "mtp/nek_mtp.hpp"
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

		nek::mtp::MtpResponse* SendCommand(WORD operationCode, nek::mtp::MtpParams& params) { return camera_.SendCommand(operationCode, params); };
		nek::mtp::MtpResponse* SendCommandAndRead(WORD operationCode, nek::mtp::MtpParams& params) { return camera_.SendCommandAndRead(operationCode, params); };
		nek::mtp::MtpResponse* SendCommandAndWrite(WORD operationCode, nek::mtp::MtpParams& params, std::vector<BYTE> data) { return camera_.SendCommandAndWrite(operationCode, params, data); };
		
	private:
		nek::mtp::MtpDevice camera_;
	};

}