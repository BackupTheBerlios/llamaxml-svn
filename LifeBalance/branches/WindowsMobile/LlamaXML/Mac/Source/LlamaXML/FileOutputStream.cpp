/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/FileOutputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {
    
    FileOutputStream::FileOutputStream(const FSRef * fileRef)
    : mRefnum(0)
    {
        HFSUniStr255 dataForkName;
        ThrowIfXMLError(::FSGetDataForkName(&dataForkName));
        ThrowIfXMLError(::FSOpenFork(fileRef, dataForkName.length, dataForkName.unicode,
            fsWrDenyPerm, &mRefnum));
    }
    
    
    FileOutputStream::~FileOutputStream() {
        ::FSCloseFork(mRefnum);
    }
    

    void FileOutputStream::WriteData(const char * buffer, uint32_t length) {
        ThrowIfXMLError(::FSWriteFork(mRefnum, fsAtMark, 0, length, buffer, 0));
    }
    
}
