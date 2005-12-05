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

#include "Tester.h"
#include <stdio.h>

Tester::Tester()
: mSuccessCount(0),
  mFailureCount(0)
{
}

Tester::~Tester()
{
}

void Tester::Test(TestFunction * testFunction) {
	(*testFunction)(*this);
}

void Tester::Assert(bool success, const char * file, int line) {
	if (success) {
		++mSuccessCount;
	}
	else {
		++mFailureCount;
		char buffer[1024];
		snprintf(buffer, sizeof(buffer), "Test failure file %s line %d", file, line);
		WriteLine(buffer);
	}
}

void Tester::Assert(bool success, const char * message) {
	if (success) {
		++mSuccessCount;
	}
	else {
		++mFailureCount;
		char buffer[1024];
		snprintf(buffer, sizeof(buffer), "Test failure: %s", message);
		WriteLine(buffer);
	}
}

void Tester::Failure(const char * file, int line, std::exception & e) {
	++mFailureCount;
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "Test failure: exception \"%s\" caught at file %s line %d",
		e.what(), file, line);
	WriteLine(buffer);
}

void Tester::Failure(const char * file, int line, LlamaXML::XMLException & e) {
	++mFailureCount;
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "Test failure: %s caught at file %s line %d",
		e.what(), file, line);
	WriteLine(buffer);
}

void Tester::WriteResults() {
	char buffer[1024];
	snprintf(buffer, sizeof(buffer), "%d tests passed", mSuccessCount);
	WriteLine(buffer);
	snprintf(buffer, sizeof(buffer), "%d tests failed", mFailureCount);
	WriteLine(buffer);
}

void Tester::WriteLine(const char * msg) {
	fprintf(stderr, "%s\n", msg);
}
