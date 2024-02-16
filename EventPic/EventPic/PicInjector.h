#pragma once
#include <main.h>



class PicInjector
{
public:
	PicInjector(DWORD Pid, LPCVOID PicPointer, SIZE_T PicSize) ;
	~PicInjector();
	bool SignalEvent();
	bool ReleasePicMem();
	bool IsOpen();
	bool Inject();
private:
	HANDLE _phandle;
	LPCVOID _ptpic;
	LPVOID _remotepic;
	SIZE_T _sizepic;
	HANDLE _eventh;
};