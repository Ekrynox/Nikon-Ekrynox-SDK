#pragma once

#include <vector>

#include <atlbase.h>
#include <PortableDeviceApi.h>
#include <PortableDevice.h>



namespace nek {
	namespace mtp {

		class MtpParams {
		public:
			MtpParams();
			MtpParams(const MtpParams&) = delete;
			MtpParams& operator= (const MtpParams&) = delete;

			IPortableDevicePropVariantCollection* GetCollection() const;

			void addUint32(uint32_t param);
			void addUint16(uint16_t param);
			void addInt32(int32_t param);
			void addInt16(int16_t param);


		private:
			CComPtr<IPortableDevicePropVariantCollection> paramsCollection_;
		};


		struct MtpResponse_ {
			HRESULT hr = E_FAIL;
			uint32_t responseCode = 0;
			CComPtr<IPortableDevicePropVariantCollection> responseParams;
			std::vector<BYTE> data;
		};
		typedef struct MtpResponse_ MtpResponse;

	}
}