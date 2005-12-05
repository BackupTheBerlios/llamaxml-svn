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

int main(int argc, const char * argv[]) {
	Tester tester;
	
	tester.Test(TestTextEncoding);
	
	tester.WriteResults();
	
	return 0;
}
