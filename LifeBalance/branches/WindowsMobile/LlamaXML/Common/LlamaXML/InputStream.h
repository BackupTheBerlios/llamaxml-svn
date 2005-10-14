/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#include "LlamaXML/LlamaStdInt.h"

namespace LlamaXML {

	/**
		Abstract base class for a source of XML text.
		
		You can't use this class directly.  Use one of the provided
		subclasses like StringInputStream or BufferInputStream instead.
		
		You can also create your own subclasses of InputStream for data
		sources that are not directly supported by the LlamaXML library.
	*/
	
	class InputStream {
	public:
	    virtual ~InputStream();
	    
		/**
			Fills the buffer with up to length chars, and returns
			the number of chars placed in the buffer.
		*/
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length) = 0;
		
		/**
			Repositions the stream to the beginning of the input.
			This function is called at most once by XMLReader, and
			only when the reader is in the first buffer of text.
			
			(Implementer's note: This function could probably be
			eliminated by more clever programming in the XMLReader
			class.  This would make it easier to implement when data
			is coming from a source that cannot be backed up, like a
			network socket. 
		*/
		virtual void Restart() = 0;

		/**
			Returns true if the stream is currently positioned at the
			end of input.
		*/
		virtual bool EndOfFile() = 0;
	};
	
}

#endif
