/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "BufferInputStream.h"
#include <string.h>

namespace LlamaXML {

	BufferInputStream::BufferInputStream(const void * buffer, uint32_t length)
	: mBuffer(reinterpret_cast<const char *>(buffer)),
	  mLength(length),
	  mOffset(0)
	{
	}
	
	uint32_t BufferInputStream::ReadUpTo(char * buffer, uint32_t length) {
		length = std::min(length, mLength - mOffset);
		memcpy(buffer, mBuffer + mOffset, length);
		mOffset += length;
		return length;
	}
	
	void BufferInputStream::Restart() {
		mOffset = 0;
	}
	
	bool BufferInputStream::EndOfFile() {
		return mOffset >= mLength;
	}
	
}
