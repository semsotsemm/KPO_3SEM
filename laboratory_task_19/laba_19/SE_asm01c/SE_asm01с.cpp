#include <iostream>
#pragma comment(lib, "..\\Debug\\SE_asm01a.lib")

extern "C" {
	int __stdcall getmin(int*, int);
	int __stdcall getmax(int*, int);
}

int main()
{
	int arr[] = { 15, 2, 9, 42, 5, 61, 7, 8, 9, 10 };
	int min = getmin(arr, sizeof(arr) / sizeof(int));
	int max = getmax(arr, sizeof(arr) / sizeof(int));
	std::cout << "getmin+getmax=" << min + max;
}

