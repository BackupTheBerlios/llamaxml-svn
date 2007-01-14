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

#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/PlatformConfig.h"
#include "LlamaXML/InputStream.h"
#include <stdio.h>


namespace LlamaXML {

	/// \brief An InputStream from a file in the filesystem.  (Mac OS X, Windows and Windows Mobile only)
    
    class FileInputStream: public InputStream {
    public:
		/// Constructs a FileInputStream from a Posix file path. (Mac OS X only)
		/// Throws an XMLException if the file cannot be opened.
        FileInputStream(const char * posixPath);
		
	    virtual ~FileInputStream();
	    
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length);
		virtual void Restart();
		virtual bool EndOfFile();
	
	private:
	    FILE * mFile;
    };

}

#endif
