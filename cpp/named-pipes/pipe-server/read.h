#pragma once

#include <windows.h>

#include "pipehandle.h"
#include "defines.h"

int Read(HANDLE hPipe, TCHAR * pchRequest, DWORD cbBytesRead);
