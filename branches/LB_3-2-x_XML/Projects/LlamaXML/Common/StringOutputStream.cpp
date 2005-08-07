#include "StringOutputStream.h"

namespace LlamaXML {

	StringOutputStream::StringOutputStream()
	{
	}
		
	void StringOutputStream::WriteData(const char * buffer, uint32_t length) {
		mString.append(buffer, length);
	}

}
