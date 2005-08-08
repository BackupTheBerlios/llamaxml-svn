/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef UNICODESTRING_H
#define UNICODESTRING_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include <string>
#include <stdint.h>

namespace LlamaXML {

	/*
		I really which I could think of a simpler way to do this
		than using template specialization.  All I really want is
		a portable way of saying:
		
		if (sizeof(wchar_t) == 2) {
			typedef wchar_t UnicodeChar;
			typedef std::wstring UnicodeString;
		}
		else {
			typedef uint16_t UnicodeChar;
			typedef std::basic_string<uint16_t> UnicodeString;
		}
	*/	

	template <int N>
	struct UnicodeTypeInfo {
		typedef uint16_t UnicodeChar;
		typedef std::basic_string<uint16_t> UnicodeString;
	};
	
	template <>
	struct UnicodeTypeInfo<2> {
		typedef wchar_t UnicodeChar;
		typedef std::wstring UnicodeString;
	};
	
	typedef UnicodeTypeInfo<sizeof(wchar_t)>::UnicodeChar UnicodeChar;
	typedef UnicodeTypeInfo<sizeof(wchar_t)>::UnicodeString UnicodeString;
	
}

#endif
