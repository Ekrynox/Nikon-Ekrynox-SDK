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
		static std::map<std::wstring, NikonDeviceInfoDS> listNikonCameras(bool onlyOn = true);
		static size_t countNikonCameras(bool onlyOn = true);

		NikonCamera(std::wstring devicePath, byte additionalThread = 0);

	private:
		virtual void mainThreadTask() override;
		virtual void threadTask() override;
		void eventThreadTask();

		std::queue<std::function<void()>> tasksEvent_;
	};

}