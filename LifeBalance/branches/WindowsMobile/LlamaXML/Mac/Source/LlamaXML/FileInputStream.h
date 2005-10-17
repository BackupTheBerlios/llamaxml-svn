/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/InputStream.h"
#include "LlamaXML/PlatformConfig.h"


namespace LlamaXML {
    
    class FileInputStream: public InputStream {
    public:
        FileInputStream(const FSRef * fileRef);
        FileInputStream(const char * posixPath);
	    virtual ~FileInputStream();
	    
		/**
			Fills the buffer with up to length chars, and returns
			the number of chars placed in the buffer.
		*/
		virtual uint32_t ReadUpTo(char * buffer, uint32_t length);
		
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
		virtual void Restart();

		/**
			Returns true if the stream is currently positioned at the
			end of input.
		*/
		virtual bool EndOfFile();
	
	private:
	    SInt16 mRefnum;
    };

}

#endif
