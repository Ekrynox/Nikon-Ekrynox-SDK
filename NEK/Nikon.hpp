#pragma once
#include "nek.hpp"
#include "mtp/nek_mtp.hpp"
#include "nikon_enum.hpp"

#include <string>
#include <vector>



namespace nek {

	class NEK_API NikonCamera : public nek::mtp::MtpDevice {
	public:
		static std::map<std::wstring, mtp::MtpDeviceInfoDS> listNikonCameras(bool onlyOn = true);
		static size_t countNikonCameras(bool onlyOn = true);

		NikonCamera(std::wstring devicePath, uint8_t additionalThread = 0);


		mtp::MtpDevicePropDescDS GetDevicePropDesc(uint32_t devicePropCode);
		mtp::MtpDatatypeVariant GetDevicePropValue(uint32_t devicePropCode);
		void SetDevicePropValue(uint32_t devicePropCode, mtp::MtpDatatypeVariant data);


	private:
		virtual void mainThreadTask() override;
		virtual void threadTask() override;
		void eventThreadTask();

		virtual void startThreads() override;

		std::deque<std::function<void()>> tasksEvent_;
	};

}