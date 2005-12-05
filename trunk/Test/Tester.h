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

#include "LlamaXML/XMLException.h"

class Tester {
public:
	Tester();
	~Tester();
	
	typedef void TestFunction(Tester & tester);
	
	void Test(TestFunction * testFunction);
	
	void Assert(bool success, const char * file, int line);
	void Assert(bool success, const char * message);
	
	void Failure(const char * file, int line, std::exception & e);
	void Failure(const char * file, int line, LlamaXML::XMLException & e);
	
	void WriteResults();
	
	void WriteLine(const char * msg);
	
private:
	int		mSuccessCount;
	int		mFailureCount;
};

#define TestAssert(x) try { tester.Assert(x, __FILE__, __LINE__); } catch (LlamaXML::XMLException & e) { tester.Failure(__FILE__, __LINE__, e); } catch (std::exception & e) { tester.Failure(__FILE__, __LINE__, e); }

#endif
