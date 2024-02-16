#include <main.h>



PicInjector::PicInjector(DWORD Pid, LPCVOID PicPointer, SIZE_T PicSize)
{
	_phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	_ptpic = PicPointer;
	_sizepic = PicSize;
	_eventh = CreateEventW(nullptr, FALSE, FALSE, SharedEvent);

}

PicInjector::~PicInjector()
{
	CloseHandle(_phandle);
	CloseHandle(_eventh);
}

bool PicInjector::SignalEvent()
{
	return SetEvent(_eventh);
}

bool PicInjector::IsOpen()
{
	if (_phandle)
		return true;
	return false;
}

bool PicInjector::Inject()
{

	// write pic
	LPVOID RemoteAddr = VirtualAllocEx(_phandle, nullptr, _sizepic, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!RemoteAddr)
		return false;
	bool status = WriteProcessMemory(_phandle, RemoteAddr, _ptpic, _sizepic, nullptr);
	if (!status)
		return false;
	_remotepic = RemoteAddr;


	// write strings 
	LPCSTR BoxText = "pic triggered";
	LPCWSTR EventName = SharedEvent;
	size_t BoxLen = strlen(BoxText);
	size_t EventLen = wcslen(EventName) * sizeof(WCHAR);
	LPVOID RemoteBoxStrAddr = VirtualAllocEx(_phandle, nullptr, BoxLen, MEM_COMMIT, PAGE_READWRITE);
	if (!RemoteBoxStrAddr)
		return false;
	 status = WriteProcessMemory(_phandle, RemoteBoxStrAddr, BoxText, BoxLen, nullptr);
	if (!status)
		return false;
	LPVOID RemoteEventStrAddr = VirtualAllocEx(_phandle, nullptr, EventLen, MEM_COMMIT, PAGE_READWRITE);
	if (!RemoteEventStrAddr)
		return false;
	 status = WriteProcessMemory(_phandle, RemoteEventStrAddr, EventName, EventLen, nullptr);
	if (!status)
		return false;

	// write params 

	PIC_PARAMS params;
	params.BoxTextStrPtr = RemoteBoxStrAddr;
	params.EventNameStrPtr = RemoteEventStrAddr;
	params.MessageBoxAddress = &MessageBoxA;
	params.VirtualFreeExAddress = &VirtualFreeEx;
	params.OpenEventAddress = &OpenEventW;
	params.CloseHandleAddr = &CloseHandle;
	params.WaitForSingleObjectAddr = &WaitForSingleObject;
	params.PicAllocationAddr = RemoteAddr;
	params.PicSize = _sizepic;

	LPVOID ParamsRemoteAddr = VirtualAllocEx(_phandle, nullptr, sizeof(PIC_PARAMS), MEM_COMMIT, PAGE_READWRITE);
	if (!ParamsRemoteAddr)
		return false;

	status = WriteProcessMemory(_phandle, ParamsRemoteAddr, (LPCVOID)&params, sizeof(PIC_PARAMS), nullptr);
	if (!status)
		return false;

	HANDLE ThreadHandle = CreateRemoteThread(_phandle, nullptr, NULL, (LPTHREAD_START_ROUTINE)RemoteAddr, ParamsRemoteAddr, NULL, nullptr);
	if (!ThreadHandle)
		return false;
	
	return true;
}


bool PicInjector::ReleasePicMem()
{
	return VirtualFreeEx(_phandle, _remotepic, 0, MEM_RELEASE);
}