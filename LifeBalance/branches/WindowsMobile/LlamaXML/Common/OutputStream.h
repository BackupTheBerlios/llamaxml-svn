/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaStdInt.h"

namespace LlamaXML {

	class OutputStream {
	public:
		virtual void WriteData(const char * buffer, uint32_t length) = 0;
	};

}

LlamaXML::OutputStream & operator << (LlamaXML::OutputStream & stream,
	const char * s);

#endif
