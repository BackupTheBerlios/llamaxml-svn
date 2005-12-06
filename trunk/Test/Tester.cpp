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
#include "LlamaXML/StdIO.h"
#include <stdio.h>

Tester * Tester::sCurrentTester = 0;

Tester::Tester()
: mParentTester(sCurrentTester),
  mSuccessCount(0),
  mFailureCount(0)
{
	sCurrentTester = this;
}

Tester::~Tester()
{
	sCurrentTester = mParentTester;
}

void Tester::Test(TestFunction * testFunction, const char * name) {
	TestBlock block(name);
	(*testFunction)(*this);
}

void Tester::Assert(bool success, const char * file, int line) {
	if (success) {
		Success();
	}
	else {
		char buffer[1024];
		LlamaXML::snprintf(buffer, sizeof(buffer), "file %s line %d", file, line);
		Failure(buffer);
	}
}

void Tester::Assert(bool success, const char * message) {
	if (success) {
		Success();
	}
	else {
		Failure(message);
	}
}

void Tester::Success() {
	++mSuccessCount;
}

void Tester::Failure(const char * msg) {
	++mFailureCount;
	TestBlock * block = TestBlock::GetCurrentTestBlock();
	if (block) block->Display();
	char buffer[1024];
	LlamaXML::snprintf(buffer, sizeof(buffer), "Test failure: %s",
		msg);
	WriteLine(buffer);
}

void Tester::Failure(const char * file, int line, std::exception & e) {
	char buffer[1024];
	LlamaXML::snprintf(buffer, sizeof(buffer), "exception \"%s\" caught at file %s line %d",
		e.what(), file, line);
	Failure(buffer);
}

void Tester::Failure(const char * file, int line, LlamaXML::XMLException & e) {
	char buffer[1024];
	LlamaXML::snprintf(buffer, sizeof(buffer), "%s caught at file %s line %d",
		e.what(), file, line);
	Failure(buffer);
}

void Tester::WriteResults() {
	char buffer[1024];
	LlamaXML::snprintf(buffer, sizeof(buffer), "%d tests passed", mSuccessCount);
	WriteLine(buffer);
	LlamaXML::snprintf(buffer, sizeof(buffer), "%d tests failed", mFailureCount);
	WriteLine(buffer);
}

void Tester::Message(const char * msg) {
#if defined(PALMOS_SDK_VERSION)
	::DbgMessage(msg);
#else
	fputs(stderr, msg);
#endif
}

void Tester::WriteLine(int level, const char * msg) {
	for (int i = 0; i < level; ++i) {
		Message("    ");
	}
	Message(msg);
	Message("\n");
}

void Tester::WriteLine(const char * msg) {
	TestBlock * block = TestBlock::GetCurrentTestBlock();
	WriteLine(block ? block->GetLevel() : 0, msg);
}


TestBlock * TestBlock::sCurrentTestBlock = 0;


TestBlock::TestBlock()
: mParentTestBlock(sCurrentTestBlock),
  mLevel(1),
  mDisplayed(false)
{
	if (mParentTestBlock) {
		mLevel = mParentTestBlock->GetLevel() + 1;
	}
	sCurrentTestBlock = this;
}


TestBlock::TestBlock(const char * msg)
: mParentTestBlock(sCurrentTestBlock),
  mLevel(1),
  mMessage(msg),
  mDisplayed(false)
{
	if (mParentTestBlock) {
		mLevel = mParentTestBlock->GetLevel() + 1;
	}
	sCurrentTestBlock = this;
}


TestBlock::~TestBlock() {
	sCurrentTestBlock = mParentTestBlock;
}


void TestBlock::Display() {
	if (mParentTestBlock) {
		mParentTestBlock->Display();
	}
	if (! mDisplayed) {
		mDisplayed = true;
		Tester::GetCurrentTester()->WriteLine(mLevel - 1, mMessage.c_str());
	}
}


TestBlock & operator << (TestBlock & block, int n) {
	char buffer[32];
	LlamaXML::snprintf(buffer, sizeof(buffer), "%d", n);
	block << buffer;
	return block;
}
