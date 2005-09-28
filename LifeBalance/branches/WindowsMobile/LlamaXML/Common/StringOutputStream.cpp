/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "StringOutputStream.h"

namespace LlamaXML {

	StringOutputStream::StringOutputStream()
	{
	}
		
	void StringOutputStream::WriteData(const char * buffer, uint32_t length) {
		mString.append(buffer, length);
	}

}
