#pragma once
#include "../nek.hpp"
#include "../utils/nek_threading.hpp"
#include "backend/nek_mtp_backend.hpp"
#include "nek_mtp_utils.hpp"
#include "nek_mtp_enum.hpp"
#include "nek_mtp_struct.hpp"
#include "nek_mtp_except.hpp"

#include <atomic>
#include <future>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>



namespace nek::mtp {

	class MtpDevice {
	public:
		NEK_API MtpDevice(std::unique_ptr<backend::IMtpTransport> backend, bool autoConnect = true);
		NEK_API MtpDevice(MtpDevice&& other) noexcept;
		NEK_API ~MtpDevice();

		NEK_API bool isConnected() const;
		NEK_API void Connect();
		NEK_API void Disconnect();

		NEK_API MtpResponse SendCommand(uint16_t operationCode, MtpParams params);
		NEK_API MtpResponse SendCommandAndRead(uint16_t operationCode, MtpParams params);
		NEK_API MtpResponse SendCommandAndWrite(uint16_t operationCode, MtpParams params, std::vector<uint8_t> data);

		NEK_API size_t RegisterCallback(std::function<void(MtpEvent)> callback);
		NEK_API void UnregisterCallback(size_t id);


		NEK_API MtpDeviceInfoDS GetDeviceInfo();

		NEK_API MtpObjectInfoDS GetObjectInfo(uint32_t handle);

		NEK_API MtpDevicePropDescDSV GetDevicePropDesc(uint16_t devicePropCode);
		NEK_API MtpDatatypeVariant GetDevicePropValue(uint16_t devicePropCode);
		NEK_API void SetDevicePropValue(uint16_t devicePropCode, MtpDatatypeVariant data);
		NEK_API void SetDevicePropValueTypesafe(uint16_t devicePropCode, MtpDatatypeVariant data);


	protected:
		std::unique_ptr<backend::IMtpTransport> backend_;

		MtpDeviceInfoDS deviceInfo_;
		std::map<uint32_t, uint16_t> devicePropDataType_;

		CComPtr<MtpEventCallback> eventCallback_;
		std::mutex mutexDeviceInfo_;


		MtpDevicePropDescDSV GetDevicePropDesc_(MtpResponse& response);
		MtpDatatypeVariant GetDevicePropValue_(MtpResponse& response, uint16_t dataType);
		std::vector<uint8_t> SetDevicePropValue_(MtpDatatypeVariant data);
		bool SetDevicePropValueTypesafe_(const uint16_t dataType, const MtpDatatypeVariant& data, MtpDatatypeVariant& newdata);
	};



	class MtpManager {
	public:
		NEK_API MtpManager();
		NEK_API void registerBackend(std::unique_ptr<backend::IMtpBackendProvider> backend);

		NEK_API std::vector<backend::MtpConnectionInfo> listAllDevices();
		NEK_API std::vector<MtpDevice> getAllDevices();
		NEK_API size_t countAllDevices();

	private:
		std::vector<std::unique_ptr<backend::IMtpBackendProvider>> backends_;
	};

}