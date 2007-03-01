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
#include <errno.h>
#include <cstring>
#include <algorithm>

namespace LlamaXML {

	ConvertFromUnicode::ConvertFromUnicode(TextEncoding destinationEncoding)
	: mDestinationEncoding(destinationEncoding),
          mConverter(iconv_open(destinationEncoding, TextEncoding::UCS2()))
	{
            if (mConverter == (iconv_t)-1) {
                ThrowXMLException(EINVAL, "Invalid encoding");
            }
	}
	
	ConvertFromUnicode::~ConvertFromUnicode() {
            iconv_close(mConverter);
	}
	
	void ConvertFromUnicode::Convert(const UnicodeChar * & sourceStart,
		const UnicodeChar * sourceEnd, char * & destStart,
		char * destEnd)
	{
            char * inbuf = const_cast<char *>(reinterpret_cast<const char *>(sourceStart));
            size_t inbytesleft = (sourceEnd - sourceStart) * sizeof(*sourceStart);
            char * outbuf = destStart;
            size_t outbytesleft = (destEnd - destStart) * sizeof(*destStart);

            size_t result = iconv (mConverter, &inbuf, &inbytesleft, &outbuf, &outbytesleft);

            sourceStart = reinterpret_cast<const UnicodeChar *>(inbuf);
            destStart = outbuf;

            if ((result == (size_t)-1) && (errno == EILSEQ)) {
                ThrowXMLException(EILSEQ, "Illegal character sequence");
            }
        }
}
