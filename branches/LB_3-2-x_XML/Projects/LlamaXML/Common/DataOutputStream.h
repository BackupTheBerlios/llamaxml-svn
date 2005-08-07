#pragma once

#include <stdint.h>

namespace LlamaXML {

	class DataOutputStream {
	public:
		virtual void WriteData(const char * buffer, uint32_t length) = 0;
	};

}

LlamaXML::DataOutputStream & operator << (LlamaXML::DataOutputStream & stream,
	const char * s);
