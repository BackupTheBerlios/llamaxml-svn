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

#include "LlamaXML/ConvertToUnicode.h"
#include "LlamaXML/XMLException.h"
#include "LlamaXML/PlatformConfig.h"

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding)
	{
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart, const char * sourceEnd,
		UnicodeChar * & destStart, UnicodeChar * destEnd)
	{
		while ((sourceStart < sourceEnd) && (destStart < destEnd)) {
			const char * charEnd = sourceStart;
			while (true) {
				if (charEnd >= sourceEnd) return;
				if (! ::IsDBCSLeadByteEx(mSourceEncoding, *charEnd++)) break;
			}
			int result = ::MultiByteToWideChar(mSourceEncoding, 0, sourceStart, int(charEnd - sourceStart),
				destStart, int(destEnd - destStart));
			if (result > 0) {
				destStart += result;
				sourceStart = charEnd;
			}
			else {
				if (result == 0) {
					result = ::GetLastError();
				}
				if (result != ERROR_INSUFFICIENT_BUFFER) ThrowXMLError(result);
			}
		}
	}
}
