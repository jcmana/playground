#pragma once

#include <windows.h>

#include "defines.h"
#include "pipehandle.h"

/*
	Waits for client connection.

	Parameters:
		sPipe	- named pipe structure reference.
*/
int WaitForClient(SPipeHandle & sPipe);
