/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef VFSOUTPUTSTREAM_H
#define VFSOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include <VfsMgr.h>
#include "OutputStream.h"

namespace LlamaXML {

	/**
		\brief An OutputStream subclass that writes data to the Palm OS
		Virtual File System.
		
		Note that any errors that occur while opening or writing
		the file will throw an ErrorCodeException.
	*/

	class VFSOutputStream: public OutputStream {
	public:
		VFSOutputStream(uint16_t volRefNum, const char * pathNameP);
		~VFSOutputStream();
	
		virtual void WriteData(const char * buffer, uint32_t length);

	private:
		FileRef	mFileRef;
	};
	
}

#endif
