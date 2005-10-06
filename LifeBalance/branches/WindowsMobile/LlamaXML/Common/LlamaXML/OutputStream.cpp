/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/OutputStream.h"
#include <string.h>

namespace LlamaXML {
    
    OutputStream::~OutputStream() {
    }
    
}

LlamaXML::OutputStream & operator << (LlamaXML::OutputStream & stream,
	const char * s) {
	stream.WriteData(s, strlen(s));
	return stream;
}
