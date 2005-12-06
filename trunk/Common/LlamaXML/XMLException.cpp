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

#include "LlamaXML/XMLException.h"
#include "LlamaXML/StdIO.h"

namespace LlamaXML {

	XMLException::XMLException(int32_t err, const char * file, long line) throw()
	: mErrorCode(err),
	  mFile(file),
	  mLine(line),
	  mWhat(0)
	{
	}
	

	XMLException::XMLException(int32_t err, const char * what) throw()
	: mErrorCode(err),
	  mFile(0),
	  mLine(0),
	  mWhat(what)
	{
	}
	
	
	const char * XMLException::what() const throw() {
		static char buffer[256];
		if (mWhat) {
			snprintf(buffer, sizeof(buffer), "XMLException error %ld, %s", mErrorCode, mWhat);
		}
		else {
			snprintf(buffer, sizeof(buffer), "XMLException error %ld, file %s, line %d",
				mErrorCode, mFile, mLine);
		}
		return buffer;
	}


	void ThrowXMLException(int32_t err, const char * file, long line)
	{
		throw XMLException(err, file, line);
	}


	void ThrowXMLException(int32_t err, const char * what)
	{
		throw XMLException(err, what);
	}

}
