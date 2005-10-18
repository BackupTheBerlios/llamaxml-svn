/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef LLAMAXML_CONVERTSTRING_H
#define LLAMAXML_CONVERTSTRING_H

#if (! __GNUC__) || __APPLE__
#pragma once
#endif

#include "LlamaXML/UnicodeString.h"
#include "LlamaXML/TextEncoding.h"
#include "LlamaXML/LlamaStdInt.h"
#include <string>

namespace LlamaXML {
	
	size_t UnicodeStringLength(const UnicodeChar * s);
	
	UnicodeString ToUnicode(const char * start, const char * end, TextEncoding sourceEncoding);
	UnicodeString ToUnicode(const char * s, TextEncoding sourceEncoding);
	UnicodeString ToUnicode(const std::string & s, TextEncoding sourceEncoding);
	
	std::string FromUnicode(const UnicodeChar * start, const UnicodeChar * end, TextEncoding destinationEncoding);
	std::string FromUnicode(const UnicodeChar * s, TextEncoding destinationEncoding);
	std::string FromUnicode(const UnicodeString & s, TextEncoding destinationEncoding);

}

#endif
