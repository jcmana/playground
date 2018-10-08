#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

int main(void)
{
	HANDLE	hPipe;

	LPTSTR	lpvMessage = TEXT("Default message from client.");
	TCHAR	chBuf[BUFSIZE];
	BOOL	fSuccess = FALSE;

	DWORD	cbRead;
	DWORD	cbToWrite;
	DWORD	cbWritten;
	DWORD	dwMode;

	LPTSTR	lpszPipename = TEXT("\\\\.\\pipe\\fuckloadpipe");


	// Try to open a named pipe; wait for it, if necessary
	while (1)
	{
		// Open named pipe handle
		hPipe = CreateFile(
			lpszPipename,	// pipe name 
			GENERIC_READ |	// read and write access 
			GENERIC_WRITE,
			0,				// no sharing 
			NULL,			// default security attributes
			OPEN_EXISTING,	// opens existing pipe 
			0,				// default attributes 
			NULL			// no template file 
		);

		// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE)
		{
			break;
		}

		// Exit if an error other than ERROR_PIPE_BUSY occurs 
		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds 
		BOOL bWaitPipe = WaitNamedPipe(lpszPipename, 20000);

		if (bWaitPipe == false)
		{
			return -1;
		}
	}


	// The pipe connected; change to message-read mode 
	dwMode = PIPE_READMODE_MESSAGE;

	fSuccess = SetNamedPipeHandleState(
		hPipe,		// pipe handle 
		&dwMode,	// new pipe mode 
		NULL,		// don't set maximum bytes 
		NULL		// don't set maximum time 
	);

	if (fSuccess == false)
	{
		return -1;
	}


	// Send a message to the pipe server
	cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);

	fSuccess = WriteFile(
		hPipe,              // pipe handle 
		lpvMessage,         // message 
		cbToWrite,          // message length 
		&cbWritten,         // bytes written 
		NULL				// not overlapped
	);

	if (fSuccess == false)
	{
		return -1;
	}


	// Read from the pipe
	do
	{
		fSuccess = ReadFile(
			hPipe,			// pipe handle 
			chBuf,			// buffer to receive reply 
			BUFSIZE * sizeof(TCHAR),  // size of buffer 
			&cbRead,		// number of bytes read 
			NULL			// not overlapped
		);

		if (fSuccess == false)
		{
			if (GetLastError() != ERROR_MORE_DATA)
			{
				break;
			}
		}
	}
	while (fSuccess == false);


	// Close named pipe handle
	CloseHandle(hPipe);


	return 0;
}
