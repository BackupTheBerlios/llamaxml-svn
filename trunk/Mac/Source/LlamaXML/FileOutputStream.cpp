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