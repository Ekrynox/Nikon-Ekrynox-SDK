#include "nek_mtp_utils.hpp"

#include <stdexcept>

#include <propvarutil.h>



using namespace nek::mtp;



//MtpEvent
MtpEvent::MtpEvent(uint16_t eventCode) {
	this->eventCode = eventCode;
}
MtpEvent::MtpEvent(uint16_t eventCode, uint32_t param) {
	this->eventCode = eventCode;
	this->eventParams.push_back(param);
}
MtpEvent::MtpEvent(uint16_t eventCode, std::vector<uint32_t> params) {
	this->eventCode = eventCode;
	this->eventParams = params;
}



//MtpEventCallback
MtpEventCallback::MtpEventCallback() : ref_(0), nextId(0) {
	callbacks_ = new std::vector<std::pair<size_t, std::function<void(MtpEvent)>>>();
}

MtpEventCallback::~MtpEventCallback() {
	delete callbacks_;
}

HRESULT MtpEventCallback::OnEvent(IPortableDeviceValues* pEventParameters) {
	MtpEvent event = MtpEvent(0); //TO UPDATE TO EXTRACT DATA
	return OnEvent(event);
}

HRESULT MtpEventCallback::OnEvent(MtpEvent event) {
	mutex_.lock();
	auto callbacks = *callbacks_;
	mutex_.unlock();
	for (auto& [id, callback] : callbacks) {
		callback(event);
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


size_t MtpEventCallback::RegisterCallback(std::function<void(MtpEvent)> callback) {
	std::lock_guard<std::mutex> lock(mutex_);

	size_t id = nextId++;
	callbacks_->emplace_back(id, callback);
	return id;
}

void MtpEventCallback::UnregisterCallback(size_t id) {
	std::lock_guard<std::mutex> lock(mutex_);

	callbacks_->erase(std::remove_if(callbacks_->begin(), callbacks_->end(), [id](const auto& pair) { return pair.first == id; }), callbacks_->end());
}
