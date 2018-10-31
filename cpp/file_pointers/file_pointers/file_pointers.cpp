#include "stdafx.h"

#include "Windows.h"

#include <thread>
#include <mutex>

void thread_procedure()
{
	HANDLE	hFile;
	DWORD	hPointer;
	LPCTSTR sWriteBuffer;

	// Open existing file
	hFile = ::CreateFile(
		L"D:\\thrash\\file.txt",		// The name of the file or device to be created or opened
		GENERIC_WRITE | GENERIC_READ,	// The requested access
		FILE_SHARE_READ,				// The requested sharing mode
		NULL,							// A pointer to a SECURITY_ATTRIBUTES structure
		OPEN_EXISTING,					// An action to take on a file or device
		FILE_FLAG_WRITE_THROUGH,		// The file or device attributes and flags
		NULL							// A valid handle to a template file with the GENERIC_READ access right
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	// Get file internal pointer
	hPointer = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	// Increament file internal pointer
	hPointer = ::SetFilePointer(hFile, 1, NULL, FILE_CURRENT);

	// Write one byte to the file
	sWriteBuffer = L"xyz";

	::WriteFile(
		hFile,			// A handle to the file or I/O device
		sWriteBuffer,	// A pointer to the buffer containing the data to be written
		1,				// The number of bytes to be written
		NULL,			// A pointer to the variable that receives the number of bytes written
		NULL			// A pointer to an OVERLAPPED structure
	);

	::WriteFile(
		hFile,			// A handle to the file or I/O device
		sWriteBuffer,	// A pointer to the buffer containing the data to be written
		1,				// The number of bytes to be written
		NULL,			// A pointer to the variable that receives the number of bytes written
		NULL			// A pointer to an OVERLAPPED structure
	);
	
	::FlushFileBuffers(hFile);

	// Close the file handle
	::CloseHandle(hFile);
}

int main()
{
	std::thread thread_A(thread_procedure);
	std::thread thread_B(thread_procedure);

	thread_A.join();
	thread_B.join();

    return 0;
}
