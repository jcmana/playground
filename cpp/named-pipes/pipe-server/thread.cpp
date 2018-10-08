#include "thread.h"

int HandleRequest(SPipeHandle & sPipe)
{
	HANDLE hThread = NULL;
	DWORD dwThreadID = 0;

	hThread = CreateThread(
		NULL,					// no security attribute 
		0,						// default stack size 
		ServerThreadProcedure,	// thread proc
		(LPVOID)(sPipe.handle),	// thread parameter 
		0,						// not suspended 
		&dwThreadID);			// returns thread ID 

	return RET_SUCCESS;
}

DWORD WINAPI ServerThreadProcedure(LPVOID lpvParam)
{
	HANDLE hHeap = GetProcessHeap();
	TCHAR * pchRequest = (TCHAR*)(HeapAlloc(hHeap, 0, BUFFER_SIZE * sizeof(TCHAR)));
	TCHAR * pchResponse = (TCHAR*)(HeapAlloc(hHeap, 0, BUFFER_SIZE * sizeof(TCHAR)));

	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
	BOOL fSuccess = FALSE;

	HANDLE hPipe = NULL;

	// Do some extra error checking since the app will keep running even if this
	// thread fails.
	if (lpvParam == NULL)
	{
		if (pchResponse != NULL) HeapFree(hHeap, 0, pchResponse);
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	if (pchRequest == NULL)
	{
		if (pchResponse != NULL) HeapFree(hHeap, 0, pchResponse);
		return (DWORD)-1;
	}

	if (pchResponse == NULL)
	{
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}


	// The thread's parameter is a handle to a pipe object instance. 
	hPipe = (HANDLE)(lpvParam);

	// Read client request from pipe
	Read(hPipe, pchRequest, cbBytesRead);

	// Flush the pipe to allow the client to read the pipe's contents 
	// before disconnecting. Then disconnect the pipe, and close the 
	// handle to this pipe instance. 

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	HeapFree(hHeap, 0, pchRequest);
	HeapFree(hHeap, 0, pchResponse);

	return RET_SUCCESS;
}