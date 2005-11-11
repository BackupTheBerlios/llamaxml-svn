/*
 * Copyright (C) 2005 Llamagraphics, Inc.
 * 
 * This file is part of LlamaXML.
 *
 * LlamaXML is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LlamaXML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * LlamaXML is also available under an alternative commercial license for
 * developers who need the flexibility to include this library in proprietary
 * applications.  Visit http://www.llamagraphics.com/LlamaXML/ for more 
 * information.
 */

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
