#include "nek_wpd.hpp"

#include "../nek_mtp_enum.hpp"

#include <algorithm>



namespace nek::mtp::backend::wpd {

	WpdMtpTransport::WpdMtpTransport(const std::wstring& devicePath) {
		this->devicePath_ = devicePath;
		running_ = false;
		command_ = nullptr;
	}

	WpdMtpTransport::~WpdMtpTransport() {
		disconnect();
	}



	void WpdMtpTransport::connect() {
		if (running_) return; //TODO: Throw an error like device already connected

		running_ = true;
		commandThread_ = std::thread(&WpdMtpTransport::commandLoop, this);

		std::unique_lock lk(commandMutex_);
		commandCV_.wait(lk, [this] { return this->isConnected(); });
		lk.unlock();
	}

	void WpdMtpTransport::disconnect() {
		if (!running_) return; //TODO: Throw an error like device already disconnected
		running_ = false;
		commandCV_.notify_all();
		commandThread_.join();
	}

	bool WpdMtpTransport::isConnected() const {
		if (!running_) return false;
		if (device_ == nullptr) return false;
		return true;
	}



	MtpResponse WpdMtpTransport::sendCommand(uint16_t operationCode, const MtpParams& params) {
		std::unique_lock lk(commandMutex_);
		commandCV_.wait(lk, [this] { return !this->running_ || this->command_ == nullptr; });

		auto p = std::promise<MtpResponse>();
		auto f = p.get_future();
		command_ = new std::function([this, &p, operationCode, &params] {
			try {
				p.set_value(this->sendCommand_(operationCode, params));
			}
			catch (...) {
				p.set_exception(std::current_exception());
			}
			});

		lk.unlock();
		commandCV_.notify_all();

		return f.get();
	}

	MtpResponse WpdMtpTransport::sendCommandAndRead(uint16_t operationCode, const MtpParams& params) {
		std::unique_lock lk(commandMutex_);
		commandCV_.wait(lk, [this] { return !this->running_ || this->command_ == nullptr; });

		auto p = std::promise<MtpResponse>();
		auto f = p.get_future();
		command_ = new std::function([this, &p, operationCode, &params] {
			try {
				p.set_value(this->sendCommandAndRead_(operationCode, params));
			}
			catch (...) {
				p.set_exception(std::current_exception());
			}
			});

		lk.unlock();
		commandCV_.notify_all();

		return f.get();
	}

	MtpResponse WpdMtpTransport::sendCommandAndWrite(uint16_t operationCode, const MtpParams& params, const std::vector<uint8_t>& data) {
		std::unique_lock lk(commandMutex_);
		commandCV_.wait(lk, [this] { return !this->running_ || this->command_ == nullptr; });

		auto p = std::promise<MtpResponse>();
		auto f = p.get_future();
		command_ = new std::function([this, &p, operationCode, &params, &data] {
			try {
				p.set_value(this->sendCommandAndWrite_(operationCode, params, data));
			}
			catch (...) {
				p.set_exception(std::current_exception());
			}
			});

		lk.unlock();
		commandCV_.notify_all();

		return f.get();
	}



	void WpdMtpTransport::initCom() {
		//Com context
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if (hr == RPC_E_CHANGED_MODE) {
			//throw MtpDeviceException(MtpExPhase::COM_INIT, hr); TODO
		}

		//Device Client
		if (deviceClient_ != nullptr) {
			return; //Already initialized
		}
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceClient_));
		if (FAILED(hr)) {
			//throw MtpDeviceException(MtpExPhase::DEVICECLIENT_INIT, hr); TODO
		}
		deviceClient_->SetStringValue(WPD_CLIENT_NAME, CLIENT_NAME);
		deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MAJOR_VERSION, CLIENT_MAJOR_VER);
		deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_MINOR_VERSION, CLIENT_MINOR_VER);
		deviceClient_->SetUnsignedIntegerValue(WPD_CLIENT_REVISION, CLIENT_REVISION);
	}

	void WpdMtpTransport::initDevice() {
		if (device_ != nullptr) {
			return; //Already connected
		}

		HRESULT hr = CoCreateInstance(CLSID_PortableDeviceFTM, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&device_));
		if (FAILED(hr)) {
			//throw MtpDeviceException(MtpExPhase::DEVICE_INIT, hr); TODO
		}

		hr = device_->Open(devicePath_.c_str(), deviceClient_);
		if (FAILED(hr)) {
			device_.Release();
			//throw MtpDeviceException(MtpExPhase::DEVICE_INIT, hr); TODO
		}
	}

	void WpdMtpTransport::commandLoop() {
		initCom();
		initDevice();
		commandCV_.notify_all();

		while (running_) {
			commandMutex_.lock();
			if (command_ != nullptr) (*command_)();
			delete command_;
			command_ = nullptr;
			commandMutex_.unlock();

			std::unique_lock lk(commandMutex_);
			commandCV_.wait(lk, [this] { return !this->running_ || this->command_ != nullptr; });
			lk.unlock();
		}

		device_.Release();
		deviceClient_.Release();
		CoUninitialize();
	}


	MtpResponse WpdMtpTransport::sendCommand_(uint16_t operationCode, const MtpParams& params) {
		MtpResponse result = MtpResponse();
		HRESULT hr;

		CComPtr<IPortableDeviceValues> command;
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			//throw MtpDeviceException(MtpExPhase::OPERATION_INIT, hr); TODO
		}

		// Set command category and ID
		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITHOUT_DATA_PHASE.pid);

		// Set operation code and parameters
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
		command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());

		// Send command
		CComPtr<IPortableDeviceValues> commandResult;
		hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(hr)) {
			command.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATION_SEND, hr); TODO
		}

		// Extract response code
		ULONG tempCode;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, &tempCode);
		result.responseCode = static_cast<uint16_t>(tempCode);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATION_RESPONSE, hr); TODO
		}

		// Extract response parameters
		CComPtr<IPortableDevicePropVariantCollection> parametersCollection;
		hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &parametersCollection);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATION_RESPONSE, hr); TODO
		}

		result.GetParams().SetCollection(parametersCollection);
		parametersCollection.Release();
		command.Release();
		commandResult.Release();

		return result;
	}

	MtpResponse WpdMtpTransport::sendCommandAndRead_(uint16_t operationCode, const MtpParams& params) {
		MtpResponse result = MtpResponse();
		HRESULT hr;

		CComPtr<IPortableDeviceValues> command;
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			//throw MtpDeviceException(MtpExPhase::OPERATIONREAD_INIT, hr); TODO
		}

		// Set command category and ID
		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_READ.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_READ.pid);

		// Set operation code and parameters
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
		command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());

		// Send command
		CComPtr<IPortableDeviceValues> commandResult;
		hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(hr)) {
			command.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONREAD_SEND, hr); TODO
		}

		LPWSTR context;
		hr = commandResult->GetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, &context);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONREAD_RESPONSE, hr); TODO
		}

		ULONG totalSize;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_TOTAL_DATA_SIZE, &totalSize);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONREAD_RESPONSE, hr); TODO
		}

		ULONG optimalSize;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPTIMAL_TRANSFER_BUFFER_SIZE, &optimalSize);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONREAD_RESPONSE, hr); TODO
		}

		command.Release();
		commandResult.Release();


		// Start Data Transfert
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::DATAREAD_INIT, hr); TODO
		}
		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_READ_DATA.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_READ_DATA.pid);
		command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
		optimalSize = std::min(optimalSize, totalSize);
		uint8_t* buffer = new uint8_t[optimalSize];
		result.data.resize(totalSize);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_NUM_BYTES_TO_READ, optimalSize);
		command->SetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, buffer, optimalSize);

		uint8_t* b = nullptr;
		DWORD bNb = 0;
		DWORD offset = 0;
		size_t retry = 0;
		do {
			hr = device_->SendCommand(0, command, &commandResult);
			if (FAILED(hr)) {
				CoTaskMemFree(context);
				command.Release();
				commandResult.Release();
				//throw MtpDeviceException(MtpExPhase::DATAREAD_SEND, hr); TODO
			}
			hr = commandResult->GetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, &b, &bNb);
			commandResult.Release();
			if (SUCCEEDED(hr) && (b != nullptr)) {
				if (bNb > 0) {
					std::memcpy(result.data.data() + offset, b, bNb);
					CoTaskMemFree(b);
					b = nullptr;
					offset += bNb;
					retry = 0;
				}
				else {
					retry += 1;
					CoTaskMemFree(b);
					b = nullptr;
					if (retry >= 10) {
						result.responseCode = MtpResponseCode::Incomplete_Transfer;
						CoTaskMemFree(context);
						command.Release();
						commandResult.Release();
						return result;
					}
				}
			}
		} while (totalSize > offset);

		delete[] buffer;
		command.Release();


		// End Data Transfert
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::ENDREAD_INIT, hr); TODO
		}
		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.pid);
		command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
		hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(hr)) {
			CoTaskMemFree(context);
			command.Release();
			//throw MtpDeviceException(MtpExPhase::ENDREAD_SEND, hr); TODO
		}

		// Extract response code
		ULONG tempCode;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, &tempCode);
		result.responseCode = static_cast<uint16_t>(tempCode);
		if (FAILED(hr)) {
			CoTaskMemFree(context);
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::ENDREAD_RESPONSE, hr); TODO
		}

		// Extract response parameters
		CComPtr<IPortableDevicePropVariantCollection> parametersCollection;
		hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &parametersCollection);
		if (FAILED(hr)) {
			CoTaskMemFree(context);
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::ENDREAD_RESPONSE, hr); TODO
		}
		result.GetParams().SetCollection(parametersCollection);

		parametersCollection.Release();
		CoTaskMemFree(context);
		command.Release();
		commandResult.Release();
		return result;
	}

	MtpResponse WpdMtpTransport::sendCommandAndWrite_(uint16_t operationCode, const MtpParams& params, const std::vector<uint8_t>& data) {
		MtpResponse result = MtpResponse();
		HRESULT hr;

		CComPtr<IPortableDeviceValues> command;
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			//throw MtpDeviceException(MtpExPhase::OPERATIONWRITE_INIT, hr); TODO
		}

		// Set command category and ID
		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_WRITE.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_EXECUTE_COMMAND_WITH_DATA_TO_WRITE.pid);

		// Set operation code and parameters
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPERATION_CODE, operationCode);
		command->SetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_OPERATION_PARAMS, params.GetCollection());
		command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_TOTAL_DATA_SIZE, (ULONG)data.size());

		// Send command
		CComPtr<IPortableDeviceValues> commandResult;
		hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONWRITE_SEND, hr); TODO
		}

		LPWSTR context;
		hr = commandResult->GetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, &context);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			//throw MtpDeviceException(MtpExPhase::OPERATIONWRITE_RESPONSE, hr); TODO
		}

		ULONG optimalSize;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_OPTIMAL_TRANSFER_BUFFER_SIZE, &optimalSize);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::OPERATIONWRITE_RESPONSE, hr); TODO
		}

		command.Release();
		commandResult.Release();


		// Start Data Transfert
		DWORD offset = 0;
		while (offset < data.size()) {
			hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
			if (FAILED(hr)) {
				CoTaskMemFree(context);
				//throw MtpDeviceException(MtpExPhase::DATAWRITE_INIT, hr); TODO
			}

			command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_WRITE_DATA.fmtid);
			command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_WRITE_DATA.pid);
			command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);

			optimalSize = std::min(optimalSize, (ULONG)(data.size() - offset));
			command->SetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_TRANSFER_NUM_BYTES_TO_WRITE, optimalSize);
			command->SetBufferValue(WPD_PROPERTY_MTP_EXT_TRANSFER_DATA, (BYTE *)data.data() + offset, optimalSize);

			hr = device_->SendCommand(0, command, &commandResult);
			if (FAILED(hr)) {
				command.Release();
				commandResult.Release();
				CoTaskMemFree(context);
				//throw MtpDeviceException(MtpExPhase::DATAWRITE_SEND, hr); TODO
			}

			offset += optimalSize;
			commandResult.Release();
			command.Release();
		}


		// End Data Transfert
		hr = CoCreateInstance(CLSID_PortableDeviceValues, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&command));
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::ENDWRITE_INIT, hr); TODO
		}

		command->SetGuidValue(WPD_PROPERTY_COMMON_COMMAND_CATEGORY, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.fmtid);
		command->SetUnsignedIntegerValue(WPD_PROPERTY_COMMON_COMMAND_ID, WPD_COMMAND_MTP_EXT_END_DATA_TRANSFER.pid);
		command->SetStringValue(WPD_PROPERTY_MTP_EXT_TRANSFER_CONTEXT, context);
		hr = device_->SendCommand(0, command, &commandResult);
		if (FAILED(hr)) {
			command.Release();
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::ENDWRITE_SEND, hr); TODO
		}

		// Extract response code
		ULONG tempCode;
		hr = commandResult->GetUnsignedIntegerValue(WPD_PROPERTY_MTP_EXT_RESPONSE_CODE, &tempCode);
		result.responseCode = static_cast<uint16_t>(tempCode);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::ENDWRITE_RESPONSE, hr); TODO
		}

		// Extract response parameters
		CComPtr<IPortableDevicePropVariantCollection> parametersCollection;
		hr = commandResult->GetIPortableDevicePropVariantCollectionValue(WPD_PROPERTY_MTP_EXT_RESPONSE_PARAMS, &parametersCollection);
		if (FAILED(hr)) {
			command.Release();
			commandResult.Release();
			CoTaskMemFree(context);
			//throw MtpDeviceException(MtpExPhase::ENDWRITE_RESPONSE, hr); TODO
		}
		result.GetParams().SetCollection(parametersCollection);

		parametersCollection.Release();
		command.Release();
		commandResult.Release();
		CoTaskMemFree(context);
		return result;
	}

}