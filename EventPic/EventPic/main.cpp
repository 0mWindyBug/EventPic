#include <main.h>

LPCWSTR SharedEvent = L"PicEvent";


int main()
{
	int trigger;
	bool status;

	DWORD TargetPid;
	std::cout << "[*] enter the pid to inject pic to : ";
	std::cin >> TargetPid;
	
	SIZE_T PicSize = (SIZE_T)((ULONG_PTR)&PicEnd - (ULONG_PTR)&PicFunction);
	std::cout << "[*] pic size : " << PicSize << std::endl;
	std::cout << std::hex << "[*] pic local address :" << &PicFunction << std::endl;

	PicInjector PIC(TargetPid, &PicFunction, PicSize);
	if (!PIC.IsOpen())
	{
		std::cout << "[*] failed to open process" << std::endl;
		return -1;
	}
	
	if (!PIC.Inject())
	{
		std::cout << "[*] injection failed" << std::endl;
		return -1;
	}
	std::cout << "[*] successfully injected pic" << std::endl;


	std::cout << "[*] type anything to signal pic for exit" << std::endl;
	std::cin >> trigger;
	if (!PIC.SignalEvent())
	{
		std::cout << "[*] failed to signal pic exit event" << std::endl;
		return -1;
	}
	std::cout << "[*] sucessfully signaled pic to exit" << std::endl;

	Sleep(15000);
	
	if (!PIC.ReleasePicMem())
	{
		std::cout << "[*] failed to release pic allocation" << std::endl;
		return -1;
	}
	std::cout << "[*] sucessfully released pic allcoation" << std::endl;

	return 0; 
}