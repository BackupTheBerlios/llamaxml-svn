#pragma once

#include "DataOutputStream.h"
#include <VfsMgr.h>

namespace LlamaXML {

	class VFSOutputStream: public DataOutputStream {
	public:
		VFSOutputStream(uint16_t volRefNum, const char * pathNameP);
		~VFSOutputStream();
	
		virtual void WriteData(const char * buffer, uint32_t length);

	private:
		FileRef	mFileRef;
	};
	
}
