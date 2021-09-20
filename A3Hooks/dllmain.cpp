// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"


AppData gAppData;


//
// supported command line arguments
//  -windowmode 1
//

BOOL InitAppData()
{
    BOOL r = FALSE;
	LPSTR lpszCmdLine;
	lpszCmdLine = GetCommandLineA();


    ZeroMemory(&gAppData, sizeof(gAppData));

	TRACEA("CommandLine: %s\n", lpszCmdLine);

    CHAR* p;
    
    p = strstr(lpszCmdLine, "-windowmode");

    if (p)
    {
        p += lstrlenA("-windowmode");

        while (*p++ == 0);


        gAppData.WindowMode = (1 == atoi(p)) ? TRUE: FALSE;
    }

    if (gAppData.WindowMode == TRUE)
    {
        p = strstr(lpszCmdLine, "-width");
        if (p)
        {
            p += lstrlenA("-width");

            while (*p++ == 0);


            gAppData.WindowWidth = atoi(p);
        }

        p = strstr(lpszCmdLine, "-height");
        if (p)
        {
            p += lstrlenA("-height");

            while (*p++ == 0);


            gAppData.WindowHeight = atoi(p);
        }
    }

    p = strstr(lpszCmdLine, "-host");
    if (p)
    {
        p += lstrlenA("-host");

        while (*p++ == 0);

        CHAR* q = gAppData.szHost;
        int i = 0;
        
        while (*p != ' ')
        {
            *q++ = *p++;
            i++;
            if (i >= INET_ADDRSTRLEN)
            {
                TRACE("ERROR: host IP to long\n");
                break;
            }
        }

    }

    p = strstr(lpszCmdLine, "-port");
    if (p)
    {
        p += lstrlenA("-port");

        while (*p++ == 0);


        gAppData.iPort = atoi(p);

    }


    TRACE("WindowMode = %d\n", gAppData.WindowMode);
    TRACE("WindowWidth = %d\n", gAppData.WindowWidth);
    TRACE("WindowHeight = %d\n", gAppData.WindowHeight);
    TRACEA("Host = %s:%d\n", gAppData.szHost, gAppData.iPort);

    r = TRUE;

    return r;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    LONG error;
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:

#if _DEBUG
        // for debug version, a message box help to break the process for debugger to attach
        MessageBox(NULL, TEXT("DllMain - attach!"), MODULENAME, MB_ICONINFORMATION);
#endif

        DetourRestoreAfterWith();

        TRACE("Starting..\n");

        if (!InitAppData())
        {
            MessageBox(NULL, TEXT("InitAppData failed!"), MODULENAME, MB_ICONHAND);
            return FALSE;
        }


        // Parse command line

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());


        TRACE("CreateMutexA, attach: %p to %p \n", CreateMutexA_original, CreateMutexA_hook);
        DetourAttach(&(PVOID&)CreateMutexA_original, CreateMutexA_hook);




        TRACE("CreateWindowExA, attach: %p to %p \n", CreateWindowExA_original, CreateWindowExA_hook);
        DetourAttach(&(PVOID&)CreateWindowExA_original, CreateWindowExA_hook);


        TRACE("CreateWindowExW, attach: %p to %p \n", CreateWindowExW_original, CreateWindowExW_hook);
        DetourAttach(&(PVOID&)CreateWindowExW_original, CreateWindowExW_hook);


        TRACE("ShowWindow, attach: %p to %p \n", ShowWindow_original, ShowWindow_hook);
        DetourAttach(&(PVOID&)ShowWindow_original, ShowWindow_hook);

        TRACE("MoveWindow, attach: %p to %p \n", MoveWindow_original, MoveWindow_hook);
        DetourAttach(&(PVOID&)MoveWindow_original, MoveWindow_hook);

        TRACE("SetWindowLongA, attach: %p to %p \n", SetWindowLongA_original, SetWindowLongA_hook);
        DetourAttach(&(PVOID&)SetWindowLongA_original, SetWindowLongA_hook);

        TRACE("ChangeDisplaySettingsA, attach: %p to %p \n", ChangeDisplaySettingsA_original, ChangeDisplaySettingsA_hook);
        DetourAttach(&(PVOID&)ChangeDisplaySettingsA_original, ChangeDisplaySettingsA_hook);



        TRACE("connect, attach: %p to %p \n", connect_original, connect_hook);
        DetourAttach(&(PVOID&)connect_original, connect_hook);

        TRACE("WSAConnect, attach: %p to %p \n", WSAConnect_original, WSAConnect_hook);
        DetourAttach(&(PVOID&)WSAConnect_original, WSAConnect_hook);



        error = DetourTransactionCommit();

        if (error == NO_ERROR)
        {
            TRACE("DetourTransactionCommit success!\n");
        }
        else
        {
            MessageBox(NULL, TEXT("DetourTransactionCommit failed!"), MODULENAME, MB_ICONHAND);
        }




        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)CreateMutexA_original, CreateMutexA_hook);


        DetourDetach(&(PVOID&)CreateWindowExA_original, CreateWindowExA_hook);
        DetourDetach(&(PVOID&)CreateWindowExW_original, CreateWindowExW_hook);
        DetourDetach(&(PVOID&)ShowWindow_original, ShowWindow_hook);
        DetourDetach(&(PVOID&)MoveWindow_original, MoveWindow_hook);
        DetourDetach(&(PVOID&)SetWindowLongA_original, SetWindowLongA_hook);
        DetourDetach(&(PVOID&)ChangeDisplaySettingsA_original, ChangeDisplaySettingsA_hook);



        DetourDetach(&(PVOID&)connect_original, connect_hook);
        DetourDetach(&(PVOID&)WSAConnect_original, WSAConnect_hook);


        error = DetourTransactionCommit();


        TRACE("Stopping.\n");

        break;
    }
    return TRUE;
}







VOID PrintMessage(LPCTSTR pszFormat, ...)
{
    va_list pArgs;

    TCHAR szMessageBuffer[16380] = { 0 };
    va_start(pArgs, pszFormat);
    StringCchVPrintf(szMessageBuffer, 16380, pszFormat, pArgs);
    va_end(pArgs);

    OutputDebugString(szMessageBuffer);
}

VOID PrintMessageA(LPCSTR pszFormat, ...)
{
    va_list pArgs;

    CHAR szMessageBuffer[16380] = { 0 };
    va_start(pArgs, pszFormat);
    StringCchVPrintfA(szMessageBuffer, 16380, pszFormat, pArgs);
    va_end(pArgs);

    OutputDebugStringA(szMessageBuffer);
}