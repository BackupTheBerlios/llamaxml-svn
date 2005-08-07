#pragma once

#include "InputStream.h"
#include <VfsMgr.h>

namespace LlamaXML {

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
