#pragma once
#include <main.h>

DWORD PicFunction(LPCVOID Param);
void PicEnd();

extern LPCWSTR SharedEvent;


typedef struct _PIC_PARAMS
{
	LPVOID PicAllocationAddr; 
	LPVOID MessageBoxAddress;
	LPVOID VirtualFreeExAddress;
	LPVOID OpenEventAddress;
	LPVOID WaitForSingleObjectAddr;
	LPVOID CloseHandleAddr;
	LPVOID EventNameStrPtr;
	LPVOID BoxTextStrPtr;
	SIZE_T PicSize;
}PIC_PARAMS, * PPIC_PARAMS;


typedef int (WINAPI* PMessageBoxA)(
	HWND   hWnd,
	LPCSTR lpText,
	LPCSTR lpCaption,
	UINT   uType
	);


typedef BOOL(WINAPI* PVirtualFreeEx)(
	HANDLE hProcess,
	LPVOID lpAddress,
	SIZE_T dwSize,
	DWORD dwFreeType
	);
typedef HANDLE(WINAPI* POpenEvent)(
	DWORD dwDesiredAccess,
	BOOL bInheritHandle,
	LPCWSTR lpName
	);

typedef DWORD(WINAPI* PWaitForSingleObject)(
	HANDLE hHandle,
	DWORD dwMilliseconds
	);


typedef BOOL(WINAPI* PCloseHandle)(
	HANDLE hHandle
	);