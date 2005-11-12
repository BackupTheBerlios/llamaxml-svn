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
#include "LlamaXML/TextEncoding.h"

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
	  mState(0)
	{
		ThrowIfXMLError(::TECCreateConverter(&mState, sourceEncoding, TextEncoding::UCS2()));
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
		::TECDisposeConverter(mState);
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
		::TECDisposeConverter(mState);
		mState = 0;
		mSourceEncoding = sourceEncoding;
		ThrowIfXMLError(::TECCreateConverter(&mState, sourceEncoding, TextEncoding::UCS2()));
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
		ByteCount sourceRead = 0;
		ByteCount destWritten = 0;
		OSStatus status = ::TECConvertText(mState, reinterpret_cast<ConstTextPtr>(sourceStart), (sourceEnd - sourceStart) * sizeof(*sourceStart),
		    &sourceRead, reinterpret_cast<TextPtr>(destStart), (destEnd - destStart) * sizeof(*destStart), &destWritten);
		if ((status == noErr) || (status == kTECArrayFullErr) || (status == kTECPartialCharErr) || (status == kTECIncompleteElementErr)
			|| (status == kTECUsedFallbacksStatus) || (status == kTECOutputBufferFullStatus)) {
			sourceStart += sourceRead / sizeof(*sourceStart);
			destStart += destWritten / sizeof(*destStart);
		}
		else ThrowXMLError(status);
	}
}
