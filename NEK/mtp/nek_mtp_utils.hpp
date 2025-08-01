#pragma once
#include "../nek.hpp"

#include <future>
#include <vector>

#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>
#include <functional>



namespace nek {
	namespace mtp {

		class NEK_API MtpReponseParams {
		public:
			MtpReponseParams();
			MtpReponseParams(const MtpReponseParams&) = delete;
			MtpReponseParams& operator= (const MtpReponseParams&) = delete;

			CComPtr<IPortableDevicePropVariantCollection> &GetCollection();

		protected:
			CComPtr<IPortableDevicePropVariantCollection> paramsCollection_;
		};


		class NEK_API MtpParams : public MtpReponseParams {
		public:
			MtpParams();
			MtpParams(const MtpParams&) = delete;
			MtpParams& operator= (const MtpParams&) = delete;

			void addUint32(uint32_t param);
			void addUint16(uint16_t param);
			void addInt32(int32_t param);
			void addInt16(int16_t param);
		};


		class NEK_API MtpResponse {
		public:
			MtpResponse();
			MtpResponse(const MtpResponse&) = delete;
			MtpResponse& operator= (const MtpResponse&) = delete;

			MtpReponseParams& GetParams();

			HRESULT hr;
			uint32_t responseCode;
			std::vector<BYTE> data;

		private:
			MtpReponseParams responseParams_;
		};


		class MtpEventCallback : public IPortableDeviceEventCallback {
		public:
			MtpEventCallback();

			HRESULT STDMETHODCALLTYPE OnEvent(IPortableDeviceValues* pEventParameters);
			HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv);
			ULONG STDMETHODCALLTYPE AddRef();
			ULONG STDMETHODCALLTYPE Release();

			size_t RegisterCallback(std::function<void(IPortableDeviceValues*)> callback);
			void UnregisterCallback(size_t id);

		private:
			ULONG ref_;

			std::mutex mutex_;
			size_t nextId;
			std::vector<std::pair<size_t, std::function<void(IPortableDeviceValues*)>>> callbacks_;
		};

	}
}