#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef int (*ImportFunction)(char*);

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);

LPCSTR szClassName = "ControlSource";
LPCSTR szTitle = "Управление ресурсами в вычислительных системах. РГЗ, Вариант 5";

char Info[256];

DWORD WINAPI ThreadFunc(void*) {
	ImportFunction DLLInfo;
	HINSTANCE hinstLib = LoadLibrary(TEXT("info.dll"));
	DLLInfo = (ImportFunction)GetProcAddress(hinstLib, "Information");
	DLLInfo(Info);
	FreeLibrary(hinstLib);
	return 0;
}

LRESULT CALLBACK WindowFunc(HWND hWind, UINT msg, WPARAM wParam, LPARAM lParam) {
	HANDLE hThread;
	DWORD IDThread;
	PAINTSTRUCT ps;
	HDC hDC;
	switch (msg) {
	case WM_CREATE:
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &IDThread);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWind, &ps);
		TextOut(hDC, 10, 10, Info, strlen(Info));
		EndPaint(hWind, &ps);
		break;
	default:
		return DefWindowProc(hWind, msg, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hThisInst,HINSTANCE hPrevInst, LPSTR str, int nWinMode){

	MSG msg;
	WNDCLASS wcl;
	HWND hWind;

	wcl.hInstance = hThisInst;

	wcl.lpszClassName = szClassName;
	wcl.lpfnWndProc = WindowFunc;

	wcl.style = CS_HREDRAW | CS_VREDRAW;
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcl.lpszMenuName = NULL;

	wcl.cbClsExtra = 0;

	wcl.cbWndExtra = 0;

	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	RegisterClass(&wcl);

	hWind = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 300, 50, 570, 220, HWND_DESKTOP, NULL, hThisInst, NULL);
	ShowWindow(hWind, nWinMode);
	UpdateWindow(hWind);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


