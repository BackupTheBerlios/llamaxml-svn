/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef CONVERTFROMUNICODE_H
#define CONVERTFROMUNICODE_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/UnicodeString.h"
#include "LlamaXML/TextEncoding.h"

namespace LlamaXML {

	/**
		\brief An internal class used by XMLReader to convert text in other
		encodings to Unicode.  You should not need to use this class
		directly.
		
		This class has different implementations on different platforms.
	*/

	class ConvertFromUnicode {
	public:
		ConvertFromUnicode(TextEncoding destinationEncoding);
		
		void Convert(const UnicodeChar * & sourceStart, const UnicodeChar * sourceEnd,
			char * & destStart, char * destEnd);
		
		TextEncoding GetDestinationEncoding() const
		{
			return mDestinationEncoding;
		}

	private:
		TextEncoding			mDestinationEncoding;
		Boolean					mNewConversion;
		TxtConvertStateType		mState;
	};

}

#endif
