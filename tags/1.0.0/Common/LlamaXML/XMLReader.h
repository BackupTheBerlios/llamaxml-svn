/*
 * Copyright (C) 2005 Llamagraphics, Inc.
 * 
 * This file is part of LlamaXML.
 *
 * LlamaXML is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * LlamaXML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 *
 * LlamaXML is also available under an alternative commercial license for
 * developers who need the flexibility to include this library in proprietary
 * applications.  Visit http://www.llamagraphics.com/LlamaXML/ for more 
 * information.
 */

#ifndef LLAMAXML_XMLREADER_H
#define LLAMAXML_XMLREADER_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif


#include "LlamaXML/InputStream.h"
#include "LlamaXML/TextEncoding.h"
#include "LlamaXML/UnicodeString.h"
#include "LlamaXML/ConvertToUnicode.h"
#include <stddef.h>
#include <vector>

namespace LlamaXML {

	/**
		\brief The main XML parser class.
		
		To use this class, first construct one of the subclasses of
		InputStream.  Then construct the XMLReader on the InputStream.
		Then call Read() repeatedly to parse the document.  When Read()
		returns false, you have finished parsing the file.
		
		As the reader parses the XML file, it moves through a
		number of states identified by the NodeType returned by
		the GetNodeType function.  The other data available from
		the reader depends on the current NodeType.
		
		\code
	StringInputStream input("<a/>");
	XMLReader reader(input);

	while (reader.Read()) {
	    switch (reader.GetNodeType()) {
	        case kElement:
	            break;
	        case kEndElement:
	            break;
	        case kText:
	            break;
	    }
	}
		\endcode
	*/

	class XMLReader {
	public:
		XMLReader(InputStream & input,
			TextEncoding initialEncoding = TextEncoding::UTF8());
		~XMLReader();

		enum NodeType {
			kAttribute,
			kCDATA,
			kComment,
			kDocument,
			kDocumentFragment,
			kDocumentType,
			kElement,
			kEndElement,
			kEndEntity,
			kEntity,
			kEntityReference,
			kNone,
			kNotation,
			kProcessingInstruction,
			kSignificantWhitespace,
			kText,
			kWhitespace,
			kXmlDeclaration
		};
		
		enum WhitespaceHandling {
		    kWhitespaceHandlingAll,
		    kWhitespaceHandlingNone,
		    kWhitespaceHandlingSignificant
		};

		bool Read();
		NodeType MoveToContent();
		void Skip();
		UnicodeString ReadString();
		std::string ReadString(TextEncoding encoding);
		
		bool IsStartElement();
		bool IsStartElement(const char * name);
		bool IsStartElement(const char * localName, const char * namespaceURI);

		void ReadStartElement();
		void ReadStartElement(const char * name);
		void ReadStartElement(const char * localName, const char * namespaceURI);
		
		bool IsNotEmptyElementRead();
		bool MoveToSubElement();
		
		void ReadEndElement();
		
		UnicodeString ReadElementString();
		UnicodeString ReadElementString(const char * name);
		UnicodeString ReadElementString(const char * localName, const char * namespaceURI);

		std::string ReadElementString(TextEncoding encoding);
		std::string ReadElementString(const char * name, TextEncoding encoding);
		std::string ReadElementString(const char * localName, const char * namespaceURI,
		    TextEncoding encoding);

		bool EndOfFile() const;

		NodeType GetNodeType() const
		{
			return mNodeType;
		}

		const UnicodeString & GetName() const
		{
			return mCurrentName.mName;
		}

		const UnicodeString & GetLocalName() const
		{
			return mCurrentName.mLocalName;
		}

		const UnicodeString & GetValue() const
		{
			return mValue;
		}

		const UnicodeString & GetPrefix() const
		{
			return mCurrentName.mPrefix;
		}

		const UnicodeString & GetNamespaceURI() const
		{
			return mCurrentName.mNamespaceURI;
		}

		bool IsEmptyElement() const
		{
			return mIsEmptyElement;
		}

		bool HasAttributes() const
		{
			return ! mAttributes.empty();
		}

		size_t AttributeCount() const
		{
			return mAttributes.size();
		}
		
		WhitespaceHandling GetWhitespaceHandling() const {
		    return mWhitespaceHandling;
		}
		
		void SetWhitespaceHandling(WhitespaceHandling whitespaceHandling) {
		    mWhitespaceHandling = whitespaceHandling;
		}
		
		bool HasAttribute(size_t i) const;
		bool HasAttribute(const char * name) const;
		bool HasAttribute(const char * localName, const char * namespaceURI) const;

		UnicodeString GetAttribute(size_t i) const;
		UnicodeString GetAttribute(const char * name) const;
		UnicodeString GetAttribute(const char * localName, const char * namespaceURI) const;

		std::string GetAttribute(size_t i, TextEncoding encoding) const;
		std::string GetAttribute(const char * name, TextEncoding encoding) const;
		std::string GetAttribute(const char * localName, const char * namespaceURI,
		    TextEncoding encoding) const;

		static bool IsBaseChar(UnicodeChar c);
		static bool IsIdeographic(UnicodeChar c);
		static bool IsLetter(UnicodeChar c);
		static bool IsDigit(UnicodeChar c);
		static bool IsWhitespace(UnicodeChar c);
		static bool IsCombiningChar(UnicodeChar c);
		static bool IsNameChar(UnicodeChar c);
		static bool IsNameToken(const UnicodeString & token);
		static bool IsName(const UnicodeString & name);
		
		static bool Equals(const UnicodeString & a, const char * b);
		static bool StartsWith(const UnicodeChar * haystack, const char * needle);

	private:
		struct UniCharRange {
			UnicodeChar	mLowChar;
			UnicodeChar	mHighChar;
		};

		static const UniCharRange sBaseCharRanges[];
		static const UniCharRange sCombiningCharRanges[];
		static const UniCharRange sDigitRanges[];

		struct Name {
			void SetName(const char * name);
			void SetName(const UnicodeChar * name);
			void SetName(const UnicodeString & name);
			void DivideName();
			void Clear();
		
			UnicodeString	mName;
			UnicodeString	mPrefix;
			UnicodeString	mLocalName;
			UnicodeString	mNamespaceURI;
		};

		struct Tag: public Name {
			size_t			mPreviousMappingSize;
		};

		struct Attribute: public Name {
			UnicodeString	mValue;
		};

		struct NamespaceMapping {
			UnicodeString	mPrefix;
			UnicodeString	mNamespaceURI;
		};

	private:
		bool ReadInternal();
		void FillInputBuffer();
		void FillOutputBuffer();
		size_t ConvertInput(char * outputBuffer, size_t len);

		bool BufferStartsWith(const char * prefix);
		bool StartsWithWhitespace();

		bool ParseElement();
		bool ParseEndElement();
		bool ParseOptionalWhitespace();
		bool ParseRequiredWhitespace();
		bool ParseXmlDeclaration();
		bool ParseAttribute(Attribute & attribute);
		bool ParseAttValue(UnicodeString & value);
		bool ParseEq();
		bool ParseName(Name & name);
		bool ParseReference(UnicodeString & value);
		bool ParseString(const char * s);
		bool ParseText();

		UnicodeChar PeekChar();
		UnicodeChar ReadChar();
		bool ParseChar(UnicodeChar c);

		void LookupNamespace(Name & name) const;
		size_t PushNamespaces();
		void PopNamespaces(size_t n);
		void PushTag();
		void PopTag();

		static bool IsInRange(UnicodeChar c, const UniCharRange ranges[],
			size_t n);

	private:
		static UnicodeString	sEmptyUniCharString;
		
		static const size_t kInputBufferCount = 512;
		static const size_t kOutputBufferCount = 512;
		
		WhitespaceHandling              mWhitespaceHandling;

		InputStream &					mInput;
		TextEncoding					mDocumentEncoding;
		char *							mInputBuffer;
		char *							mInputStart;
		char *							mInputEnd;
		UnicodeChar	*					mOutputBuffer;
		UnicodeChar *					mOutputStart;
		UnicodeChar *					mOutputEnd;

		NodeType						mNodeType;
		Name							mCurrentName;
		UnicodeString					mValue;
		std::vector<Attribute>			mAttributes;
		std::vector<NamespaceMapping>	mNamespaceMappings;
		std::vector<Tag>				mOpenTags;
		bool							mIsEmptyElement;

		ConvertToUnicode				mConverter;
	};

}


#endif
