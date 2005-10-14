#include "LlamaXML/FileInputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {
    
    FileInputStream::FileInputStream(const FSRef * fileRef)
    : mRefnum(0)
    {
        HFSUniStr255 dataForkName;
        ThrowIfXMLError(::FSGetDataForkName(&dataForkName));
        ThrowIfXMLError(::FSOpenFork(fileRef, dataForkName.length, dataForkName.unicode,
            fsRdPerm, &mRefnum));
    }
    
    FileInputStream::FileInputStream(const char * posixPath)
    : mRefnum(0) {
    	FSRef fileRef;
    	Boolean isDirectory;
    	ThrowIfXMLError(::FSPathMakeRef(reinterpret_cast<const UInt8 *>(posixPath),
    	    &fileRef, &isDirectory));
        HFSUniStr255 dataForkName;
        ThrowIfXMLError(::FSGetDataForkName(&dataForkName));
        ThrowIfXMLError(::FSOpenFork(&fileRef, dataForkName.length, dataForkName.unicode,
            fsRdPerm, &mRefnum));
    }
    
    FileInputStream::~FileInputStream() {
        ::FSCloseFork(mRefnum);
    }
    
	uint32_t FileInputStream::ReadUpTo(char * buffer, uint32_t length) {
	    ByteCount actualCount;
	    OSErr err = ::FSReadFork(mRefnum, fsAtMark, 0, length, buffer, &actualCount);
	    if ((err != noErr) && (err != eofErr)) ThrowXMLError(err);
	    return actualCount;
	}
	
	void FileInputStream::Restart() {
	    ThrowIfXMLError(::FSSetForkPosition(mRefnum, fsFromStart, 0));
	}

	bool FileInputStream::EndOfFile() {
	    SInt64 current, end;
	    ThrowIfXMLError(::FSGetForkPosition(mRefnum, &current));
	    ThrowIfXMLError(::FSGetForkSize(mRefnum, &end));
	    return current >= end;
	}

}
