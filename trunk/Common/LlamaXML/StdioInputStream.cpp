/*
 *  StdioInputStream.cpp
 *  BuddySystem
 *
 *  Created by Stuart A. Malone on 8/28/09.
 *  Copyright 2009 Llamagraphics, Inc. All rights reserved.
 *
 */

#include "LlamaXML/StdioInputStream.h"
#include "LlamaXML/XMLException.h"

extern "C" int errno;

namespace LlamaXML {

	StdioInputStream::StdioInputStream(const char * path)
	: mFile(fopen(path, "rb"))
	{
		if (! mFile) {
			ThrowXMLError(errno);
		}
	}
	
	StdioInputStream::~StdioInputStream() {
		fclose(mFile);
	}

	uint32_t StdioInputStream::ReadUpTo(char * buffer, uint32_t length) {
		size_t bytesRead = fread(buffer, sizeof(*buffer), length, mFile);
		if (ferror(mFile)) {
			ThrowXMLError(errno);
		}
		return bytesRead;
	}
		
	void StdioInputStream::Restart() {
		rewind(mFile);
	}

	bool StdioInputStream::EndOfFile() {
		return feof(mFile) != 0;
	}

}
