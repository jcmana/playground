#pragma once

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "defines.h"
#include "pipehandle.h"

int Create(SPipeHandle & sPipe, LPTSTR lpszPipename);
