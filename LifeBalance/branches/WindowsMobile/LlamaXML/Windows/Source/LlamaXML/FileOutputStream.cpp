#include "LlamaXML/FileOutputStream.h"
#include "LlamaXML/XMLException.h"

namespace LlamaXML {

	FileOutputStream::FileOutputStream(const wchar_t * path)
		: mFile(::CreateFileW(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, NULL))
	{
		if (mFile == INVALID_HANDLE_VALUE) ThrowXMLError(::GetLastError());
	}

	FileOutputStream::~FileOutputStream() {
		::CloseHandle(mFile);
	}

	void FileOutputStream::WriteData(const char * buffer, uint32_t length) {
		DWORD bytesWritten = 0;
		if ((! ::WriteFile(mFile, buffer, length, &bytesWritten, NULL)) || (bytesWritten < length)) {
			ThrowXMLError(::GetLastError());
		}
	}

}
