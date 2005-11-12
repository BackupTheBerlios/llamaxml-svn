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

#ifndef VFSOUTPUTSTREAM_H
#define VFSOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include <VfsMgr.h>
#include "OutputStream.h"

namespace LlamaXML {

	/**
		\brief An OutputStream subclass that writes data to the Palm OS
		Virtual File System.
		
		Note that any errors that occur while opening or writing
		the file will throw an ErrorCodeException.
	*/

	class VFSOutputStream: public OutputStream {
	public:
		VFSOutputStream(uint16_t volRefNum, const char * pathNameP);
		~VFSOutputStream();
	
		virtual void WriteData(const char * buffer, uint32_t length);

	private:
		FileRef	mFileRef;
	};
	
}

#endif
