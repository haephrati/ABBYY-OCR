/*

	Developed by Michael Haephrati
	https://www.securedglobe.net
	https://www.freelancer.com/u/NYCyber

	January 2021

*/
#include "stdafx.h"
#include "utils.h"
#include "ABBYY_OCR_Main.h"
#include "ABBYY_OCR.h"
#include "resource.h"

HANDLE hConsole;
WNDCLASS wc = { };

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void StartConsole()
{
	AllocConsole();
	SetConsoleTitle(L"ABBYY OCR C++ Demo by Michael Haephrati");
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPWSTR    lpCmdLine,
	int       nCmdShow)
{


	LPWSTR *szArglist{ NULL };
	int nArgs;

	StartConsole();

	// Ensuring the software will only run once at the same time
	HANDLE hMutex;
	hMutex = CreateMutex(
		NULL,					// default security descriptor
		FALSE,                  // mutex not owned
		APP_NAME);				// object name

	if (hMutex == NULL)
		WriteLogFile(L"Error creating Mutext", GetLastError());
	else
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		WriteLogFile(L"Software already running\n");
		exit(0);
	}
	// =========================================================

	// Creating main window
	const wchar_t CLASS_NAME[] = L"ABBYY_OCR_Demo";

	TCHAR szExeFileName[MAX_PATH];
	GetModuleFileName(NULL, szExeFileName, MAX_PATH);
	PathStripPath(szExeFileName);

	wcscpy(LOGFILENAME, DEFLOGFILENAME);
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"AbbyyOCRDemo",				// Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// =========================================================

	RECT rectWorkArea;
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	::GetMonitorInfo(::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi);
	rectWorkArea = mi.rcWork;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND Handle = GetConsoleWindow();
	SetWindowLong(Handle, GWL_EXSTYLE, GetWindowLong(Handle, GWL_EXSTYLE) | WS_EX_LAYERED);

	// Set icon
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOGO));
	SendMessage(Handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

	// Set posision
	::SetWindowPos(Handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::ShowWindow(Handle, SW_NORMAL);
	RECT rect = { 0, rectWorkArea.bottom - 150, rectWorkArea.right, rectWorkArea.bottom };
	MoveWindow(Handle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd , 0, 90, LWA_ALPHA);

	setcolor(LOG_COLOR_GREEN,0);
	WriteLogFile(L"Welcome to %s", szExeFileName);
	setcolor(LOG_COLOR_YELLOW,0);


	// Handling Command Line Arguments
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (szArglist == NULL)
	{
		WriteLogFile(L"CommandLineToArgvW failed\n");
		return 0;
	}
	else
	{
		if (nArgs < 2)
		{
			WriteLogFile(L"%s <image file> <output file>\n", szArglist[0]);
			system("pause");
			exit(0);
		}
		if (PathFileExists(szArglist[1]) == FALSE)
		{
			WriteLogFile(L"Error: input file '%s' does not exist", szArglist[1]);
			exit(0);
		}
		WriteLogFile(L"Input Image: %ws\n",szArglist[1]);
		WriteLogFile(L"Output File: %ws\n", szArglist[2]);
	}

	// =========================================================



	SG_OCR(szArglist[1], szArglist[2]);

	system("pause");
	exit(0);
	return 0;
}