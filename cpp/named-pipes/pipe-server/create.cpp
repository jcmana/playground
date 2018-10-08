#include "create.h"

int Create(SPipeHandle & sPipe, LPTSTR lpszPipename)
{
	// Create new named pipe
	sPipe.handle = INVALID_HANDLE_VALUE;

	sPipe.handle = CreateNamedPipe(
		/* lpName */				lpszPipename,				// pipe name
		/* dwOpenMode */			PIPE_ACCESS_DUPLEX,			// read/write access 
		/* dwPipeMode */			PIPE_TYPE_MESSAGE |			// message type pipe
									PIPE_READMODE_MESSAGE |		// message-read mode 
									PIPE_WAIT,					// blocking mode
		/* nMaxInstances */			PIPE_UNLIMITED_INSTANCES,
		/* nOutBufferSize */		BUFFER_SIZE,
		/* nInBufferSize */			BUFFER_SIZE,
		/* nDefaultTimeOut */		0,
		/* lpSecurityAttributes */	NULL						// default security attributes
	);

	if (sPipe.handle == INVALID_HANDLE_VALUE)
	{
		return RET_PIPE_CREATE_FAILED;
	}

	return RET_SUCCESS;
}