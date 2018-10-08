#include "wait.h"

int WaitForClient(SPipeHandle & sPipe)
{
	BOOL fConnected = ConnectNamedPipe(sPipe.handle, NULL);

	if (fConnected)
	{
		return RET_SUCCESS;
	}
	else
	{
		// Client already connected
		if (GetLastError() == ERROR_PIPE_CONNECTED)
		{
			return RET_SUCCESS;
		}
	}

	return RET_PIPE_CONNECT_FAILED;
}