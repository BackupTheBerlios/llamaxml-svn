/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/OutputStream.h"
#if __MACH__
	#include <Carbon/Carbon.h>
#else
    #include <Carbon.h>
#endif

namespace LlamaXML {

	class FileOutputStream: public OutputStream {
	public:
	    FileOutputStream(const FSRef * fileRef);
	    virtual ~FileOutputStream();
	    
		virtual void WriteData(const char * buffer, uint32_t length);
	
	private:
	    SInt16 mRefnum;
	};

}

#endif
