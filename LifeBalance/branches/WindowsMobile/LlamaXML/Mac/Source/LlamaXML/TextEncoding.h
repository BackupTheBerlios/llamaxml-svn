#pragma once

#include "LlamaXML/UnicodeString.h"
#include "LlamaXML/PlatformConfig.h"

namespace LlamaXML {

	class TextEncoding {
	public:
		TextEncoding();
		
		// Returns true if conversion to/from Unicode is available on this system.
		bool IsAvailable() const;
		
		// LlamaPlatform::String Name() const;
		
		bool operator == (const TextEncoding & other) const;
		bool operator != (const TextEncoding & other) const;
		
		operator ::TextEncoding() const;
		
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
		static TextEncoding Macintosh(::TextEncoding encoding);
		// static TextEncoding Palm(UInt8 encoding);

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
