/*
 *  StdioOutputStream.cpp
 *  BuddySystem
 *
 *  Created by Stuart A. Malone on 8/28/09.
 *  Copyright 2009 Llamagraphics, Inc. All rights reserved.
 *
 */

#include "LlamaXML/StdioOutputStream.h"
#include "LlamaXML/XMLException.h"

extern "C" int errno;

namespace LlamaXML {

	StdioOutputStream::StdioOutputStream(const char * path)
	: mFile(fopen(path, "wb"))
	{
		if (! mFile) {
			ThrowXMLError(errno);
		}
	}
	
	StdioOutputStream::~StdioOutputStream() {
		fclose(mFile);
	}
	    
	void StdioOutputStream::WriteData(const char * buffer, uint32_t length) {
		size_t bytesWritten = fwrite(buffer, sizeof(*buffer), length, mFile);
		if (bytesWritten < length) {
			ThrowXMLError(errno);
		}
	}
	
}
