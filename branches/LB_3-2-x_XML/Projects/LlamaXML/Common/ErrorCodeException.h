#ifndef ERRORCODEEXCEPTION_H
#define ERRORCODEEXCEPTION_H

#pragma once

#include <exception>
#include <stdint.h>

namespace LlamaXML {

	class ErrorCodeException: public std::exception
	{
	public:
		ErrorCodeException(int32_t err, const char * file = 0, long line = 0) throw();
		
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

	void ThrowErrorCodeException(int32_t err, const char * file, long line);
}

#define ThrowError(x) LlamaXML::ThrowErrorCodeException(x, __FILE__, __LINE__)
#if TARGET_OS_WIN32
	#define ThrowIfError(x) do {HRESULT e = x; if (FAILED(e)) LlamaXML::ThrowErrorCodeException(e, __FILE__, __LINE__);} while(0)
#else
	#define ThrowIfError(x) do {int32_t e = x; if (e != 0) LlamaXML::ThrowErrorCodeException(e, __FILE__, __LINE__);} while(0)
#endif

#endif
