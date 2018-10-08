// kernel-routines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _X86_

//#include <windows.h>
#include <wdm.h>


#include <ntstatus.h>
#include <ntdef.h>
#include <ntstrsafe.h>
#include <ntddk.h>


#define ERROR_OBJECT_NAME_INIT -10

int main()
{
	return 0;

	NTSTATUS status;

	// Setup filename UNICODE string
	UNICODE_STRING sObjectName;
	status = RtlUnicodeStringInit(
		/* DestinationString */		&sObjectName,
		/* pszSrc */				_T("\\DosDevices\\D:\\issues\\AV-14228\\target\\natives_blob.bin")
	);

	if (NT_ERROR(status))
		return ERROR_OBJECT_NAME_INIT;

	// Open file handle
	OBJECT_ATTRIBUTES sObjectAttributes;
	InitializeObjectAttributes(
		/* InitializedAttributes */ &sObjectAttributes,
		/* ObjectName */			&sObjectName,
		/* Attributes */			OBJ_INHERIT,
		/* RootDirectory */			NULL,
		/* SecurityDescriptor */	NULL
	);

	IO_STATUS_BLOCK sOpenFileStatus;

	HANDLE hFile;
	ZwOpenFile(
		/* FileHandle */			&hFile,
		/* DesiredAccess */			GENERIC_READ | GENERIC_WRITE,
		/* ObjectAttributes */		&sObjectAttributes,
		/* IoStatusBlock */			&sOpenFileStatus,
		/* ShareAccess */			NULL,
		/* OpenOptions */			FILE_RANDOM_ACCESS
	);

    return 0;
}

