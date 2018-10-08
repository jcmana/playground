#pragma once

#include <windows.h>

#include "defines.h"
#include "pipehandle.h"
#include "read.h"

int HandleRequest(SPipeHandle & sPipe);

DWORD WINAPI ServerThreadProcedure(LPVOID lpvParam);