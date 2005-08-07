#pragma once

#include "VFSInputStream.h"
#include "ErrorCodeException.h"

namespace LlamaXML {

	VFSInputStream::VFSInputStream(uint16_t volRefNum, const char * pathNameP)
	: mFileRef(0)
	{
		ThrowIfError(::VFSFileOpen(volRefNum, pathNameP, vfsModeRead, &mFileRef));
	}
	
	VFSInputStream::~VFSInputStream() {
		::VFSFileClose(mFileRef);
	}
	
	uint32_t VFSInputStream::ReadUpTo(char * buffer, uint32_t length) {
		uint32_t numBytesRead;
		Err err = ::VFSFileRead(mFileRef, length, buffer, &numBytesRead);
		if ((err == errNone) || (err == vfsErrFileEOF)) {
			return numBytesRead;
		}
		else {
			ThrowError(err);
			return 0;	// to suppress compiler warning
		}
	}
	
	void VFSInputStream::Restart() {
		ThrowIfError(::VFSFileSeek(mFileRef, vfsOriginBeginning, 0));
	}
	
	bool VFSInputStream::EndOfFile() {
		Err err = ::VFSFileEOF(mFileRef);
		if (err == errNone) return false;
		else if (err == vfsErrFileEOF) return true;
		else {
			ThrowError(err);
			return true;	// to suppress compiler warning
		}
	}
	
}
