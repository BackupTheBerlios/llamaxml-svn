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

#ifndef LLAMAXML_TESTER_H
#define LLAMAXML_TESTER_H

#if (! __GNUC__) || __APPLE__
#pragma once
#endif

#include "LlamaXML/PlatformConfig.h"
#include "LlamaXML/XMLException.h"
#include "LlamaXML/UnicodeString.h"

class Tester {
public:
	Tester();
	~Tester();
	
	typedef void TestFunction(Tester & tester);
	
	void Test(TestFunction * testFunction, const char * name);
	
	void Assert(bool success, const char * file, int line);
	void Assert(bool success, const char * message);
	
	void Success();
	void Failure(const char * msg);
	void Failure(const char * file, int line, std::exception & e);
	void Failure(const char * file, int line, LlamaXML::XMLException & e);
	
	void WriteResults();
	
	void WriteLine(int level, const char * msg);
	void WriteLine(const char * msg);

	static Tester * GetCurrentTester() {
		return sCurrentTester;
	}

private:
	static void Message(const char * msg);
	
private:
	static Tester *		sCurrentTester;

	Tester *	mParentTester;
	int			mSuccessCount;
	int			mFailureCount;
};

class TestBlock {
public:
	TestBlock();
	TestBlock(const char * msg);
	~TestBlock();

	int GetLevel() const {
		return mLevel;
	}

	const char * GetMessage() const {
		return mMessage.c_str();
	}

	void AppendMessage(const char * msg) {
		mMessage += msg;
	}

	void Display();

	static TestBlock * GetCurrentTestBlock() {
		return sCurrentTestBlock;
	}

private:
	static TestBlock *	sCurrentTestBlock;

	std::string			mMessage;
	int					mLevel;
	bool				mDisplayed;
	TestBlock *			mParentTestBlock;
};

inline TestBlock & operator << (TestBlock & block, const char * msg) {
	block.AppendMessage(msg);
	return block;
}

TestBlock & operator << (TestBlock & block, int n);

#define TestAssert(x) try { tester.Assert(x, __FILE__, __LINE__); } catch (LlamaXML::XMLException & e) { tester.Failure(__FILE__, __LINE__, e); } catch (std::exception & e) { tester.Failure(__FILE__, __LINE__, e); }

#endif
