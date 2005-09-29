/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef FROMUNICODECONVERTER_H
#define FROMUNICODECONVERTER_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "UnicodeString.h"
#include "TextEncoding.h"

namespace LlamaXML {

	/**
		\brief An internal class used by XMLReader to convert text in other
		encodings to Unicode.  You should not need to use this class
		directly.
		
		This class has different implementations on different platforms.
	*/

	class FromUnicodeConverter {
	public:
		FromUnicodeConverter(TextEncoding destinationEncoding);
		~FromUnicodeConverter();
		
		void Convert(const UnicodeChar * & sourceStart, const UnicodeChar * sourceEnd,
			char * & destStart, char * destEnd);
		
		TextEncoding GetDestinationEncoding() const
		{
			return mDestinationEncoding;
		}

	private:
		TextEncoding			mDestinationEncoding;
	};

}

#endif
