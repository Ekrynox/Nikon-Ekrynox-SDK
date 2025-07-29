#include "nek_mtp_utils.hpp"

#include <stdexcept>

#include <propvarutil.h>



using namespace nek::mtp;


MtpParams::MtpParams() {
	HRESULT hr = CoCreateInstance(CLSID_PortableDevicePropVariantCollection, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&paramsCollection_));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create Prop Variant Collection: " + hr);
	}
}

IPortableDevicePropVariantCollection* MtpParams::GetCollection() const {
	return paramsCollection_;
}


void MtpParams::addUint32(uint32_t value) {
	PROPVARIANT pv;
	InitPropVariantFromUInt32(value, &pv);
	paramsCollection_->Add(&pv);
	PropVariantClear(&pv);
}

void MtpParams::addUint16(uint16_t value) {
	PROPVARIANT pv;
	InitPropVariantFromUInt16(value, &pv);
	paramsCollection_->Add(&pv);
	PropVariantClear(&pv);
}

void MtpParams::addInt32(int32_t value) {
	PROPVARIANT pv;
	InitPropVariantFromInt32(value, &pv);
	paramsCollection_->Add(&pv);
	PropVariantClear(&pv);
}

void MtpParams::addInt16(int16_t value) {
	PROPVARIANT pv;
	InitPropVariantFromInt16(value, &pv);
	paramsCollection_->Add(&pv);
	PropVariantClear(&pv);
}