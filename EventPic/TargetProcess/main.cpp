#include <iostream>
#include <Windows.h>


int main()
{
	MessageBoxA(NULL, "Target Process Hello", "target", MB_OK);
	std::cout << "[*] hello world " << std::endl;
	Sleep(INFINITE);

	return 0; 

}