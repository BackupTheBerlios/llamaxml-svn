/*
 *  TestTextEncoding.cpp
 *  LlamaXML
 *
 *  Created by Stuart A. Malone on 12/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestTextEncoding.h"
#include "LlamaXML/TextEncoding.h"
#include <vector>

using namespace std;

static LlamaXML::TextEncoding GetTextEncoding(const char * name) {
	LlamaXML::UnicodeChar unicodeName[256];
	
	for (LlamaXML::UnicodeChar * p = unicodeName; true; ++p, ++name) {
		*p = *name;
		if (*name == 0) break;
	}
	
	return LlamaXML::TextEncoding::WebCharset(unicodeName);
}


void TestTextEncoding(Tester & tester) {
	using LlamaXML::TextEncoding;
	
	// Make sure that the TextEncoding generated by the empty constructor is undefined.
	TextEncoding undefined;
	TestAssert(! undefined.IsAvailable());
	
	// Make sure that the System and Application encodings are defined.
	TestAssert(TextEncoding::System().IsAvailable());
	TestAssert(TextEncoding::Application().IsAvailable());
	
	// Make sure that the most common encodings are available.
	TestAssert(TextEncoding::WindowsLatin1().IsAvailable());
	TestAssert(TextEncoding::ISOLatin1().IsAvailable());
	TestAssert(TextEncoding::UTF8().IsAvailable());
	
	// Make sure that each of these encodings is equal to itself, and not equal
	// to any other encoding.
	vector<TextEncoding> encodings;
	encodings.push_back(TextEncoding());
	encodings.push_back(TextEncoding::ISOLatin1());
	encodings.push_back(TextEncoding::UTF8());
	encodings.push_back(TextEncoding::UTF16());
	
	// Because the only difference between UCS-2 and UTF-16 is the handling of surrogate pairs,
	// and because not all platforms support this difference, we allow UTF-16 and UCS-2 to map
	// to the same encoding, so we don't put UCS-2 in the list of test encodings.
	// See http://en.wikipedia.org/wiki/UTF-16 for more information.
	
	// encodings.push_back(TextEncoding::UCS2());
	
	// These encodings are not required to be supported, but if they are, they should
	// be distinct from any other encoding.
	if (TextEncoding::ShiftJIS().IsAvailable()) {
		encodings.push_back(TextEncoding::ShiftJIS());
	}
	if (TextEncoding::WindowsLatin1().IsAvailable()) {
		encodings.push_back(TextEncoding::WindowsLatin1());
	}
	
	for (size_t i = 0; i < encodings.size(); ++i) {
		for (size_t j = 0; j < encodings.size(); ++j) {
			if (i == j) {
				TestAssert(encodings[i] == encodings[j]);
				TestAssert(! (encodings[i] != encodings[j]));
			}
			else {
				TestAssert(encodings[i] != encodings[j]);
				TestAssert(! (encodings[i] == encodings[j]));
			}
		}
	}
	
	// Make sure that common encoding names map to the correct encoding, and that
	// the names are case-insensitive.
	// More information about how XML handles encoding names is available from
	// http://www.w3.org/TR/2004/REC-xml-20040204/#charencoding
	TestAssert(GetTextEncoding("UTF-8") == TextEncoding::UTF8());
	TestAssert(GetTextEncoding("utf-8") == TextEncoding::UTF8());
	TestAssert(GetTextEncoding("UTF-16") == TextEncoding::UTF16());
	TestAssert(GetTextEncoding("utf-16") == TextEncoding::UTF16());
	TestAssert(GetTextEncoding("ISO-10646-UCS-2") == TextEncoding::UCS2());
	TestAssert(GetTextEncoding("iso-10646-ucs-2") == TextEncoding::UCS2());
	TestAssert(GetTextEncoding("ISO-8859-1") == TextEncoding::ISOLatin1());
	TestAssert(GetTextEncoding("iso-8859-1") == TextEncoding::ISOLatin1());
	if (TextEncoding::ShiftJIS().IsAvailable()) {
		TestAssert(GetTextEncoding("Shift_JIS") == TextEncoding::ShiftJIS());
		TestAssert(GetTextEncoding("shift_jis") == TextEncoding::ShiftJIS());
	}
}
