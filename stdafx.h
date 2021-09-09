/*
	Developed by Michael Haephrati
	https://www.securedglobe.net
	https://www.freelancer.com/u/NYCyber

	January 2021

*/
#pragma once
#define UNICODE
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>
#include "curl/curl.h"
#include <regex>
#include <shlwapi.h>
#include <atlbase.h>
#include <atlstr.h>
#include <atltime.h>
#include <time.h>
#include <set>
#include <chrono>
#include <gdiplus.h>
#include <ShellScalingApi.h>
// Libs
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "libcurl_a.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "Shcore.lib")
#ifdef _DEBUG
#pragma comment(lib, "libcurl_a_debug.lib")
#else
#pragma comment(lib, "libcurl_a.lib")
#endif
// DirectX
#include <d3d9.h>
#include <d3d11.h>
#include <dxgi1_2.h>

using namespace std;
