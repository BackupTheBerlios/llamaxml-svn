/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef BUFFERINPUTSTREAM_H
#define BUFFERINPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "InputStream.h"
#include <string>

namespace LlamaXML {

	class BufferInputStream: public InputStream {
	public:
		BufferInputStream(const void * buffer, uint32_t length);
		
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length);
		virtual void Restart();
		virtual bool EndOfFile();
	
	private:
		const char *	mBuffer;
		uint32_t		mLength;
		uint32_t		mOffset;
	};
	
}

#endif
