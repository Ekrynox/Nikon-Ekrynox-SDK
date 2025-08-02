#include "nek_mtp_utils.hpp"

#include <stdexcept>
#include <iostream>

#include <propvarutil.h>



using namespace nek::mtp;



//MtpResponseParams
MtpReponseParams::MtpReponseParams(CComPtr<IPortableDevicePropVariantCollection> paramsCollection) {
	SetCollection(paramsCollection);
}

MtpReponseParams::~MtpReponseParams() {
	for (auto& pv : pv_) {
		PropVariantClear(&pv);
	}
	pv_.clear();
}

void MtpReponseParams::SetCollection(CComPtr<IPortableDevicePropVariantCollection> paramsCollection) {
	pv_.clear();
	DWORD size = 0;
	paramsCollection->GetCount(&size);
	for (DWORD i = 0; i < size; i++) {
		PROPVARIANT pv;
		paramsCollection->GetAt(i, &pv);
		pv_.push_back(pv);
	}
}




//MtpParams
CComPtr<IPortableDevicePropVariantCollection> MtpParams::GetCollection() {
	CComPtr<IPortableDevicePropVariantCollection> paramsCollection;
	HRESULT hr = CoCreateInstance(CLSID_PortableDevicePropVariantCollection, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&paramsCollection));
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to create Prop Variant Collection: " + hr);
	}
	for (auto pv : pv_) {
		paramsCollection->Add(&pv);
	}

	return paramsCollection;
}

void MtpParams::addUint32(uint32_t value) {
	PROPVARIANT pv;
	InitPropVariantFromUInt32(value, &pv);
	pv_.push_back(pv);
}

void MtpParams::addUint16(uint16_t value) {
	PROPVARIANT pv;
	InitPropVariantFromUInt16(value, &pv);
	pv_.push_back(pv);
}

void MtpParams::addInt32(int32_t value) {
	PROPVARIANT pv;
	InitPropVariantFromInt32(value, &pv);
	pv_.push_back(pv);
}

void MtpParams::addInt16(int16_t value) {
	PROPVARIANT pv;
	InitPropVariantFromInt16(value, &pv);
	pv_.push_back(pv);
}




//MtpResponse
MtpResponse::MtpResponse() {
	hr = E_FAIL;
	responseCode = 0;
}

MtpReponseParams& MtpResponse::GetParams() {
	return responseParams_;
}




//MtpEventCallback
MtpEventCallback::MtpEventCallback() : ref_(0), nextId(0) {}

HRESULT MtpEventCallback::OnEvent(IPortableDeviceValues* pEventParameters) {
	std::lock_guard<std::mutex> lock(mutex_);

	for (auto& [id, callback] : callbacks_) {
		callback(pEventParameters);
	}
	return HRESULT(0);
}

HRESULT MtpEventCallback::QueryInterface(REFIID riid, void** ppv) {
	static const QITAB qitab[] = { QITABENT(MtpEventCallback, IPortableDeviceEventCallback), { }, };
	return QISearch(this, qitab, riid, ppv);
}

ULONG MtpEventCallback::AddRef() {
	return InterlockedIncrement(&ref_);
}

ULONG MtpEventCallback::Release() {
	ULONG ref = _InterlockedDecrement(&ref_);
	if (ref == 0) {
		delete this;
	}

	return ref;
}


size_t MtpEventCallback::RegisterCallback(std::function<void(IPortableDeviceValues*)> callback) {
	std::lock_guard<std::mutex> lock(mutex_);

	size_t id = nextId++;
	callbacks_.emplace_back(id, callback);
	return id;
}

void MtpEventCallback::UnregisterCallback(size_t id) {
	std::lock_guard<std::mutex> lock(mutex_);

	callbacks_.erase(std::remove_if(callbacks_.begin(), callbacks_.end(), [id](const auto& pair) { return pair.first == id; }), callbacks_.end());
}