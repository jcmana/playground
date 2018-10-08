#include "stdafx.h"

#include <windows.h>
#include <shobjidl.h>

int main()
{
	HRESULT hRes;

	IEnumAssocHandlers * assocHandlersEnum;
	hRes = SHAssocEnumHandlersForProtocolByApplication(L"https", IID_IEnumAssocHandlers, (void **)(&assocHandlersEnum));

	ULONG			assocHandlerCount = 0;
	IAssocHandler *	assocHandlerList[16];
	hRes = assocHandlersEnum->Next(16, assocHandlerList, &assocHandlerCount);

	LPWSTR assocHandlerName;
	for (ULONG i = 0; i < assocHandlerCount; i++)
	{
		hRes = assocHandlerList[i]->GetName(&assocHandlerName);
	}

	//::ShellExecute(NULL, L"open", L"chrome.exe", L"http://google.com", NULL, SW_SHOWDEFAULT);

	return 0;
}

