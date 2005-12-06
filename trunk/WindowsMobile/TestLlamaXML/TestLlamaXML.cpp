// TestLlamaXML.cpp : Defines the entry point for the console application.
//

#include "Tester.h"
#include "TestTextEncoding.h"
#include <Windows.h>
#include <Objbase.h>
#include <tchar.h>


int _tmain(int argc, _TCHAR* argv[])
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	Tester tester;
	
	tester.Test(TestTextEncoding, "TextEncoding");
	
	tester.WriteResults();

	CoUninitialize();

	return 0;
}

