/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	XMLException::XMLException(int32_t err, const char * file, long line) throw()
	: mErrorCode(err),
	  mFile(file),
	  mLine(line)
	{
	}


	void ThrowXMLException(int32_t err, const char * file, long line)
	{
		throw XMLException(err, file, line);
	}

}
