#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <strsafe.h>
#include <synchapi.h>

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#include "detours.h"



#include "a3hooks.h"



#define MODULENAME TEXT("a3hooks.dll - " DETOURS_STRINGIFY(DETOURS_BITS) "bit")


VOID PrintMessage(LPCTSTR pszFormat, ...);
VOID PrintMessageA(LPCSTR pszFormat, ...);

#define TRACE(_message_, ...)\
	PrintMessage(TEXT("a3hooks.dll!")_message_, __VA_ARGS__)

#define TRACEA(_message_, ...)\
	PrintMessageA("a3hooks.dll!"_message_, __VA_ARGS__)