#pragma once

#include "OutputStream.h"
#include <string>

namespace LlamaXML {

	class StringOutputStream {
	public:
		StringOutputStream();
		
		virtual void WriteData(const char * buffer, uint32_t length);
		
		const std::string & GetString() const {
			return mString;
		}
		
	private:
		std::string	mString;
	};

}
