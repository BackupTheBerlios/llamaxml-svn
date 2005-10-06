/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "OutputStream.h"
#include <Carbon/Carbon.h>

namespace LlamaXML {

	class FileOutputStream {
	public:
	    FileOutputStream(const FSRef * fileRef);
	    
		virtual void WriteData(const char * buffer, uint32_t length);
	
	private:
	    SInt16 mRefnum;
	};

}

#endif
