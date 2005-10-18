/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef VFSINPUTSTREAM_H
#define VFSINPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include <VfsMgr.h>
#include "InputStream.h"

namespace LlamaXML {

	/**
		\brief An InputStream subclass that reads data from the Palm OS
		Virtual File System.
		
		Note that any errors that occur while opening or reading
		the file will throw an ErrorCodeException.
	*/

	class VFSInputStream: public InputStream {
	public:
		VFSInputStream(uint16_t volRefNum, const char * pathNameP);
		~VFSInputStream();
		
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length);
		virtual void Restart();
		virtual bool EndOfFile();
	
	private:
		FileRef	mFileRef;
	};
	
}

#endif
