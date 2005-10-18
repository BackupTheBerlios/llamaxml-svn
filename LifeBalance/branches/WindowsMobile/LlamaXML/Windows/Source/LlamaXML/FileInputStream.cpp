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
