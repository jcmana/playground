#include "defines.h"
#include "create.h"
#include "wait.h"
#include "thread.h"

int main(void)
{
	SPipeHandle sPipe;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\fuckloadpipe");

	int	ret = RET_SUCCESS;

	ret = Create(sPipe, lpszPipename);
	if (ret != RET_SUCCESS) return ret;

	ret = WaitForClient(sPipe);
	if (ret != RET_SUCCESS) return ret;

	ret = HandleRequest(sPipe);

	return RET_SUCCESS;
}
