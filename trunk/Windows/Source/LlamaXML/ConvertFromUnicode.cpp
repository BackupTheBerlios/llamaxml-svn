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

#include "LlamaXML/ConvertFromUnicode.h"
#include "LlamaXML/XMLException.h"
#include "LlamaXML/PlatformConfig.h"

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding)
	{
	}
	
	ConvertFromUnicode::~ConvertFromUnicode() {
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
		while ((sourceStart < sourceEnd) && (destStart < destEnd)) {
			int result = ::WideCharToMultiByte(mDestinationEncoding, 0, sourceStart, 1,
				destStart, int(destEnd - destStart), 0, 0);
			if (result > 0) {
				destStart += result;
				sourceStart += 1;
			}
			else {
				DWORD err = ::GetLastError();
				if (err == ERROR_INSUFFICIENT_BUFFER) {
					return;
				}
				else {
					ThrowXMLError(err);
				}
			}
		}
	}
}
