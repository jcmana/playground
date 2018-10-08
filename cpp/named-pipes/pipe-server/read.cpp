#include "read.h"

int Read(HANDLE hPipe, TCHAR * pchRequest, DWORD cbBytesRead)
{
	BOOL fSuccess = FALSE;

	// Loop until done reading
	while (1)
	{
		// Read client requests from the pipe. This simplistic code only allows messages
		// up to BUFSIZE characters in length.
		fSuccess = ReadFile(
			hPipe,				// handle to pipe 
			pchRequest,			// buffer to receive data 
			BUFFER_SIZE * sizeof(TCHAR),	// size of buffer 
			&cbBytesRead,		// number of bytes read 
			NULL				// not overlapped I/O
		);

		if (!fSuccess || cbBytesRead == 0)
		{
			break;
		}
	}

	return RET_SUCCESS;
}