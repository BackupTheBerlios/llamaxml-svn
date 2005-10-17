/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef LLAMAXML_FILEOUTPUTSTREAM_H
#define LLAMAXML_FILEOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#include "LlamaXML/OutputStream.h"
#include "LlamaXML/PlatformConfig.h"

namespace LlamaXML {
	class FileOutputStream: public OutputStream {
	public:
		FileOutputStream(const wchar_t * path);
		virtual ~FileOutputStream();

		virtual void WriteData(const char * buffer, uint32_t length);

	private:
		HANDLE	mFile;
	};
}

#endif
