#pragma once

#include <stdint.h>

namespace LlamaXML {

	class OutputStream {
	public:
		virtual void WriteData(const char * buffer, uint32_t length) = 0;
	};

}

LlamaXML::OutputStream & operator << (LlamaXML::OutputStream & stream,
	const char * s);
