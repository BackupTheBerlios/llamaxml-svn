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
		LARGE_INTEGER zero;
		zero.QuadPart = 0;
		if (! ::SetFilePointerEx(mFile, zero, NULL, FILE_BEGIN)) {
			ThrowXMLError(::GetLastError());
		}
	}

	bool FileInputStream::EndOfFile() {
		LARGE_INTEGER zero;
		zero.QuadPart = 0;
		LARGE_INTEGER fileSize;
		if (! ::GetFileSizeEx(mFile, &fileSize)) {
			ThrowXMLError(::GetLastError());
		}
		LARGE_INTEGER filePosition;
		if (! ::SetFilePointerEx(mFile, zero, &filePosition, FILE_CURRENT)) {
			ThrowXMLError(::GetLastError());
		}
		return filePosition.QuadPart >= fileSize.QuadPart;
	}
}
