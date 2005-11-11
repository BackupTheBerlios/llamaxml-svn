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

#ifndef VFSINPUTSTREAM_H
#define VFSINPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include <VfsMgr.h>
#include "InputStream.h"

namespace LlamaXML {

	/**
		\brief An InputStream subclass that reads data from the Palm OS
		Virtual File System.
		
		Note that any errors that occur while opening or reading
		the file will throw an ErrorCodeException.
	*/

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

#endif
