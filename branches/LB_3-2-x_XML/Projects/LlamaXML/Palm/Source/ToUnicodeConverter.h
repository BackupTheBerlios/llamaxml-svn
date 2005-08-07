#pragma once

#include "UnicodeString.h"
#include "TextEncoding.h"

namespace LlamaXML {

	class ToUnicodeConverter {
	public:
		ToUnicodeConverter(TextEncoding sourceEncoding);

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
