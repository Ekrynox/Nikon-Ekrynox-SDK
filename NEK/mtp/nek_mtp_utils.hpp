#pragma once
#include "../nek.hpp"

#include <future>
#include <vector>

#define NOMINMAX
#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
#include <WpdMtpExtensions.h>
#include <functional>



namespace nek::mtp {

	struct MtpResponse_ {
		uint16_t responseCode;
		std::vector<uint32_t> parameters;
		std::vector<uint8_t> data;
	};
	typedef struct MtpResponse_ MtpResponse;


	class MtpEvent {
	public:
		NEK_API MtpEvent(uint16_t eventCode);
		NEK_API MtpEvent(uint16_t eventCode, uint32_t param);
		NEK_API MtpEvent(uint16_t eventCode, std::vector<uint32_t> params);

		uint16_t eventCode;
		std::vector<uint32_t> eventParams;
	};


	class MtpEventCallback : public IPortableDeviceEventCallback {
	public:
		MtpEventCallback();
		~MtpEventCallback();

		HRESULT STDMETHODCALLTYPE OnEvent(IPortableDeviceValues* pEventParameters);
		HRESULT STDMETHODCALLTYPE OnEvent(MtpEvent event);
		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
		ULONG STDMETHODCALLTYPE AddRef();
		ULONG STDMETHODCALLTYPE Release();

		size_t RegisterCallback(std::function<void(MtpEvent)> callback);
		void UnregisterCallback(size_t id);

	private:
		ULONG ref_;

		std::mutex mutex_;
		size_t nextId;
		std::vector<std::pair<size_t, std::function<void(MtpEvent)>>>* callbacks_;
	};

}