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

#include "LlamaXML/VFSInputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	VFSInputStream::VFSInputStream(uint16_t volRefNum, const char * pathNameP)
	: mFileRef(0)
	{
		ThrowIfXMLError(::VFSFileOpen(volRefNum, pathNameP, vfsModeRead, &mFileRef));
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
			ThrowXMLError(err);
			return 0;	// to suppress compiler warning
		}
	}
	
	void VFSInputStream::Restart() {
		ThrowIfXMLError(::VFSFileSeek(mFileRef, vfsOriginBeginning, 0));
	}
	
	bool VFSInputStream::EndOfFile() {
		Err err = ::VFSFileEOF(mFileRef);
		if (err == errNone) return false;
		else if (err == vfsErrFileEOF) return true;
		else {
			ThrowXMLError(err);
			return true;	// to suppress compiler warning
		}
	}
	
}
