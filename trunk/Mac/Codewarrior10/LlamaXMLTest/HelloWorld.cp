/*
 *  Copyright © 1997-2003 Metrowerks Corporation.  All Rights Reserved.
 *
 *  Questions and comments to:
 *       <mailto:support@metrowerks.com>
 *       <http://www.metrowerks.com/>
 */

#include <iostream>
#include "LlamaXML/XMLReader.h"
#include "LlamaXML/FileInputStream.h"

using namespace std;
using namespace LlamaXML;

std::ostream & operator << (std::ostream & stream, UnicodeString s) {
    for (int i = 0; i < s.size(); ++i) {
        stream << char(s[i]);
    }
    return stream;
}


std::ostream & operator << (std::ostream & stream, XMLReader::NodeType n) {
    switch (n) {
        case XMLReader::kAttribute:
			stream << "Attribute";
			break;
		case XMLReader::kCDATA:
			stream << "CDATA";
			break;
		case XMLReader::kComment:
			stream << "Comment";
			break;
		case XMLReader::kDocument:
			stream << "Document";
			break;
		case XMLReader::kDocumentFragment:
			stream << "DocumentFragment";
			break;
		case XMLReader::kDocumentType:
			stream << "DocumentType";
			break;
		case XMLReader::kElement:
			stream << "Element";
			break;
		case XMLReader::kEndElement:
			stream << "EndElement";
			break;
		case XMLReader::kEndEntity:
			stream << "EndEntity";
			break;
		case XMLReader::kEntity:
			stream << "Entity";
			break;
		case XMLReader::kEntityReference:
			stream << "EntityReference";
			break;
		case XMLReader::kNone:
			stream << "None";
			break;
		case XMLReader::kNotation:
			stream << "Notation";
			break;
		case XMLReader::kProcessingInstruction:
			stream << "ProcessingInstruction";
			break;
		case XMLReader::kSignificantWhitespace:
			stream << "SignificantWhitespace";
			break;
		case XMLReader::kText:
			stream << "Text";
			break;
		case XMLReader::kWhitespace:
			stream << "Whitespace";
			break;
		case XMLReader::kXmlDeclaration:
			stream << "XmlDeclaration";
			break;
        default:
            stream << "Unknown";
            break;
    }
    return stream;
}

int main()
{
	FileInputStream inputStream("/Users/samalone/Downloads/Saratoga Little League.lbe");
	XMLReader reader(inputStream);
	reader.SetWhitespaceHandling(XMLReader::kWhitespaceHandlingNone);
	
	int i = 0;
	while (reader.Read()) {
	    cout << reader.GetNodeType();
	    switch (reader.GetNodeType()) {
	        case XMLReader::kElement:
	        case XMLReader::kEndElement:
	            cout << " " << reader.GetLocalName() << " " << reader.GetNamespaceURI();
	            break;
	        case XMLReader::kText:
	            cout << " " << reader.GetValue();
	            break;
	    }
	    cout << endl;
	    if (++i > 40) break;
	}
	
	return 0;
}