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
#include "LlamaXML/RecodeOuter.h"
#include <errno.h>
#include <cstring>
#include <algorithm>

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
	  mRequest(recode_new_request(RecodeOuter::Get())),
	  mOutputBuffer(0),
	  mOutputBufferUsed(0),
	  mOutputBufferSize(0),
	  mOutputBufferAlloc(0)
	{
		Reset(sourceEncoding);
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
		if (mOutputBuffer) free(mOutputBuffer);
		recode_delete_request(mRequest);
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
		mSourceEncoding = sourceEncoding;
		std::string conversion(mSourceEncoding);
		conversion += "..UCS-2-INTERNAL";
		if (! recode_scan_request(mRequest, conversion.c_str())) {
			ThrowXMLException(EINVAL, "Invalid encoding");
		}
	}
	
	void ConvertToUnicode::ShiftOutput(UnicodeChar * & destStart, UnicodeChar * destEnd) {
		size_t shiftCount = std::min<size_t>(destEnd - destStart,
			(mOutputBufferSize - mOutputBufferUsed) / sizeof(*destStart));
		if (shiftCount > 0) {
			std::memcpy(destStart, mOutputBuffer + mOutputBufferUsed, 
				shiftCount * sizeof(*destStart));
			mOutputBufferUsed += shiftCount * sizeof(*destStart);
			destStart += shiftCount;
			if (mOutputBufferUsed == mOutputBufferSize) {
				mOutputBufferUsed = mOutputBufferSize = 0;
			}
		}
	}
		
	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
		// If there is text left over from the previous conversion, append it first.
		ShiftOutput(destStart, destEnd);
		// If the output buffer is empty, fill it.
		if (mOutputBufferSize == 0) {
			if (! recode_buffer_to_buffer(mRequest, reinterpret_cast<const char *>(sourceStart),
					(sourceEnd - sourceStart) * sizeof(*sourceStart),
					&mOutputBuffer, &mOutputBufferSize, &mOutputBufferAlloc)) {
				ThrowXMLException(EILSEQ, "Conversion error");
			}
			// Append as much of the new output as we can.
			ShiftOutput(destStart, destEnd);
		}
	}
}
