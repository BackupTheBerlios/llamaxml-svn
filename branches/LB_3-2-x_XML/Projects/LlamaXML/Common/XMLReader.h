#ifndef LLAMAXMLREADER_H
#define LLAMAXMLREADER_H

#pragma once

#include "InputStream.h"
#include "TextEncoding.h"
#include "UnicodeString.h"
#include "ToUnicodeConverter.h"
#include <stddef.h>
#include <vector>

namespace LlamaXML {

	class XMLReader {
	public:
		XMLReader(InputStream & input);
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

		bool Read();
		NodeType MoveToContent();
		void Skip();

		bool EndOfFile() const;

		const NodeType & GetNodeType() const
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

		UnicodeString GetAttribute(size_t i) const;
		UnicodeString GetAttribute(const char * name) const;
		UnicodeString GetAttribute(const char * localName, const char * namespaceURI) const;

		static bool IsBaseChar(UnicodeChar c);
		static bool IsIdeographic(UnicodeChar c);
		static bool IsLetter(UnicodeChar c);
		static bool IsDigit(UnicodeChar c);
		static bool IsWhitespace(UnicodeChar c);
		static bool IsCombiningChar(UnicodeChar c);
		static bool IsNameChar(UnicodeChar c);
		static bool IsNameToken(const UnicodeString & token);
		static bool IsName(const UnicodeString & name);

	private:
		struct UniCharRange {
			UnicodeChar	mLowChar;
			UnicodeChar	mHighChar;
		};

		static const UniCharRange sBaseCharRanges[];
		static const UniCharRange sCombiningCharRanges[];
		static const UniCharRange sDigitRanges[];

		struct Name {
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
		void FillInputBuffer();
		void FillOutputBuffer();
		size_t ConvertInput(char * outputBuffer, size_t len);

		bool StartsWith(const char * prefix);
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
		static bool Equals(const UnicodeString & a, const char * b);

	private:
		static UnicodeString	sEmptyUniCharString;
		
		static const size_t kInputBufferCount = 512;
		static const size_t kOutputBufferCount = 512;

		InputStream &				mInput;
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

		ToUnicodeConverter				mConverter;
	};

}


#endif
