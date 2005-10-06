/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "FileOutputStream.h"
#include "XMLException.h"

namespace LlamaXML {
    
    FileOutputStream::FileOutputStream(const FSRef * fileRef)
    : mRefnum(0)
    {
        HFSUniStr255 dataForkName;
        ThrowIfXMLError(::FSGetDataForkName(&dataForkName));
        ThrowIfXMLError(::FSOpenFork(fileRef, dataForkName.length, dataForkName.unicode,
            fsWrDenyPerm, &mRefnum));
    }
    

    void FileOutputStream::WriteData(const char * buffer, uint32_t length) {
        ThrowIfXMLError(::FSWriteFork(mRefnum, fsAtMark, 0, length, buffer, 0));
    }
    
}
