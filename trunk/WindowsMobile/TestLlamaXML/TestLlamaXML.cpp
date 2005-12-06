// TestLlamaXML.cpp : Defines the entry point for the console application.
//

#include "Tester.h"
#include "TestTextEncoding.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Tester tester;
	
	tester.Test(TestTextEncoding, "TextEncoding");
	
	tester.WriteResults();

	return 0;
}

