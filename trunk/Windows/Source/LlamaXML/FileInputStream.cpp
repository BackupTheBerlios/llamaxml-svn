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

#include "LlamaXML/FileInputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	FileInputStream::FileInputStream(const wchar_t * path)
		: mFile(::CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, NULL))
	{
		if (mFile == INVALID_HANDLE_VALUE) ThrowXMLError(::GetLastError());
	}

	FileInputStream::~FileInputStream() {
		::CloseHandle(mFile);
	}

	uint32_t FileInputStream::ReadUpTo(char * buffer, uint32_t length) {
		DWORD bytesRead = 0;
		if (! ::ReadFile(mFile, buffer, length, &bytesRead, NULL)) {
			ThrowXMLError(::GetLastError());
		}
		return bytesRead;
	}

	void FileInputStream::Restart() {
		DWORD result = ::SetFilePointer(mFile, 0, NULL, FILE_BEGIN);
		if (result == INVALID_FILE_SIZE) {
			ThrowIfXMLError(::GetLastError());
		}
	}

	bool FileInputStream::EndOfFile() {
		DWORD fileSize = ::GetFileSize(mFile, NULL);
		if (fileSize == INVALID_FILE_SIZE) {
			ThrowIfXMLError(::GetLastError());
		}
		DWORD filePosition = ::SetFilePointer(mFile, 0, NULL, FILE_CURRENT);
		if (filePosition == INVALID_FILE_SIZE) {
			ThrowIfXMLError(::GetLastError());
		}
		return filePosition >= fileSize;
	}
}
