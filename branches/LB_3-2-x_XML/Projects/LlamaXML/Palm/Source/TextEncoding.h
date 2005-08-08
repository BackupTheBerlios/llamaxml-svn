/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef TEXTENCODING_H
#define TEXTENCODING_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include "UnicodeString.h"

namespace LlamaXML {

	/**
		\brief An internal class that XMLReader uses to represent a text
		encoding.  You should not have to use this class in your
		own code.
		
		This class is implemented differently on different platforms.
	*/

	class TextEncoding {
	public:
		TextEncoding();
		
		CharEncodingType AsPalmCharEncoding() const
		{
			return mEncoding;
		}
		
		bool operator == (TextEncoding other) const
		{
			return mEncoding == other.mEncoding;
		}
		
		bool operator != (TextEncoding other) const
		{
			return mEncoding != other.mEncoding;
		}
		
		static TextEncoding System();
		static TextEncoding Application();
		
		static TextEncoding Palm(CharEncodingType encoding);
		static TextEncoding UTF8();
		static TextEncoding UTF16LE();
		static TextEncoding UTF16BE();
		
		static TextEncoding WebCharset(const UnicodeChar * name);
	
	private:
		explicit TextEncoding(CharEncodingType encoding);
		
	private:
		CharEncodingType	mEncoding;
	};
	
	
	inline TextEncoding::TextEncoding()
	: mEncoding(charEncodingPalmLatin)
	{
	}
	
	
	inline TextEncoding::TextEncoding(CharEncodingType encoding)
	: mEncoding(encoding)
	{
	}
	
	
	inline TextEncoding TextEncoding::Palm(CharEncodingType encoding)
	{
		return TextEncoding(encoding);
	}
	
	
	inline TextEncoding TextEncoding::Application()
	{
		return System();
	}
	
	
	inline TextEncoding TextEncoding::UTF8()
	{
		return TextEncoding(charEncodingUTF8);
	}
	
	
	inline TextEncoding TextEncoding::UTF16LE()
	{
		return TextEncoding(charEncodingUTF16LE);
	}
	
	
	inline TextEncoding TextEncoding::UTF16BE()
	{
		return TextEncoding(charEncodingUTF16BE);
	}
}

#endif