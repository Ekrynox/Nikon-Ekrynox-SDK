#pragma once
#include "../nek.hpp"

#include <stdlib.h>
#include <exception>
#include <string>



namespace nek {
	namespace mtp {

		class NEK_API MtpException : public std::exception {
		public:
			MtpException(uint32_t operationCode, uint32_t responseCode);

			uint32_t operationCode() const;
			uint32_t responseCode() const;

		private:
			uint32_t operationCode_;
			uint32_t responseCode_;
		};

	}
}