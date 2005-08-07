#include "OutputStream.h"
#include <string.h>

LlamaXML::OutputStream & operator << (LlamaXML::OutputStream & stream,
	const char * s) {
	stream.WriteData(s, strlen(s));
	return stream;
}
