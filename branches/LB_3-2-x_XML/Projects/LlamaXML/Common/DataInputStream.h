#ifndef DATAINPUTSTREAM_H
#define DATAINPUTSTREAM_H

#pragma once

#include <stdint.h>

namespace LlamaXML {

	// This abstract class represents the source of the XML data.
	// It should be subclassed to read data from a particular source,
	// such as a file or network socket.
	
	// If this were Java, this would be an interface rather than a class.
	
	class DataInputStream {
	public:
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length) = 0;
		virtual void Restart() = 0;
		virtual bool EndOfFile() const = 0;
	};
	
}

#endif
