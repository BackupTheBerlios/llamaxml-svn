#include "VFSOutputStream.h"
#include "ErrorCodeException.h"

namespace LlamaXML {

	VFSOutputStream::VFSOutputStream(uint16_t volRefNum, const char * pathNameP)
	: mFileRef(0)
	{
		ThrowIfError(::VFSFileOpen(volRefNum, pathNameP,
			vfsModeExclusive | vfsModeWrite | vfsModeCreate | vfsModeTruncate,
			&mFileRef));
	}
	
	VFSOutputStream::~VFSOutputStream() {
		::VFSFileClose(mFileRef);
	}
	
	void VFSOutputStream::WriteData(const char * buffer, uint32_t length) {
		ThrowIfError(::VFSFileWrite(mFileRef, length, buffer, NULL));
	}
	
}
