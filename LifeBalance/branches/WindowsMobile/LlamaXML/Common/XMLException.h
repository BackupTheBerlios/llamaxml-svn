/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef XMLEXCEPTION_H
#define XMLEXCEPTION_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#include <exception>
#include "LlamaStdInt.h"

namespace LlamaXML {

	class XMLException: public std::exception
	{
	public:
		XMLException(int32_t err, const char * file = 0, long line = 0) throw();
		
		int32_t ErrorCode() const {
			return mErrorCode;
		}
		
		const char * File() const {
			return mFile;
		}
		
		long Line() const {
			return mLine;
		}

	private:
		int32_t					mErrorCode;
		const char *			mFile;
		long					mLine;
	};

	void ThrowXMLException(int32_t err, const char * file, long line);
}

#define ThrowXMLError(x) LlamaXML::ThrowXMLException(x, __FILE__, __LINE__)
#if TARGET_OS_WIN32
	#define ThrowIfXMLError(x) do {HRESULT e = x; if (FAILED(e)) LlamaXML::ThrowXMLException(e, __FILE__, __LINE__);} while(0)
#else
	#define ThrowIfXMLError(x) do {int32_t e = x; if (e != 0) LlamaXML::ThrowXMLException(e, __FILE__, __LINE__);} while(0)
#endif

#endif
