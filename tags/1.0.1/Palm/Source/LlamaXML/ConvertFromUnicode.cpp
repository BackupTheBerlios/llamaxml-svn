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

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding),
	  mNewConversion(true)
	{
	}
		
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
#if PALMOS_SDK_VERSION >= 0x600
		typedef size_t TxtConvertLength;
#else
		typedef uint16_t TxtConvertLength;
#endif
		TxtConvertLength sourceBytes = TxtConvertLength((sourceEnd - sourceStart) * sizeof(*sourceStart));
		TxtConvertLength destBytes = TxtConvertLength((destEnd - destStart) * sizeof(*destStart));
		Err err = ::TxtConvertEncoding(mNewConversion, &mState,
			reinterpret_cast<const Char *>(sourceStart), &sourceBytes,
			charEncodingUCS2,
			reinterpret_cast<Char *>(destStart),
			&destBytes,
			mDestinationEncoding.AsPalmCharEncoding(), 0, 0);
		if ((err != errNone) && (err != txtErrConvertOverflow) &&
			(err != txtErrConvertUnderflow)) {
			ThrowXMLError(err);
		}
		sourceStart += (sourceBytes / sizeof(*sourceStart));
		destStart += (destBytes / sizeof(*destStart));
		mNewConversion = false;
	}
}
