/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef STRINGOUTPUTSTREAM_H
#define STRINGOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/OutputStream.h"
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

#endif
