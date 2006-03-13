/*
 *  TestLlamaXML.cpp
 *  LlamaXML
 *
 *  Created by Stuart A. Malone on 12/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "Tester.h"
#include "TestTextEncoding.h"
#include "LlamaXML/StringInputStream.h"
#include "LlamaXML/XMLReader.h"
#include "LlamaXML/UnicodeString.h"

int main(int argc, const char * argv[]) {
	Tester tester;
	
	tester.Test(TestTextEncoding, "TextEncoding");
	
	tester.WriteResults();
	
	LlamaXML::StringInputStream stream("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?> <foo>bar</foo>");

	LlamaXML::XMLReader reader(stream);

	reader.ReadStartElement("foo");

	LlamaXML::UnicodeString bar = reader.GetAttribute("bar");
	LlamaXML::UnicodeString foo = reader.ReadString();
	reader.ReadEndElement();

	
	return 0;
}
