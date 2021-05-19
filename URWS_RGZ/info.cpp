#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>


extern "C" __declspec(dllexport) int Information(char* InfoString) {
	sprintf(InfoString, "Ширина монитора в пикселях равна: %d", GetSystemMetrics(SM_CXFULLSCREEN));
	return 0;
}
