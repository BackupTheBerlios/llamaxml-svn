#include "DataOutputStream.h"
#include <string.h>

LlamaXML::DataOutputStream & operator << (LlamaXML::DataOutputStream & stream,
	const char * s) {
	stream.WriteData(s, strlen(s));
	return stream;
}
