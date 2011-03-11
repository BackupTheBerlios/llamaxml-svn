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

#include "LlamaXML/XMLReader.h"
#include "LlamaXML/XMLException.h"
#include "LlamaXML/ConvertString.h"
#include <stddef.h>

#define countof(x) (sizeof(x) / sizeof(*x))

namespace LlamaXML {

	XMLReader::SkippingContent::SkippingContent(XMLReader & reader)
	: mReader(reader),
	  mWasSkippingContent(reader.mSkipContent)
	{
		mReader.mSkipContent = true;
	}
	
	XMLReader::SkippingContent::~SkippingContent()
	{
		mReader.mSkipContent = mWasSkippingContent;
	}
	
	void XMLReader::Name::SetName(const char * name) {
		// Perform a crude conversion from ASCII to Unicode
		while (*name) {
			mName += UnicodeChar(*name++);
		}
	}

	void XMLReader::Name::SetName(const UnicodeChar * name)
	{
		mName = name;
		DivideName();
	}

	void XMLReader::Name::SetName(const UnicodeString & name)
	{
		mName = name;
		DivideName();
	}

	void XMLReader::Name::DivideName()
	{
		mNamespaceURI.clear();
		UnicodeString::size_type i = mName.find(UnicodeChar(':'));
		if (i == UnicodeString::npos) {
			mPrefix.clear();
			mLocalName = mName;
		}
		else {
			mPrefix = UnicodeString(mName, 0, i);
			mLocalName = UnicodeString(mName, i + 1);
		}
	}

	void XMLReader::Name::Clear()
	{
		mName.clear();
		mNamespaceURI.clear();
		mPrefix.clear();
		mLocalName.clear();
	}


	XMLReader::XMLReader(InputStream & input, TextEncoding initialEncoding)
	: mWhitespaceHandling(kWhitespaceHandlingAll),
	  mInput(input),
	  mInputBuffer(new char[kInputBufferCount]),
	  mInputStart(mInputBuffer),
	  mInputEnd(mInputBuffer),
	  mOutputBuffer(new UnicodeChar[kOutputBufferCount]),
	  mOutputStart(mOutputBuffer),
	  mOutputEnd(mOutputBuffer),
	  mNodeType(kNone),
	  mConverter(initialEncoding),
	  mSkipContent(false)
	{
	}
	
	
	XMLReader::~XMLReader()
	{
		delete [] mInputBuffer;
		delete [] mOutputBuffer;
	}
	
	
	bool XMLReader::Read() {
	    if (mWhitespaceHandling == kWhitespaceHandlingNone) {
	        while (ReadInternal()) {
	            if (mNodeType != kWhitespace) {
	                return true;
	            }
	        }
	        return false;
	    }
	    else {
	        return ReadInternal();
	    }
	}


	// Returns true if a node was read successfully, false on EOF
	bool XMLReader::ReadInternal()
	{
		switch (mNodeType) {
			case kNone:
				FillInputBuffer();
				if ((mInputEnd - mInputStart) >= 2) {
					// We have to be careful here in case char is signed.
					uint16_t x = (uint16_t(uint8_t(mInputBuffer[0])) << 8) | uint8_t(mInputBuffer[1]);
					switch (x) {
						case uint16_t(0xFEFF):
						case uint16_t(0x003C):
							mConverter.Reset(TextEncoding::UTF16BE());
							break;
						case uint16_t(0xFFFE):
						case uint16_t(0x3C00):
							mConverter.Reset(TextEncoding::UTF16LE());
							break;
					}
				}
				mNodeType = kDocument;
				return true;
			
			case kDocument:
				// An XML document can start with:
				// document	::= prolog element Misc*
				// prolog	::= XMLDecl? Misc* (doctypedecl Misc*)?
				// XMLDecl	::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
				// Misc ::= Comment | PI | S
				// doctypedecl ::= '<!DOCTYPE' S Name (S ExternalID)? S? ('[' intSubset ']' S?)? '>'
				// Comment ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
				// PI ::= '<?' PITarget (S (Char* - (Char* '?>' Char*)))? '?>'
				// S ::= (#x20 | #x9 | #xD | #xA)+

				// If the XML file starts with a byte order mark, throw it away.
				// The earlier code for the kNone case has already used it to
				// set the default encoding.
				ParseChar(UnicodeChar(0xFEFF));

				if (BufferStartsWith("<?xml")) {
					if (! ParseXmlDeclaration()) return false;
					UnicodeString encodingName = GetAttribute("encoding");
					if (encodingName.empty()) return true;
					TextEncoding newEncoding = TextEncoding::WebCharset(encodingName.c_str());
					if (newEncoding == mConverter.GetSourceEncoding()) return true;

					// The encoding in the XML declaration is different from the one
					// we assumed, so we have to reset all the input buffering and
					// re-parse the XmlDeclaration.
					mConverter.Reset(newEncoding);
					mInput.Restart();
					mInputStart = mInputEnd = mInputBuffer;
					mOutputStart = mOutputEnd = mOutputBuffer;
					ParseChar(UnicodeChar(0xFEFF));
					return ParseXmlDeclaration();
				}
				else if (StartsWithWhitespace()) return ParseRequiredWhitespace();
				//else if (BufferStartsWith("<!--")) return ParseComment();
				//else if (BufferStartsWith("<?")) return ParseProcessingInstruction();
				//else if (BufferStartsWith("<!DOCTYPE")) return ParseDocumentType();
				else if (BufferStartsWith("<")) return ParseElement();
				else return false;

			case kXmlDeclaration:
			case kElement:
			case kEndElement:
			case kText:
		    case kWhitespace:
			default:
				// The default case handles several node types that we declare but never use.
				// If we start parsing these node types then they should be handled explicitly.
				if (BufferStartsWith("</")) return ParseEndElement();
				else if (BufferStartsWith("<")) return ParseElement();
				else return ParseText();
		}
		return false;
	}
	
	
	XMLReader::NodeType XMLReader::MoveToContent() {
	    // (non-white space text, CDATA, Element, EndElement, EntityReference, or EndEntity)
	    do {
#if 0
	        // We don't support MoveToAttribute, so we don't need this code yet.
            if (mNodeType == kAttribute) {
                MoveToElement();
                return mNodeType;
            }
            else
#endif
            if ((mNodeType == kText) || (mNodeType == kCDATA) || (mNodeType == kElement)
                    || (mNodeType == kEndElement) || (mNodeType == kEntityReference)
                    || (mNodeType == kEndEntity)) {
                return mNodeType;
            }
	    } while (Read());
        return mNodeType;
	}
	
	
	bool XMLReader::IsStartElement() {
	    return MoveToContent() == kElement;
	}
	
	
	bool XMLReader::IsStartElement(const char * name) {
	    return (MoveToContent() == kElement) && Equals(GetName(), name);
	}
	
	
	bool XMLReader::IsStartElement(const char * localName, const char * namespaceURI) {
	    return (MoveToContent() == kElement)
	        && Equals(GetLocalName(), localName)
	        && Equals(GetNamespaceURI(), namespaceURI);
	}
	
	
	void XMLReader::ReadStartElement() {
	    if (IsStartElement()) {
	        Read();
	    }
	    else {
	        ThrowXMLError(0);
	    }
	}
	
	
	void XMLReader::ReadStartElement(const char * name) {
	    if (IsStartElement(name)) {
	        Read();
	    }
	    else {
	        ThrowXMLError(0);
	    }
	}
	
	
	void XMLReader::ReadStartElement(const char * localName, const char * namespaceURI) {
	    if (IsStartElement(localName, namespaceURI)) {
	        Read();
	    }
	    else {
	        ThrowXMLError(0);
	    }
	}
	
	
	bool XMLReader::IsNotEmptyElementRead() {
	    bool result = ! IsEmptyElement();
	    Read();
	    return result;
	}
	
	bool XMLReader::IsOpenElementRead() {
		if (IsStartElement() && ! IsEmptyElement()) {
			Read();
			return true;
		}
		else return false;
	}
	
	bool XMLReader::IsOpenElementRead(const char * name) {
		if (IsStartElement(name) && ! IsEmptyElement()) {
			Read();
			return true;
		}
		else return false;
	}
	
	bool XMLReader::IsOpenElementRead(const char * localName, const char * namespaceURI) {
		if (IsStartElement(localName, namespaceURI) && ! IsEmptyElement()) {
			Read();
			return true;
		}
		else return false;
	}
	
	
	bool XMLReader::MoveToSubElement() {
	    while (true) {
	        switch (MoveToContent()) {
	            case kElement:
	                return true;
	            case kEndElement:
	                Read();
	                return false;
	            default:
	                Skip();
	                break;
	        }
	    }
	}
	
	
	void XMLReader::ReadEndElement() {
	    if (MoveToContent() == kEndElement) {
	        Read();
	    }
	    else {
	        ThrowXMLError(0);
	    }
	}
	
	
	UnicodeString XMLReader::ReadString() {
	    if (mNodeType == kElement) {
	        if (IsEmptyElement()) return UnicodeString();
	        Read();
	    }
	    UnicodeString result;
	    while ((mNodeType == kText) || (mNodeType == kWhitespace) ||
	           (mNodeType == kSignificantWhitespace) || (mNodeType == kCDATA)) {
	        result += mValue;
	        Read();
        }
        return result;
	}
	
	
	std::string XMLReader::ReadString(TextEncoding encoding) {
	    return FromUnicode(ReadString(), encoding);
	}
	
	
    UnicodeString XMLReader::ReadElementString() {
        if (! IsStartElement()) {
            ThrowXMLError(0);
        }
        return ReadString();
    }
    
    
	UnicodeString XMLReader::ReadElementString(const char * name) {
        if (! IsStartElement(name)) {
            ThrowXMLError(0);
        }
        return ReadString();
	}
	
	
	UnicodeString XMLReader::ReadElementString(const char * localName, const char * namespaceURI) {
        if (! IsStartElement(localName, namespaceURI)) {
            ThrowXMLError(0);
        }
        return ReadString();
	}

	
    std::string XMLReader::ReadElementString(TextEncoding encoding) {
        return FromUnicode(ReadElementString(), encoding);
    }
    
    
	std::string XMLReader::ReadElementString(const char * name, TextEncoding encoding) {
	    return FromUnicode(ReadElementString(name), encoding);
	}
	
	
	std::string XMLReader::ReadElementString(const char * localName, const char * namespaceURI,
	        TextEncoding encoding) {
	    return FromUnicode(ReadElementString(localName, namespaceURI), encoding);
	}

	
	void XMLReader::Skip() {
		SkippingContent skipping(*this);
        int depth = 0;
        do {
            if ((mNodeType == kElement) && (! mIsEmptyElement)) {
                ++depth;
            }
            else if (mNodeType == kEndElement) {
                --depth;
            }
            Read();
        } while (depth > 0);
	}


	bool XMLReader::EndOfFile() const
	{
		return (mOutputStart == mOutputEnd) &&
			(mInputStart == mInputEnd) &&
			mInput.EndOfFile();
	}


	UnicodeChar XMLReader::PeekChar()
	{
		if (mOutputStart < mOutputEnd) {
			return *mOutputStart;
		}
		else {
			FillOutputBuffer();
			if (mOutputStart < mOutputEnd) return *mOutputStart;
			else return UnicodeChar(0xFFFF);
		}
	}


	UnicodeChar XMLReader::ReadChar()
	{
		if (mOutputStart < mOutputEnd) {
			return *mOutputStart++;
		}
		else {
			FillOutputBuffer();
			if (mOutputStart < mOutputEnd) return *mOutputStart++;
			else return UnicodeChar(0xFFFF);
		}
	}


	bool XMLReader::ParseChar(UnicodeChar c)
	{
		if (PeekChar() == c) {
			++mOutputStart;
			return true;
		}
		else return false;
	}


	bool XMLReader::ParseOptionalWhitespace()
	{
		while (IsWhitespace(PeekChar())) {
			ReadChar();
		}
		return true;
	}


	bool XMLReader::ParseRequiredWhitespace()
	{
		if (IsWhitespace(PeekChar())) {
			ReadChar();
			ParseOptionalWhitespace();
			return true;
		}
		else return false;
	}


	bool XMLReader::ParseElement()
	{
		// element ::= EmptyElemTag
		//				| STag content ETag
		// EmptyElemTag ::= '<' Name (S Attribute)* S? '/>'
		// STag	   ::=   	'<' Name (S Attribute)* S? '>'
		if (ParseString("<") && ParseName(mCurrentName)) {
			mNodeType = kElement;
			mAttributes.clear();
			while (ParseRequiredWhitespace()) {
				Attribute attribute;
				if (ParseString("/>")) {
					mIsEmptyElement = true;
					PopNamespaces(PushNamespaces());
					return true;
				}
				else if (ParseString(">")) {
					mIsEmptyElement = false;
					PushTag();
					return true;
				}
				else if (ParseAttribute(attribute)) {
					mAttributes.push_back(attribute);
				}
				else return false;
			}
			if (ParseString("/>")) {
				mIsEmptyElement = true;
				PopNamespaces(PushNamespaces());
				return true;
			}
			else if (ParseString(">")) {
				mIsEmptyElement = false;
				PushTag();
				return true;
			}
		}

		return false;
	}


	bool XMLReader::ParseEndElement()
	{
		// ETag ::= '</' Name S? '>'
		if (ParseString("</") && ParseName(mCurrentName) && ParseOptionalWhitespace() && ParseString(">")) {
			if (mOpenTags.empty()) return false;
			Tag & currentTag = mOpenTags.back();
			if (currentTag.mName != mCurrentName.mName) return false;
			mNodeType = kEndElement;
			PopTag();
			return true;
		}
		return false;
	}


	bool XMLReader::ParseText()
	{
		// CharData ::= [^<&]* - ([^<&]* ']]>' [^<&]*)

		mValue.clear();
		mNodeType = kWhitespace;
		mCurrentName.Clear();
		while (true) {
			switch (PeekChar()) {
				case UnicodeChar(0xFFFF):
				case UnicodeChar('<'):
					return ! mValue.empty();
				case UnicodeChar('&'):
					ReadChar();
					if (! ParseReference(mValue)) return false;
					break;
				default: {
				    UnicodeChar c = ReadChar();
				    if (! IsWhitespace(c)) mNodeType = kText;
					if (! mSkipContent) mValue += c;
					break;
				}
			}
		}
	}


	bool XMLReader::ParseName(Name & name)
	{
		// Name ::= (Letter | '_' | ':') (NameChar)*
		UnicodeString temp;
		UnicodeChar c = ReadChar();
		if ((c == UnicodeChar('_')) || (c == UnicodeChar(':')) || IsLetter(c)) {
			temp += c;
			while (IsNameChar(PeekChar())) temp += ReadChar();
			name.SetName(temp);
			return true;
		}
		return false;
	}

	bool XMLReader::ParseAttribute(Attribute & attribute)
	{
		// Attribute ::= Name Eq AttValue
		if (ParseName(attribute)) {
			return ParseEq()
				&& ParseAttValue(attribute.mValue);
		}
		return false;
	}

	bool XMLReader::ParseEq()
	{
		// Eq ::= S? '=' S?
		return ParseOptionalWhitespace()
			&& ParseString("=")
			&& ParseOptionalWhitespace();
	}

	bool XMLReader::ParseAttValue(UnicodeString & value)
	{
		// AttValue ::= '"' ([^<&"] | Reference)* '"'
		//	|  "'" ([^<&'] | Reference)* "'"
		value.clear();
		if (ParseString("\"")) {
			while (true) {
				UnicodeChar c = ReadChar();
				switch (c) {
					case UnicodeChar('<'):
					case 0:
						return false;
					case '&':
                        if (! ParseReference(value)) return false;
						break;
					case UnicodeChar('"'):
						return true;
					default:
						value += c;
						break;
				}
			}
		}
		else if (ParseString("'")) {
			while (true) {
				UnicodeChar c = ReadChar();
				switch (c) {
					case UnicodeChar('<'):
					case 0:
						return false;
					case '&':
                        if (! ParseReference(value)) return false;
						break;
					case UnicodeChar('\''):
						return true;
					default:
						value += c;
						break;
				}
			}
		}
		return false;
	}
	bool XMLReader::ParseReference(UnicodeString & value)
	{
		// Note that the '&' has already been read when this
		// function is called.
		// Reference 	   ::=    	EntityRef | CharRef
		// EntityRef 	   ::=    	'&' Name ';'
		// CharRef 	   ::=    	'&#' [0-9]+ ';'
		//	| '&#x' [0-9a-fA-F]+ ';'
		if (ParseString("#x")) {
			UnicodeChar result = 0;
			while (true) {
				UnicodeChar c = ReadChar();
				if (c == UnicodeChar(';')) {
					value += result;
					return true;
				}
				else if ((c >= UnicodeChar('0')) && (c <= UnicodeChar('9'))) {
					result = (result * 16) + (c - UnicodeChar('0'));
				}
				else if ((c >= UnicodeChar('A')) && (c <= UnicodeChar('F'))) {
					result = (result * 16) + (c - UnicodeChar('A') + 10);
				}
				else if ((c >= UnicodeChar('a')) && (c <= UnicodeChar('f'))) {
					result = (result * 16) + (c - UnicodeChar('a') + 10);
				}
				else return false;
			}
		}
		else if (ParseString("#")) {
			UnicodeChar result = 0;
			while (true) {
				UnicodeChar c = ReadChar();
				if (c == UnicodeChar(';')) {
					value += result;
					return true;
				}
				else if ((c >= UnicodeChar('0')) && (c <= UnicodeChar('9'))) {
					result = (result * 10) + (c - UnicodeChar('0'));
				}
				else return false;
			}
		}
		// Note that we're hardwiring the minimal set of entity names.
		// It would be more correct to call ParseName and then call
		// a function to get the entity value.
		else if (ParseString("amp;")) {
			value += '&';
			return true;
		}
		else if (ParseString("quot;")) {
			value += '"';
			return true;
		}
		else if (ParseString("apos;")) {
			value += '\'';
			return true;
		}
		else if (ParseString("lt;")) {
			value += '<';
			return true;
		}
		else if (ParseString("gt;")) {
			value += '>';
			return true;
		}

		return false;
	}

	bool XMLReader::ParseXmlDeclaration()
	{
		// XMLDecl	::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
		// VersionInfo ::= S 'version' Eq ("'" VersionNum "'" | '"' VersionNum '"')
		// EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' | "'" EncName "'" )
		// SDDecl ::=  S 'standalone' Eq (("'" ('yes' | 'no') "'") | ('"' ('yes' | 'no') '"'))
		if (ParseString("<?xml")) {
			mNodeType = kXmlDeclaration;
			mCurrentName.SetName("xml");
			mAttributes.clear();
			while (ParseRequiredWhitespace()) {
				Attribute attribute;
				if (ParseString("?>")) return true;
				else if (ParseAttribute(attribute)) {
					mAttributes.push_back(attribute);
				}
				else return false;
			}
			if (ParseString("?>")) return true;
		}
		return false;
	}

	void XMLReader::FillInputBuffer()
	{
		if (mInputStart > mInputBuffer) {
			size_t len = mInputEnd - mInputStart;
			std::memmove(mInputBuffer, mInputStart, len * sizeof(*mInputBuffer));
			mInputEnd = mInputBuffer + len;
			mInputStart = mInputBuffer;
		}
		size_t remainingCapacity = mInputBuffer + kInputBufferCount - mInputEnd;
		if (remainingCapacity > 0) {
			mInputEnd += mInput.ReadUpTo(mInputEnd, uint32_t(remainingCapacity));
		}
	}


	void XMLReader::FillOutputBuffer()
	{
		if (mOutputStart > mOutputBuffer) {
			size_t len = mOutputEnd - mOutputStart;
			std::memmove(mOutputBuffer, mOutputStart, len * sizeof(*mOutputBuffer));
			mOutputEnd = mOutputBuffer + len;
			mOutputStart = mOutputBuffer;
		}
		size_t remainingCapacity = mOutputBuffer + kOutputBufferCount - mOutputEnd;
		if (remainingCapacity > 0) {
			FillInputBuffer();
			if (mInputEnd > mInputStart) {
				mConverter.Convert(const_cast<const char * &>(mInputStart), mInputEnd,
					mOutputEnd, mOutputBuffer + kOutputBufferCount);
			}
		}
	}


	bool XMLReader::BufferStartsWith(const char * prefix)
	{
		ptrdiff_t prefixLen = std::strlen(prefix);
		if (mOutputEnd - mOutputStart < prefixLen) FillOutputBuffer();
		if (mOutputEnd - mOutputStart >= prefixLen) {
			const char * prefixEnd = prefix + prefixLen;
			const UnicodeChar * output = mOutputStart;
			while (prefix < prefixEnd) {
				if (*output++ != UnicodeChar(*prefix++)) return false;
			}
			return true;
		}
		else return false;
	}

	bool XMLReader::ParseString(const char * s)
	{
		if (BufferStartsWith(s)) {
			mOutputStart += std::strlen(s);
			return true;
		}
		else return false;
	}

	bool XMLReader::IsWhitespace(UnicodeChar c)
	{
		return (c == '\x20') || (c == '\x0d') || (c == '\x0A') || (c == '\x0D');
	}

	bool XMLReader::StartsWithWhitespace()
	{
		return IsWhitespace(PeekChar());
	}

	void XMLReader::LookupNamespace(Name & name) const
	{
		for (std::vector<NamespaceMapping>::const_reverse_iterator i = mNamespaceMappings.rbegin();
				i != mNamespaceMappings.rend();
				++i) {
			if (i->mPrefix == name.mPrefix) {
				name.mNamespaceURI = i->mNamespaceURI;
				return;
			}
		}
		name.mNamespaceURI.clear();
	}

	bool XMLReader::IsInRange(UnicodeChar c, const UniCharRange ranges[],
		size_t n)
	{
		size_t low = 0;
		size_t high = n;
		while (low < high) {
			size_t mid = (low + high + 1) / 2;
			if (c >= ranges[mid].mLowChar) low = mid;
			else high = mid - 1;
		}
		return (c >= ranges[low].mLowChar) && (c <= ranges[low].mHighChar);
	}
	
	const XMLReader::UniCharRange XMLReader::sBaseCharRanges[] = {
			{0x0041, 0x005A}, {0x0061, 0x007A}, {0x00C0, 0x00D6}, {0x00D8, 0x00F6}, 
			{0x00F8, 0x00FF}, {0x0100, 0x0131}, {0x0134, 0x013E}, {0x0141, 0x0148}, 
			{0x014A, 0x017E}, {0x0180, 0x01C3}, {0x01CD, 0x01F0}, {0x01F4, 0x01F5}, 
			{0x01FA, 0x0217}, {0x0250, 0x02A8}, {0x02BB, 0x02C1}, {0x0386, 0x0386},
			{0x0388, 0x038A}, {0x038C, 0x038C}, {0x038E, 0x03A1}, {0x03A3, 0x03CE}, 
			{0x03D0, 0x03D6}, {0x03DA, 0x03DA}, {0x03DC, 0x03DC}, {0x03DE, 0x03DE},
			{0x03E0, 0x03E0}, {0x03E2, 0x03F3}, {0x0401, 0x040C}, {0x040E, 0x044F}, 
			{0x0451, 0x045C}, {0x045E, 0x0481}, {0x0490, 0x04C4}, {0x04C7, 0x04C8}, 
			{0x04CB, 0x04CC}, {0x04D0, 0x04EB}, {0x04EE, 0x04F5}, {0x04F8, 0x04F9}, 
			{0x0531, 0x0556}, {0x0559, 0x0559}, {0x0561, 0x0586}, {0x05D0, 0x05EA}, 
			{0x05F0, 0x05F2}, {0x0621, 0x063A}, {0x0641, 0x064A}, {0x0671, 0x06B7}, 
			{0x06BA, 0x06BE}, {0x06C0, 0x06CE}, {0x06D0, 0x06D3}, {0x06D5, 0x06D5},
			{0x06E5, 0x06E6}, {0x0905, 0x0939}, {0x093D, 0x093D}, {0x0958, 0x0961}, 
			{0x0985, 0x098C}, {0x098F, 0x0990}, {0x0993, 0x09A8}, {0x09AA, 0x09B0}, 
			{0x09B2, 0x09B2}, {0x09B6, 0x09B9}, {0x09DC, 0x09DD}, {0x09DF, 0x09E1}, 
			{0x09F0, 0x09F1}, {0x0A05, 0x0A0A}, {0x0A0F, 0x0A10}, {0x0A13, 0x0A28}, 
			{0x0A2A, 0x0A30}, {0x0A32, 0x0A33}, {0x0A35, 0x0A36}, {0x0A38, 0x0A39}, 
			{0x0A59, 0x0A5C}, {0x0A5E, 0x0A5E}, {0x0A72, 0x0A74}, {0x0A85, 0x0A8B}, 
			{0x0A8D, 0x0A8D}, {0x0A8F, 0x0A91}, {0x0A93, 0x0AA8}, {0x0AAA, 0x0AB0}, 
			{0x0AB2, 0x0AB3}, {0x0AB5, 0x0AB9}, {0x0ABD, 0x0ABD}, {0x0AE0, 0x0AE0},
			{0x0B05, 0x0B0C}, {0x0B0F, 0x0B10}, {0x0B13, 0x0B28}, {0x0B2A, 0x0B30}, 
			{0x0B32, 0x0B33}, {0x0B36, 0x0B39}, {0x0B3D, 0x0B3D}, {0x0B5C, 0x0B5D}, 
			{0x0B5F, 0x0B61}, {0x0B85, 0x0B8A}, {0x0B8E, 0x0B90}, {0x0B92, 0x0B95}, 
			{0x0B99, 0x0B9A}, {0x0B9C, 0x0B9C}, {0x0B9E, 0x0B9F}, {0x0BA3, 0x0BA4}, 
			{0x0BA8, 0x0BAA}, {0x0BAE, 0x0BB5}, {0x0BB7, 0x0BB9}, {0x0C05, 0x0C0C}, 
			{0x0C0E, 0x0C10}, {0x0C12, 0x0C28}, {0x0C2A, 0x0C33}, {0x0C35, 0x0C39}, 
			{0x0C60, 0x0C61}, {0x0C85, 0x0C8C}, {0x0C8E, 0x0C90}, {0x0C92, 0x0CA8}, 
			{0x0CAA, 0x0CB3}, {0x0CB5, 0x0CB9}, {0x0CDE, 0x0CDE}, {0x0CE0, 0x0CE1}, 
			{0x0D05, 0x0D0C}, {0x0D0E, 0x0D10}, {0x0D12, 0x0D28}, {0x0D2A, 0x0D39}, 
			{0x0D60, 0x0D61}, {0x0E01, 0x0E2E}, {0x0E30, 0x0E30}, {0x0E32, 0x0E33}, 
			{0x0E40, 0x0E45}, {0x0E81, 0x0E82}, {0x0E84, 0x0E84}, {0x0E87, 0x0E88}, 
			{0x0E8A, 0x0E8A}, {0x0E8D, 0x0E8D}, {0x0E94, 0x0E97}, {0x0E99, 0x0E9F}, 
			{0x0EA1, 0x0EA3}, {0x0EA5, 0x0EA5}, {0x0EA7, 0x0EA7}, {0x0EAA, 0x0EAB}, 
			{0x0EAD, 0x0EAE}, {0x0EB0, 0x0EB0}, {0x0EB2, 0x0EB3}, {0x0EBD, 0x0EBD},
			{0x0EC0, 0x0EC4}, {0x0F40, 0x0F47}, {0x0F49, 0x0F69}, {0x10A0, 0x10C5}, 
			{0x10D0, 0x10F6}, {0x1100, 0x1100}, {0x1102, 0x1103}, {0x1105, 0x1107}, 
			{0x1109, 0x1109}, {0x110B, 0x110C}, {0x110E, 0x1112}, {0x113C, 0x113C},
			{0x113E, 0x113E}, {0x1140, 0x1140}, {0x114C, 0x114C}, {0x114E, 0x114E},
			{0x1150, 0x1150}, {0x1154, 0x1155}, {0x1159, 0x1159}, {0x115F, 0x1161}, 
			{0x1163, 0x1163}, {0x1165, 0x1165}, {0x1167, 0x1167}, {0x1169, 0x1169},
			{0x116D, 0x116E}, {0x1172, 0x1173}, {0x1175, 0x1175}, {0x119E, 0x119E},
			{0x11A8, 0x11A8}, {0x11AB, 0x11AB}, {0x11AE, 0x11AF}, {0x11B7, 0x11B8}, 
			{0x11BA, 0x11BA}, {0x11BC, 0x11C2}, {0x11EB, 0x11EB}, {0x11F0, 0x11F0},
			{0x11F9, 0x11F9}, {0x1E00, 0x1E9B}, {0x1EA0, 0x1EF9}, {0x1F00, 0x1F15}, 
			{0x1F18, 0x1F1D}, {0x1F20, 0x1F45}, {0x1F48, 0x1F4D}, {0x1F50, 0x1F57}, 
			{0x1F59, 0x1F59}, {0x1F5B, 0x1F5B}, {0x1F5D, 0x1F5D}, {0x1F5F, 0x1F7D}, 
			{0x1F80, 0x1FB4}, {0x1FB6, 0x1FBC}, {0x1FBE, 0x1FBE}, {0x1FC2, 0x1FC4}, 
			{0x1FC6, 0x1FCC}, {0x1FD0, 0x1FD3}, {0x1FD6, 0x1FDB}, {0x1FE0, 0x1FEC}, 
			{0x1FF2, 0x1FF4}, {0x1FF6, 0x1FFC}, {0x2126, 0x2126}, {0x212A, 0x212B}, 
			{0x212E, 0x212E}, {0x2180, 0x2182}, {0x3041, 0x3094}, {0x30A1, 0x30FA}, 
			{0x3105, 0x312C}, {0xAC00, 0xD7A3}
		};

	bool XMLReader::IsBaseChar(UnicodeChar c)
	{
		return IsInRange(c, sBaseCharRanges, countof(sBaseCharRanges));
	}

	bool XMLReader::IsIdeographic(UnicodeChar c)
	{
		return ((c >= 0x4E00) && (c <= 0x9FA5))
			|| (c == 0x3007)
			|| ((c >= 0x3021) && (c <= 0x3029));
	}

	bool XMLReader::IsLetter(UnicodeChar c)
	{
		return IsBaseChar(c) || IsIdeographic(c);
	}

	const XMLReader::UniCharRange XMLReader::sCombiningCharRanges[] = {
		{0x0300, 0x0345}, {0x0360, 0x0361}, {0x0483, 0x0486}, {0x0591, 0x05A1},
		{0x05A3, 0x05B9}, {0x05BB, 0x05BD}, {0x05BF, 0x05BF}, {0x05C1, 0x05C2},
		{0x05C4, 0x05C4}, {0x064B, 0x0652}, {0x0670, 0x0670}, {0x06D6, 0x06DC},
		{0x06DD, 0x06DF}, {0x06E0, 0x06E4}, {0x06E7, 0x06E8}, {0x06EA, 0x06ED},
		{0x0901, 0x0903}, {0x093C, 0x093C}, {0x093E, 0x094C}, {0x094D, 0x094D},
		{0x0951, 0x0954}, {0x0962, 0x0963}, {0x0981, 0x0983}, {0x09BC, 0x09BC},
		{0x09BE, 0x09BE}, {0x09BF, 0x09BF}, {0x09C0, 0x09C4}, {0x09C7, 0x09C8},
		{0x09CB, 0x09CD}, {0x09D7, 0x09D7}, {0x09E2, 0x09E3}, {0x0A02, 0x0A02},
		{0x0A3C, 0x0A3C}, {0x0A3E, 0x0A3E}, {0x0A3F, 0x0A3F}, {0x0A40, 0x0A42},
		{0x0A47, 0x0A48}, {0x0A4B, 0x0A4D}, {0x0A70, 0x0A71}, {0x0A81, 0x0A83},
		{0x0ABC, 0x0ABC}, {0x0ABE, 0x0AC5}, {0x0AC7, 0x0AC9}, {0x0ACB, 0x0ACD},
		{0x0B01, 0x0B03}, {0x0B3C, 0x0B3C}, {0x0B3E, 0x0B43}, {0x0B47, 0x0B48},
		{0x0B4B, 0x0B4D}, {0x0B56, 0x0B57}, {0x0B82, 0x0B83}, {0x0BBE, 0x0BC2},
		{0x0BC6, 0x0BC8}, {0x0BCA, 0x0BCD}, {0x0BD7, 0x0BD7}, {0x0C01, 0x0C03},
		{0x0C3E, 0x0C44}, {0x0C46, 0x0C48}, {0x0C4A, 0x0C4D}, {0x0C55, 0x0C56},
		{0x0C82, 0x0C83}, {0x0CBE, 0x0CC4}, {0x0CC6, 0x0CC8}, {0x0CCA, 0x0CCD},
		{0x0CD5, 0x0CD6}, {0x0D02, 0x0D03}, {0x0D3E, 0x0D43}, {0x0D46, 0x0D48},
		{0x0D4A, 0x0D4D}, {0x0D57, 0x0D57}, {0x0E31, 0x0E31}, {0x0E34, 0x0E3A},
		{0x0E47, 0x0E4E}, {0x0EB1, 0x0EB1}, {0x0EB4, 0x0EB9}, {0x0EBB, 0x0EBC},
		{0x0EC8, 0x0ECD}, {0x0F18, 0x0F19}, {0x0F35, 0x0F35}, {0x0F37, 0x0F37},
		{0x0F39, 0x0F39}, {0x0F3E, 0x0F3E}, {0x0F3F, 0x0F3F}, {0x0F71, 0x0F84},
		{0x0F86, 0x0F8B}, {0x0F90, 0x0F95}, {0x0F97, 0x0F97}, {0x0F99, 0x0FAD},
		{0x0FB1, 0x0FB7}, {0x0FB9, 0x0FB9}, {0x20D0, 0x20DC}, {0x20E1, 0x20E1},
		{0x302A, 0x302F}, {0x3099, 0x3099}, {0x309A, 0x309A}
	};

	bool XMLReader::IsCombiningChar(UnicodeChar c)
	{
		return IsInRange(c, sCombiningCharRanges, countof(sCombiningCharRanges));
	}

	const XMLReader::UniCharRange XMLReader::sDigitRanges[] = {
		{0x0030, 0x0039}, {0x0660, 0x0669}, {0x06F0, 0x06F9}, {0x0966, 0x096F},
		{0x09E6, 0x09EF}, {0x0A66, 0x0A6F}, {0x0AE6, 0x0AEF}, {0x0B66, 0x0B6F},
		{0x0BE7, 0x0BEF}, {0x0C66, 0x0C6F}, {0x0CE6, 0x0CEF}, {0x0D66, 0x0D6F},
		{0x0E50, 0x0E59}, {0x0ED0, 0x0ED9}, {0x0F20, 0x0F29}
	};

	bool XMLReader::IsDigit(UnicodeChar c)
	{
		return IsInRange(c, sDigitRanges, countof(sDigitRanges));
	}

	bool XMLReader::IsNameChar(UnicodeChar c)
	{
		// NameChar ::= Letter | Digit | '.' | '-' | '_' | ':' | CombiningChar | Extender
		return IsLetter(c)
			|| IsDigit(c)
			|| (c == UnicodeChar('.')) || (c == UnicodeChar('-'))
			|| (c == UnicodeChar('_')) || (c == UnicodeChar(':'));
	}

	bool XMLReader::IsNameToken(const UnicodeString & token)
	{
		// Nmtoken ::= (NameChar)+
		size_t n = token.size();
		if (n == 0) return false;
		for (size_t i = 0; i < n; ++i) {
			if (! IsNameChar(token[i])) return false;
		}
		return true;
	}

	bool XMLReader::IsName(const UnicodeString & name)
	{
		// Name ::= (Letter | '_' | ':') (NameChar)*
		size_t n = name.size();
		if (n == 0) return false;
		UnicodeChar c = name[0];
		if ((c == UnicodeChar('_')) || (c == UnicodeChar(':')) || IsLetter(c)) {
			for (size_t i = 1; i < n; ++i) {
				if (! IsNameChar(name[i])) return false;
			}
		}
		return true;
	}


	bool XMLReader::Equals(const UnicodeString & a, const char * b)
	{
		size_t bLen = std::strlen(b);
		if (a.size() != bLen) return false;
		for (size_t i = 0; i < bLen; ++i) {
			if (a[i] != UnicodeChar(b[i])) return false;
		}
		return true;
	}
	
	
	bool XMLReader::StartsWith(const UnicodeChar * haystack, const char * needle) {
	    while (*needle) {
	        if (*haystack++ != UnicodeChar(*needle++)) return false;
	    }
	    return true;
	}


	bool XMLReader::HasAttribute(size_t i) const
	{
		return (i < mAttributes.size());
	}

	
	bool XMLReader::HasAttribute(const char * name) const
	{
		for (std::vector<Attribute>::const_iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			if (Equals(i->mName, name)) return true;
		}
		return false;
	}


	bool XMLReader::HasAttribute(const char * localName, const char * namespaceURI) const
	{
		for (std::vector<Attribute>::const_iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			if (Equals(i->mLocalName, localName) && Equals(i->mNamespaceURI, namespaceURI)) return true;
		}
		return false;
	}


	UnicodeString XMLReader::GetAttribute(size_t i) const
	{
		if (i < mAttributes.size()) return mAttributes[i].mValue;
		else return UnicodeString();
	}

	
	UnicodeString XMLReader::GetAttribute(const char * name) const
	{
		for (std::vector<Attribute>::const_iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			if (Equals(i->mName, name)) return i->mValue;
		}
		return UnicodeString();
	}


	UnicodeString XMLReader::GetAttribute(const char * localName, const char * namespaceURI) const
	{
		for (std::vector<Attribute>::const_iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			if (Equals(i->mLocalName, localName) && Equals(i->mNamespaceURI, namespaceURI)) return i->mValue;
		}
		return UnicodeString();
	}


	std::string XMLReader::GetAttribute(size_t i, TextEncoding encoding) const
	{
	    return FromUnicode(GetAttribute(i), encoding);
	}

	
	std::string XMLReader::GetAttribute(const char * name, TextEncoding encoding) const
	{
	    return FromUnicode(GetAttribute(name), encoding);
	}


	std::string XMLReader::GetAttribute(const char * localName, const char * namespaceURI,
	    TextEncoding encoding) const
	{
	    return FromUnicode(GetAttribute(localName, namespaceURI), encoding);
	}


	size_t XMLReader::PushNamespaces()
	{
		size_t previousMappingSize = mNamespaceMappings.size();

		NamespaceMapping mapping;
		for (std::vector<Attribute>::iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			if (Equals(i->mName, "xmlns")) {
				mapping.mPrefix.clear();
				mapping.mNamespaceURI = i->mValue;
				mNamespaceMappings.push_back(mapping);
			}
			else if (Equals(i->mPrefix, "xmlns")) {
				mapping.mPrefix = i->mLocalName;
				mapping.mNamespaceURI = i->mValue;
				mNamespaceMappings.push_back(mapping);
			}
		}
		LookupNamespace(mCurrentName);
		for (std::vector<Attribute>::iterator i = mAttributes.begin(); i != mAttributes.end(); ++i) {
			LookupNamespace(*i);
		}
		return previousMappingSize;
	}

	void XMLReader::PushTag()
	{
		Tag newTag;
		newTag.mPreviousMappingSize = PushNamespaces();
		newTag.mName = mCurrentName.mName;
		newTag.mPrefix = mCurrentName.mPrefix;
		newTag.mLocalName = mCurrentName.mLocalName;
		newTag.mNamespaceURI = mCurrentName.mNamespaceURI;
		mOpenTags.push_back(newTag);
	}

	void XMLReader::PopNamespaces(size_t n)
	{
		mNamespaceMappings.resize(n);
	}

	void XMLReader::PopTag()
	{
		Tag & currentTag = mOpenTags.back();
		PopNamespaces(currentTag.mPreviousMappingSize);
		mCurrentName = currentTag;
		mOpenTags.pop_back();
	}


#if LLAMA_UNIT_TESTS && 0

	class StringInputStream: public InputStream {
	public:
		StringInputStream(const char * s);

		virtual UInt32 GetMarker();
		virtual void SetMarker(UInt32 position);
		virtual void ReadData(Char * buffer, UInt32 length);
		virtual UInt32 ReadUpTo(Char * buffer, UInt32 length);
		virtual bool EndOfFile() const;

	private:
		const char *	mString;
		UInt32			mStringLength;
		UInt32			mCurrentPosition;
	};

	StringInputStream::StringInputStream(const char * s)
		: mString(s), mStringLength(std::strlen(s)), mCurrentPosition(0)
	{
	}

	UInt32 StringInputStream::GetMarker()
	{
		return mCurrentPosition;
	}

	void StringInputStream::SetMarker(UInt32 position)
	{
		mCurrentPosition = position;
	}

	void StringInputStream::ReadData(Char * buffer, UInt32 length)
	{
		if (length <= mStringLength - mCurrentPosition) {
			std::memcpy(buffer, mString + mCurrentPosition, length);
			mCurrentPosition += length;
		}
#if TARGET_OS_WIN32
		else ThrowXMLError(E_FAIL);
#elif TARGET_OS_MAC
		else ThrowXMLError(errEndOfDocument);
#else
	#error Unknown platform for EOF error
#endif
	}

	UInt32 StringInputStream::ReadUpTo(Char * buffer, UInt32 length)
	{
		length = std::min<UInt32>(length, mStringLength - mCurrentPosition);
		std::memcpy(buffer, mString + mCurrentPosition, length);
		mCurrentPosition += length;
		return length;
	}

	bool StringInputStream::EndOfFile() const
	{
		return mCurrentPosition >= mStringLength;
	}
	
	
	static void Test1()
	{
		StringInputStream input("<a/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);
		
		TestAssert(! reader.Read());
	}
	
	static void Test2()
	{
		StringInputStream input("<a foo='bar'/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"bar");
		TestAssert(reader.GetAttribute("foo") == L"bar");
		TestAssert(reader.GetAttribute("foo", "") == L"bar");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test3()
	{
		StringInputStream input("<a></a>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(! reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kEndElement);
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test4()
	{
		StringInputStream input("<a>foo</a>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(! reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kText);
		TestAssert(reader.GetValue() == L"foo");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kEndElement);
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test5()
	{
		StringInputStream input("<a xmlns='http://foo.com'/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"http://foo.com");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"http://foo.com");
		TestAssert(reader.GetAttribute("xmlns") == L"http://foo.com");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test6()
	{
		StringInputStream input("<foo:a xmlns:foo='http://foo.com'/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"foo:a");
		TestAssert(reader.GetPrefix() == L"foo");
		TestAssert(reader.GetNamespaceURI() == L"http://foo.com");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"http://foo.com");
		TestAssert(reader.GetAttribute("xmlns:foo") == L"http://foo.com");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test7()
	{
		StringInputStream input("<a xmlns:foo='http://foo.com'/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"http://foo.com");
		TestAssert(reader.GetAttribute("xmlns:foo") == L"http://foo.com");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test8()
	{
		StringInputStream input("<a xmlns='http://foo.com'><b/></a>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(! reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"http://foo.com");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"http://foo.com");
		TestAssert(reader.GetAttribute("xmlns") == L"http://foo.com");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"b");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"http://foo.com");
		TestAssert(reader.GetLocalName() == L"b");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kEndElement);
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"http://foo.com");
		TestAssert(reader.GetLocalName() == L"a");

		TestAssert(! reader.Read());
	}
	
	
	static void Test9()
	{
		StringInputStream input("<a>&amp;&quot;&apos;&gt;&lt;</a>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(! reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kText);
		TestAssert(reader.GetValue() == L"&\"'><");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kEndElement);
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test10()
	{
		StringInputStream input("<?xml version='1.0'?><a/>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kXmlDeclaration);
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 1);
		TestAssert(reader.GetAttribute(size_t(0)) == L"1.0");
		TestAssert(reader.GetAttribute("version") == L"1.0");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test11()
	{
		StringInputStream input("<?xml version='1.0' encoding='Windows-1252'?><a>j'aime fran\347ais</a>");
		XMLReader reader(input);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kDocument);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kXmlDeclaration);
		TestAssert(reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 2);
		TestAssert(reader.GetAttribute(size_t(0)) == L"1.0");
		TestAssert(reader.GetAttribute("version") == L"1.0");
		TestAssert(reader.GetAttribute("encoding") == L"Windows-1252");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kElement);
		TestAssert(! reader.IsEmptyElement());
		TestAssert(reader.GetName() == L"a");
		TestAssert(reader.GetPrefix() == L"");
		TestAssert(reader.GetNamespaceURI() == L"");
		TestAssert(reader.GetLocalName() == L"a");
		TestAssert(! reader.HasAttributes());
		TestAssert(reader.AttributeCount() == 0);

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kText);
		TestAssert(reader.GetValue() == L"j'aime fran\347ais");

		TestAssert(reader.Read());
		TestAssert(reader.GetNodeType() == XMLReader::kEndElement);
		
		TestAssert(! reader.Read());
	}
	
	
	static void Test12()
	{
#if TARGET_OS_WIN32
		LlamaPlatform::FileSpecification file("C:\\Program Files\\Llamagraphics\\Life Balance\\New Sample Data.lbe");
		LlamaPlatform::OpenFileHandle fileHandle(file);
		FileInputStream input(fileHandle);
		XMLReader reader(input);

		while (reader.Read()) {
		}
		TestAssert(reader.EndOfFile());
#endif
	}


	static void TestXMLReader()
	{
		Test1();
		Test2();
		Test3();
		Test4();
		Test5();
		Test6();
		Test7();
		Test8();
		Test9();
		Test10();
		Test11();
		Test12();
	}

	UnitTest::Register gRegisterTest("XMLReader", TestXMLReader);
#endif

}
