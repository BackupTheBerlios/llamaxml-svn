/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef CONVERTTOUNICODE_H
#define CONVERTTOUNICODE_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <PalmOS.h>
#include "UnicodeString.h"
#include "TextEncoding.h"

namespace LlamaXML {

	/**
		\brief An internal class used by XMLReader to convert text in other
		encodings to Unicode.  You should not need to use this class
		directly.
		
		This class has different implementations on different platforms.
	*/

	class ConvertToUnicode {
	public:
		ConvertToUnicode(TextEncoding sourceEncoding);

		void Reset(TextEncoding sourceEncoding);
		
		void Convert(char * & sourceStart,
			char * sourceEnd, UnicodeChar * & destStart,
			UnicodeChar * destEnd);
		
		TextEncoding GetSourceEncoding() const
		{
			return mSourceEncoding;
		}

	private:
		TextEncoding			mSourceEncoding;
		Boolean					mNewConversion;
		TxtConvertStateType		mState;
	};

}

#endif
