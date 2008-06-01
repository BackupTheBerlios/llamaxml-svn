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

#pragma once

#include "LlamaXML/PlatformConfig.h"
#include "LlamaXML/UnicodeString.h"

namespace LlamaXML {

	/// Specifies a character encoding for conversions to or from Unicode.
	
	/// TextEncoding objects are small objects that are normally passed by
	/// value rather than by reference.
	/// To create a TextEncoding object, call one of the static factory functions
	/// rather than calling the constructor directly.

	class TextEncoding {
	public:
		/// Creates an undefined TextEncoding object.

		/// Be sure to assign a valid TextEncoding object to this object
		/// before passing it to any other parts of the library.
		TextEncoding();
		
		/// Returns true if conversion to/from Unicode is available on this system.
		bool IsAvailable() const;
		
		// LlamaPlatform::String Name() const;
		
		/// Returns true if the two objects refer to the same encoding.
		bool operator == (const TextEncoding & other) const;
		
		/// Returnts true if the two objects refer to different encodings.
		bool operator != (const TextEncoding & other) const;
		
		/// Returns the Macintosh TextEncoding for the encoding. (Mac OS X only)
		operator ::TextEncoding() const;
		
		/// Returns the Macintosh TextEncoding for the encoding. (Mac OS X only)
		::TextEncoding AsMacTextEncoding() const;
		
		static TextEncoding System();
		static TextEncoding Application();
		
		static TextEncoding WindowsLatin1();
		static TextEncoding PalmLatin1();
		static TextEncoding ISOLatin1();
		static TextEncoding ASCII();
		static TextEncoding WindowsShiftJIS();
		static TextEncoding PalmShiftJIS();
		static TextEncoding ShiftJIS();
		static TextEncoding UCS2();
		static TextEncoding UTF7();
		static TextEncoding UTF8();
		static TextEncoding UTF16();
		static TextEncoding UTF16BE();
		static TextEncoding UTF16LE();
		static TextEncoding UTF32();
		static TextEncoding UTF32BE();
		static TextEncoding UTF32LE();
		
		/// Returns the TextEncoding corresponding to the specified Mac TextEncoding. (Mac OS X only)
		static TextEncoding Macintosh(::TextEncoding encoding);
		// static TextEncoding Palm(UInt8 encoding);

		/// Returns the TextEncoding identified by the XML encoding name.
		static TextEncoding WebCharset(const UnicodeChar * name);
	
	private:
		explicit TextEncoding(::TextEncoding encoding);
		TextEncoding(TextEncodingBase base, TextEncodingVariant variant,
			TextEncodingFormat format);
		
		static TextEncoding Fallback(TextEncoding a, TextEncoding b);
		static TextEncoding Fallback(TextEncoding a, TextEncoding b, TextEncoding c);
		static TextEncoding Fallback(TextEncoding a, TextEncoding b, TextEncoding c,
			TextEncoding d);
		
		TextEncoding(TextEncodingBase base1, TextEncodingVariant variant1,
			TextEncodingFormat format1, TextEncodingBase base2, TextEncodingVariant variant2,
			TextEncodingFormat format2);
		
		static bool EqualIgnoringCase(const UnicodeChar * a, const char * b);
	
	private:
		::TextEncoding	mEncoding;
	};
	
	
	inline TextEncoding::TextEncoding(::TextEncoding encoding)
	: mEncoding(encoding)
	{
	}
	
	
	inline bool TextEncoding::operator == (const TextEncoding & other) const
	{
		return mEncoding == other.mEncoding;
	}
	
	
	inline bool TextEncoding::operator != (const TextEncoding & other) const
	{
		return mEncoding != other.mEncoding;
	}
	
	
	inline TextEncoding::operator ::TextEncoding() const
	{
		return mEncoding;
	}
	
	
	inline ::TextEncoding TextEncoding::AsMacTextEncoding() const
	{
		return mEncoding;
	}

}
