/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/VFSOutputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	VFSOutputStream::VFSOutputStream(uint16_t volRefNum, const char * pathNameP)
	: mFileRef(0)
	{
		ThrowIfXMLError(::VFSFileOpen(volRefNum, pathNameP,
			vfsModeExclusive | vfsModeWrite | vfsModeCreate | vfsModeTruncate,
			&mFileRef));
	}
	
	VFSOutputStream::~VFSOutputStream() {
		::VFSFileClose(mFileRef);
	}
	
	void VFSOutputStream::WriteData(const char * buffer, uint32_t length) {
		ThrowIfXMLError(::VFSFileWrite(mFileRef, length, buffer, NULL));
	}
	
}
