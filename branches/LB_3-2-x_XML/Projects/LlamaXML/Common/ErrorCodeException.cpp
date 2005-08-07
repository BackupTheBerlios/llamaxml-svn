#include "ErrorCodeException.h"

namespace LlamaXML {

	ErrorCodeException::ErrorCodeException(int32_t err, const char * file, long line) throw()
	: mErrorCode(err),
	  mFile(file),
	  mLine(line)
	{
	}


	void ThrowErrorCodeException(int32_t err, const char * file, long line)
	{
		throw ErrorCodeException(err, file, line);
	}

}
