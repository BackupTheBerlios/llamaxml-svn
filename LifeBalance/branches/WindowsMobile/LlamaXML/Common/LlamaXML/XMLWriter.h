/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#ifndef XMLWRITER_H
#define XMLWRITER_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/OutputStream.h"
#include "LlamaXML/TextEncoding.h"
#include "LlamaXML/ConvertToUnicode.h"
#include "LlamaXML/ConvertFromUnicode.h"
#include <vector>
#include <string>

namespace LlamaXML {

	class XMLWriter {
	public:
		XMLWriter(OutputStream & output, TextEncoding applicationEncoding);
		~XMLWriter();
		
		void StartDocument(const char * version,
                           const char * encoding,
                           const char * standalone);
        void EndDocument();
        
        void StartElement(const char * name);
        void StartElement(const char * prefix, const char * name,
        	const char * namespaceURI = 0);
        void EndElement();

		void WriteElement(const char * name);
        
        template <class T>
        void WriteElement(const char * name, const T & value)
		{
			StartElement(name);
			*this << value;
			EndElement();
		}
        
        void WriteString(const char * content);
		void WriteString(const UnicodeChar * content);
		void WriteString(const UnicodeString & content);
        
        void StartAttribute(const char * name);
        void StartAttribute(const char * prefix, const char * name,
                            const char * namespaceURI = 0);
        void EndAttribute();
        
        template <class T>
        void WriteAttribute(const char * name, const T & value)
		{
			StartAttribute(name);
			*this << value;
			EndAttribute();
		}
    
    private:
		static const int kBufferSize = 256;

    	enum State {
    		kStateNormal,
    		kStateOpenTag,
    		kStateOpenAttribute
    	};

		static const char * Scan(const char * content, const char * tokens);
		static const UnicodeChar * Scan(const UnicodeChar * content, const UnicodeChar * contentEnd, const char * tokens);

		static const char * StringEnd(const char * s);
		static const UnicodeChar * StringEnd(const UnicodeChar * s);

		void WriteRawUnicode(const UnicodeChar * unicodeStart, const UnicodeChar * unicodeEnd);
		void WriteApplicationContent(const char * content);
		void WriteUTF8Content(const char * content);
		void WriteUnicodeContent(const UnicodeChar * content, const UnicodeChar * contentEnd);

	private:
		static const char *			kNewline;
		static const char *			kIndent;
		static const char *			kAmpersand;
		static const char *			kLessThan;
		static const char *			kGreaterThan;
		static const char *			kQuote;

		OutputStream &				mOutput;
		State						mState;
		std::vector<std::string>	mElementStack;
		size_t						mIndentLevel;
		ConvertToUnicode			mApplicationToUnicode;
		ConvertFromUnicode		mUnicodeToUTF8;
	};

}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output, const char * s);
LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output, unsigned long long n);
LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output, int n);
LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output, const LlamaXML::UnicodeString & s);
LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output, double n);

#endif
