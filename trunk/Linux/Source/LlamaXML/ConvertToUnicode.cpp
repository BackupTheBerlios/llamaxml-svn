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
#include <errno.h>
#include <cstring>
#include <algorithm>

namespace LlamaXML {

	ConvertToUnicode::ConvertToUnicode(TextEncoding sourceEncoding)
	: mSourceEncoding(sourceEncoding),
          mConverter((iconv_t)-1)
	{
            Reset(sourceEncoding);
	}
	
	ConvertToUnicode::~ConvertToUnicode() {
            if (mConverter != (iconv_t)-1) {
                iconv_close(mConverter);
            }
	}

	void ConvertToUnicode::Reset(TextEncoding sourceEncoding)
	{
            if (mConverter != (iconv_t)-1) {
                iconv_close(mConverter);
                mConverter = (iconv_t)-1;
            }
            mSourceEncoding = sourceEncoding;
            mConverter = iconv_open(TextEncoding::UCS2(), mSourceEncoding);
            if (mConverter == (iconv_t)-1) {
                ThrowXMLException(EINVAL, "Invalid encoding");
            }
	}
	
	// There is an annoying problem where some versions of iconv are defined:
	// size_t iconv (iconv_t, const char **, size_t *, char **, size_t *);
	// and others are defined:
	// size_t iconv (iconv_t,       char **, size_t *, char **, size_t *);
	// Since arg 2 is not actually modified, and the calling convention is the
	// same, we simply coerce iconv to the const version and call that.

	typedef size_t (*MyIconv)(iconv_t, const char **, size_t *, char **, size_t *);

	void ConvertToUnicode::Convert(const char * & sourceStart,
		const char * sourceEnd, UnicodeChar * & destStart,
		UnicodeChar * destEnd)
	{
            const char * inbuf = sourceStart;
            size_t inbytesleft = (sourceEnd - sourceStart) * sizeof(*sourceStart);
            char * outbuf = reinterpret_cast<char *>(destStart);
            size_t outbytesleft = (destEnd - destStart) * sizeof(*destStart);

            size_t result = (*reinterpret_cast<MyIconv>(iconv))(mConverter, &inbuf, &inbytesleft, &outbuf, &outbytesleft);

            sourceStart = inbuf;
            destStart = reinterpret_cast<UnicodeChar *>(outbuf);

            if ((result == (size_t)-1) && (errno == EILSEQ)) {
                ThrowXMLException(EILSEQ, "Illegal character sequence");
            }
	}
}
