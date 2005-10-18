/*
 * Copyright (c) 2005, Llamagraphics, Inc.
 * All rights reserved.
 */

#include "LlamaXML/XMLWriter.h"
#include "LlamaXML/XMLException.h"
#include <cstring>
#include <cstdio>
#include <stddef.h>
#include <assert.h>
#include <limits>

#define countof(x) (sizeof(x) / sizeof(*x))

namespace LlamaXML {

	const char * XMLWriter::kNewline =
#if defined(WIN32)
		"\r\n";
#else
		"\n";
#endif
	const char * XMLWriter::kIndent = "  ";
	const char * XMLWriter::kAmpersand = "&amp;";
	const char * XMLWriter::kLessThan = "&lt;";
	const char * XMLWriter::kGreaterThan = "&gt;";
	const char * XMLWriter::kQuote = "&quot;";

	XMLWriter::XMLWriter(OutputStream & output, TextEncoding applicationEncoding)
	: mOutput(output),
	  mState(kStateNormal),
	  mIndentLevel(std::numeric_limits<size_t>::max()),
	  mApplicationToUnicode(applicationEncoding),
	  mUnicodeToUTF8(TextEncoding::UTF8())
	{
	}
	
	
	XMLWriter::~XMLWriter()
	{
		EndDocument();
	}
	
	
	void XMLWriter::StartDocument(const char * version,
                           const char * encoding,
                           const char * standalone)
	{
		mOutput << "<?xml version=\"" << version
			<< "\" encoding=\"" << encoding
			<< "\" standalone=\"" << standalone
			<< "\" ?>";
	}
	
	
	void XMLWriter::EndDocument()
	{
	    if (mState != kStateDocumentClosed) {
    		while (! mElementStack.empty()) {
    			EndElement();
    		}
    		mOutput << kNewline;
    		mState = kStateDocumentClosed;
	    }
	}
	
	
	void XMLWriter::StartElement(const char * name)
	{
		switch (mState) {
			case kStateOpenAttribute:
				mOutput << "\"";
				// fall through
			case kStateOpenTag:
				mOutput << ">";
				break;
			case kStateNormal:
				break;
		}
		
		if (mElementStack.size() < mIndentLevel) {
			mOutput << kNewline;
			for (size_t i = mElementStack.size(); i > 0; --i) {
				mOutput << kIndent;
			}
		}
		mOutput << "<" << name;
		mElementStack.push_back(name);
		mState = kStateOpenTag;
	}
	
	
	void XMLWriter::StartElement(const char * prefix, const char * name,
		const char * namespaceURI)
	{
		std::string buffer;
		if (prefix && *prefix) {
			buffer = prefix;
			buffer += ':';
		}
		buffer += name;
		StartElement(buffer.c_str());
		
		if (namespaceURI) {
			buffer = "xmlns";
			if (prefix && *prefix) {
				buffer += ':';
				buffer += prefix;
			}
			WriteAttribute(buffer.c_str(), namespaceURI);
		}
	}
	
	
	void XMLWriter::EndElement()
	{
		if (! mElementStack.empty()) {
			switch (mState) {
				case kStateOpenAttribute:
					// Ending an element automatically closes
					// the current attribute
					mOutput << "\"/>";
					break;
				case kStateOpenTag:
					mOutput << "/>";
					break;
				case kStateNormal:
					if (mElementStack.size() < mIndentLevel) {
						mOutput << kNewline;
						for (size_t i = mElementStack.size() - 1; i > 0; --i) {
							mOutput << kIndent;
						}
					}
					else if (mElementStack.size() == mIndentLevel) {
						mIndentLevel = std::numeric_limits<size_t>::max();
					}

					mOutput << "</" << mElementStack.back().c_str() << ">";
					break;
			}
			mElementStack.pop_back();
		}
		mState = kStateNormal;
	}


	void XMLWriter::WriteElement(const char * name)
	{
		StartElement(name);
		EndElement();
	}

	const char * XMLWriter::Scan(const char * content, const char * tokens) {
		while (*content) {
			for (const char * token = tokens; *token; ++token) {
				if (*content == *token) return content;
			}
			++content;
		}
		return content;
	}

	const UnicodeChar * XMLWriter::Scan(const UnicodeChar * content, const UnicodeChar * contentEnd, const char * tokens) {
		while (content < contentEnd) {
			for (const char * token = tokens; *token; ++token) {
				if (*content == UnicodeChar(*token)) return content;
			}
			++content;
		}
		return content;
	}

	const char * XMLWriter::StringEnd(const char * s) {
		while (*s) ++s;
		return s;
	}

	const UnicodeChar * XMLWriter::StringEnd(const UnicodeChar * s) {
		while (*s) ++s;
		return s;
	}

	void XMLWriter::WriteApplicationContent(const char * content) {
		if (mApplicationToUnicode.GetSourceEncoding() == mUnicodeToUTF8.GetDestinationEncoding()) {
			WriteUTF8Content(content);
		}
		else {
			UnicodeChar buffer[kBufferSize];
			const char * contentEnd = StringEnd(content);
			while (content < contentEnd) {
				UnicodeChar * bufferStart = buffer;
				mApplicationToUnicode.Convert(content, contentEnd, bufferStart, buffer + countof(buffer));
				WriteUnicodeContent(buffer, bufferStart);
			}
		}
	}

	void XMLWriter::WriteUTF8Content(const char * content) {
		while (*content) {
			const char * token = Scan(content, "<>\"&");
			if (content < token) {
				mOutput.WriteData(content, uint32_t(token - content));
				content = token;
			}
			switch (*content) {
				case '<':
					mOutput << kLessThan;
					break;
				case '>':
					mOutput << kGreaterThan;
					break;
				case '"':
					mOutput << kQuote;
					break;
				case '&':
					mOutput << kAmpersand;
					break;
				default:
					return;
			}
			++content;
		}
	}


	void XMLWriter::WriteRawUnicode(const UnicodeChar * unicodeStart, const UnicodeChar * unicodeEnd) {
		char buffer[kBufferSize];
		while (unicodeStart < unicodeEnd) {
			char * utf8Start = buffer;
			char * utf8End = buffer + countof(buffer);
			mUnicodeToUTF8.Convert(unicodeStart, unicodeEnd, utf8Start, utf8End);
			mOutput.WriteData(buffer, uint32_t(utf8Start - buffer));
		}
	}


	void XMLWriter::WriteUnicodeContent(const UnicodeChar * content, const UnicodeChar * contentEnd) {
		while (content < contentEnd) {
			const UnicodeChar * token = Scan(content, contentEnd, "<>\"&");
			if (content < token) {
				WriteRawUnicode(content, token);
				content = token;
			}
			switch (*content) {
				case UnicodeChar('<'):
					mOutput << kLessThan;
					break;
				case UnicodeChar('>'):
					mOutput << kGreaterThan;
					break;
				case UnicodeChar('"'):
					mOutput << kQuote;
					break;
				case UnicodeChar('&'):
					mOutput << kAmpersand;
					break;
			}
			++content;
		}
	}
	
	
	void XMLWriter::WriteString(const char * content)
	{
		switch (mState) {
			case kStateOpenTag:
				mOutput << ">";
				mState = kStateNormal;
				// fall through
			case kStateNormal:
				mIndentLevel = std::min<size_t>(mIndentLevel, mElementStack.size());
				WriteApplicationContent(content);
				break;
			case kStateOpenAttribute:
				WriteApplicationContent(content);
				break;
		}
	}
	
	
	void XMLWriter::WriteString(const UnicodeString & content)
	{
		switch (mState) {
			case kStateOpenTag:
				mOutput << ">";
				mState = kStateNormal;
				// fall through
			case kStateNormal:
				mIndentLevel = std::min<size_t>(mIndentLevel, mElementStack.size());
				WriteUnicodeContent(content.data(), content.data() + content.size());
				break;
			case kStateOpenAttribute:
				WriteUnicodeContent(content.data(), content.data() + content.size());
				break;
		}
	}
	
	
	void XMLWriter::WriteString(const UnicodeChar * content)
	{
		switch (mState) {
			case kStateOpenTag:
				mOutput << ">";
				mState = kStateNormal;
				// fall through
			case kStateNormal:
				mIndentLevel = std::min<size_t>(mIndentLevel, mElementStack.size());
				WriteUnicodeContent(content, StringEnd(content));
				break;
			case kStateOpenAttribute:
				WriteUnicodeContent(content, StringEnd(content));
				break;
		}
	}
	
	
	void XMLWriter::StartAttribute(const char * name)
	{
		switch (mState) {
			case kStateNormal:
				// Error: cannot start attribute when
				// no element is open
				assert(false);
				return;
			case kStateOpenAttribute:
				// Starting an attribute automatically closes
				// the previous attribute
				mOutput << "\"";
				// fall through
			case kStateOpenTag:
				mOutput << " " << name << "=\"";
				break;
		}
		mState = kStateOpenAttribute;
	}
	
	
	void XMLWriter::StartAttribute(const char * prefix,
		const char * name, const char * namespaceURI)
	{
		std::string buffer;
		if (namespaceURI) {
			buffer = "xmlns";
			if (prefix && *prefix) {
				buffer += ':';
				buffer += prefix;
			}
			WriteAttribute(buffer.c_str(), namespaceURI);
		}
		
		buffer.clear();
		if (prefix && *prefix) {
			buffer = prefix;
			buffer += ':';
		}
		buffer += name;
		StartAttribute(buffer.c_str());
	}
	
	
	void XMLWriter::EndAttribute()
	{
		switch (mState) {
			case kStateOpenAttribute:
				mOutput << "\"";
				break;
			default:
				// Error: closing attribute when no attribute
				// is open.
				assert(false);
				return;
		}
		mState = kStateOpenTag;
	}
}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
	const char * s)
{
	output.WriteString(s);
	return output;
}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
	unsigned long long n)
{
	if (n == 0) output.WriteString("0");
	else {
		char buffer[22];
		char * p = buffer + sizeof(buffer);
		*--p = 0;
		while (n) {
			*--p = char(n % 10) + '0';
			n /= 10;
		}
		output.WriteString(p);
	}
	return output;
}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
										const LlamaXML::UnicodeString & s)
{
	output.WriteString(s);
	return output;
}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
										int n)
{
	char buffer[64];
	std::sprintf(buffer, "%d", n);
	output.WriteString(buffer);
	return output;
}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
										double n)
{
	char buffer[64];
	std::sprintf(buffer, "%lf", n);
	output.WriteString(buffer);
	return output;
}

#if LLAMA_DEBUG
namespace Llamagraphics {

	static bool OutputMatches(VectorOutputStream & output,
		const char * result)
	{
		size_t len = std::strlen(result);
		
		return (output.size() == len) &&
			(std::strncmp(reinterpret_cast<const char *>(output.data()),
				result, len) == 0);
	}
	
	static void TestXMLWriter()
	{
		{
			VectorOutputStream output;
			XMLWriter writer(output);
			
			writer.StartDocument("1.0", "UTF-8", "yes");
			writer.StartElement("foo");
			writer.EndDocument();
			
			TestAssert(OutputMatches(output,
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<foo/>"));
		}
		{
			VectorOutputStream output;
			XMLWriter writer(output);
			
			writer.StartDocument("1.0", "UTF-8", "yes");
			writer.StartElement("foo");
			writer.WriteString("bar");
			writer.EndDocument();
			
			TestAssert(OutputMatches(output,
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<foo>bar</foo>"));
		}
		{
			VectorOutputStream output;
			XMLWriter writer(output);
			
			writer.StartDocument("1.0", "UTF-8", "yes");
			writer.StartElement("foo");
			writer.WriteAttribute("bar", "baz");
			writer.EndDocument();
			
			TestAssert(OutputMatches(output,
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<foo bar=\"baz\"/>"));
		}
		{
			VectorOutputStream output;
			XMLWriter writer(output);
			
			writer.StartDocument("1.0", "UTF-8", "yes");
			writer.StartElement("foo");
			writer.WriteString("bar & baz < foo > \"bar\"");
			writer.EndDocument();
			
			TestAssert(OutputMatches(output,
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<foo>bar &amp; baz &lt; foo &gt; \"bar\"</foo>"));
		}
		{
			VectorOutputStream output;
			XMLWriter writer(output);
			
			writer.StartDocument("1.0", "UTF-8", "yes");
			writer.StartElement("foo");
			writer.WriteAttribute("bar", "bar & baz < foo > \"bar\"");
			writer.EndDocument();
			
			TestAssert(OutputMatches(output,
				"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n<foo bar=\"bar &amp; baz &lt; foo &gt; &quot;bar&quot;\"/>"));
		}
	}
	
	static UnitTest::Register gUnitTest("XMLWriter", TestXMLWriter);

}
#endif
