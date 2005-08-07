#pragma once

#include "InputStream.h"
#include <string>

namespace LlamaXML {

	class StringInputStream: public InputStream {
	public:
		StringInputStream(const std::string & s);
		StringInputStream(const char * s);
		
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length);
		virtual void Restart();
		virtual bool EndOfFile();
	
	private:
		std::string	mString;
		uint32_t	mOffset;
	};
	
}
