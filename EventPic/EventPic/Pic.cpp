#include <main.h>


#pragma code_seg(".text$func_a")
DWORD PicFunction(LPCVOID Param)
{
	DWORD WaitStat;
	PPIC_PARAMS params = (PPIC_PARAMS)Param;
	PVirtualFreeEx Free = (PVirtualFreeEx)params->VirtualFreeExAddress;
	PMessageBoxA PopBox = (PMessageBoxA)params->MessageBoxAddress; // we can assume user32 functions are loaded to the same address in each process 
	POpenEvent OpEvent = (POpenEvent)params->OpenEventAddress;
	PWaitForSingleObject Wait = (PWaitForSingleObject)params->WaitForSingleObjectAddr;
	LPCVOID PicAddr = params->PicAllocationAddr;
	SIZE_T  PicSize = params->PicSize;
	LPCWSTR EventName = (LPCWSTR)params->EventNameStrPtr;
	LPCSTR BoxText = (LPCSTR)params->BoxTextStrPtr;
	HANDLE hEvent = OpEvent(SYNCHRONIZE, FALSE, EventName);
	if (hEvent)
	{
		while (true)
		{
			WaitStat = Wait(hEvent, 10000);
			if (WaitStat == WAIT_OBJECT_0)
				break;
			PopBox(NULL, BoxText, BoxText, MB_OK);
		}
	}
	Free((HANDLE)-1, (LPVOID)params, sizeof(PIC_PARAMS), MEM_RELEASE);

	return 0;
}
#pragma code_seg(".text$func_b")
void PicEnd() {};

