#pragma once
#include "../nek.hpp"

#include <future>
#include <vector>

#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
#include <WpdMtpExtensions.h>
#include <functional>



namespace nek::mtp {

	class NEK_API MtpReponseParams {
	public:
		MtpReponseParams();
		MtpReponseParams(CComPtr<IPortableDevicePropVariantCollection> paramsCollection);
		MtpReponseParams(CComPtr<IPortableDeviceValues> eventParameters);
		MtpReponseParams(const MtpReponseParams& other);
		MtpReponseParams& operator=(const MtpReponseParams& other);
		~MtpReponseParams();

		void SetCollection(CComPtr<IPortableDevicePropVariantCollection> paramsCollection);
		void SetCollection(CComPtr<IPortableDeviceValues> eventParameters);

		/*uint32_t getUint32(size_t pos);
		uint16_t getUint16(size_t param);
		int32_t getInt32(size_t pos);
		int16_t getInt16(size_t pos);*/

	protected:
		std::vector<PROPVARIANT> pv_;
	};


	class NEK_API MtpParams {
	public:
		MtpParams() {};
		~MtpParams();

		CComPtr<IPortableDevicePropVariantCollection> GetCollection();

		void addUint32(uint32_t param);
		void addUint16(uint16_t param);
		void addInt32(int32_t param);
		void addInt16(int16_t param);

	protected:
		std::vector<PROPVARIANT> pv_;
	};


	class NEK_API MtpResponse {
	public:
		MtpResponse();

		MtpReponseParams& GetParams();

		uint16_t responseCode;
		std::vector<BYTE> data;

	private:
		MtpReponseParams responseParams_;
	};


	class NEK_API MtpEvent {
	public:
		MtpEvent(uint16_t eventCode);
		MtpEvent(uint16_t eventCode, uint32_t param);
		MtpEvent(uint16_t eventCode, std::vector<uint32_t> params);

		uint16_t eventCode;
		std::vector<uint32_t> eventParams;
	};


	class MtpEventCallback : public IPortableDeviceEventCallback {
	public:
		MtpEventCallback();

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
		std::vector<std::pair<size_t, std::function<void(MtpEvent)>>> callbacks_;
	};

}