#include "XMLWriter.h"
#include "ErrorCodeException.h"
#include <cstring>
#include <stddef.h>
#include <assert.h>

namespace LlamaXML {


	XMLWriter::XMLWriter(DataOutputStream & output)
	: mOutput(output),
	  mState(kStateNormal)
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
			<< "\"?>\n";
	}
	
	
	void XMLWriter::EndDocument()
	{
		while (! mElementStack.empty()) {
			EndElement();
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
		
		mOutput << "<" << name;
		mElementStack.push_back(name);
		mState = kStateOpenTag;
	}
	
	
	void XMLWriter::StartElement(const char * prefix, const char * name,
		const char * namespaceURI)
	{
		std::string buffer;
		if (prefix) {
			buffer = prefix;
			buffer += ':';
		}
		buffer += name;
		StartElement(buffer.c_str());
		
		if (namespaceURI) {
			buffer = "xmlns";
			if (prefix) {
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
					mOutput << "</" << mElementStack.back().c_str() << ">";
					break;
			}
			mElementStack.pop_back();
		}
		mState = kStateNormal;
	}
	
	
	void XMLWriter::WriteString(const char * content)
	{
		switch (mState) {
			case kStateOpenTag:
				mOutput << ">";
				mState = kStateNormal;
				// fall through
			case kStateNormal:
				while (*content) {
					size_t count = std::strcspn(content, "<>&");
					if (count) {
						mOutput.WriteData(content, count);
						content += count;
					}
					switch (*content) {
						case '<':
							mOutput << "&lt;";
							break;
						case '>':
							mOutput << "&gt;";
							break;
						case '&':
							mOutput << "&amp;";
							break;
						default:
							return;
					}
					++content;
				}
				break;
			case kStateOpenAttribute:
				while (*content) {
					size_t count = std::strcspn(content, "<>\"&");
					if (count) {
						mOutput.WriteData(content, count);
						content += count;
					}
					switch (*content) {
						case '<':
							mOutput << "&lt;";
							break;
						case '>':
							mOutput << "&gt;";
							break;
						case '"':
							mOutput << "&quot;";
							break;
						case '&':
							mOutput << "&amp;";
							break;
						default:
							return;
					}
					++content;
				}
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
			if (prefix) {
				buffer += ':';
				buffer += prefix;
			}
			WriteAttribute(buffer.c_str(), namespaceURI);
		}
		
		buffer.clear();
		if (prefix) {
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
