/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef STRINGINPUTSTREAM_H
#define STRINGINPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/InputStream.h"
#include <string>

namespace LlamaXML {

	/**
		InputStream subclass for reading data from a C or C++ string.
		
		Note that this class copies the input string.  While this is safe,
		if you don't need the data to be copied the BufferInputStream
		class is more efficient.
	*/

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

#endif
