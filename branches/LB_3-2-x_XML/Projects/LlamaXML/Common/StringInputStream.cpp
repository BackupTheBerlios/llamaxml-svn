/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "StringInputStream.h"
#include <string.h>

namespace LlamaXML {

	StringInputStream::StringInputStream(const std::string & s)
	: mString(s),
	  mOffset(0)
	{
	}
	
	StringInputStream::StringInputStream(const char * s)
	: mString(s),
	  mOffset(0)
	{
	}
	
	uint32_t StringInputStream::ReadUpTo(char * buffer, uint32_t length) {
		length = std::min(length, mString.size() - mOffset);
		memcpy(buffer, mString.data() + mOffset, length);
		mOffset += length;
		return length;
	}
	
	void StringInputStream::Restart() {
		mOffset = 0;
	}
	
	bool StringInputStream::EndOfFile() {
		return mOffset >= mString.size();
	}
	
}
