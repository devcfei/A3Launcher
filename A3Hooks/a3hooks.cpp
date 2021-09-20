#include "pch.h"


// kernel32.dll 

PFN_CreateMutexA CreateMutexA_original = CreateMutexA;



HANDLE
WINAPI
CreateMutexA_hook(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_ BOOL bInitialOwner,
    _In_opt_ LPCSTR lpName
)
{
    HANDLE h;
    if (0 == lstrcmpA(lpName, "OnceMutexA3Client"))
    {
        TRACEA("CreateMutexA_hook -  %s\n", lpName);
        lpName = NULL;

    }

    h = CreateMutexA_original(lpMutexAttributes, bInitialOwner, lpName);

    return h;
}





// user32.dll 

PFN_CreateWindowExA CreateWindowExA_original = CreateWindowExA;
PFN_CreateWindowExW CreateWindowExW_original = CreateWindowExW;
PFN_ShowWindow ShowWindow_original = ShowWindow_hook;
PFN_MoveWindow MoveWindow_original = MoveWindow;
PFN_SetWindowLongA SetWindowLongA_original = SetWindowLongA;
PFN_ChangeDisplaySettingsA ChangeDisplaySettingsA_original = ChangeDisplaySettingsA;

HWND
WINAPI
CreateWindowExA_hook(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCSTR lpClassName,
    _In_opt_ LPCSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam)
{

    HWND hwnd;

    TRACEA("CreateWindowExA_hook -  %s, dwExStyle: %x, dwStyle: %x, Size=%d,%d,%d,%d\n", lpWindowName, dwExStyle, dwStyle, X, Y, nWidth, nHeight);

    LPCSTR szWindowName = "A3";
    if (lpWindowName)
    {
        if (0 == lstrcmpA(lpWindowName, "pRoJeCtAaA") && gAppData.WindowMode == TRUE)
        {
            dwExStyle = 0;
            dwStyle = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;


            if (gAppData.WindowWidth != 0 && gAppData.WindowHeight != 0)
            {
                // override by settings
                nWidth = gAppData.WindowWidth;
                nHeight = gAppData.WindowHeight;
                TRACEA(" override by Window Size=%d,%d\n", nWidth, nHeight);

            }


            RECT rect;
            rect.left = X; rect.right = rect.left + nWidth;
            rect.top = Y; rect.bottom = rect.top + nHeight;
            AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);

            TRACEA("AdjustWindowRectEx RECT: %d, %d, %d,%d\n", rect.left, rect.top, rect.right, rect.bottom);

            nWidth = rect.right - rect.left;
            nHeight = rect.bottom - rect.top;
            TRACEA("CreateWindow change Size=,%d,%d\n", nWidth, nHeight);




            // make window in the center    
            int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
            int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);


            X = (iScreenWidth - nWidth) / 2;
            Y = (iScreenHeight - nHeight) / 2;


            lpWindowName = szWindowName;


        }

    }


    hwnd = CreateWindowExA_original(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );


    return hwnd;
}



HWND
WINAPI
CreateWindowExW_hook(
    _In_ DWORD dwExStyle,
    _In_opt_ LPCWSTR lpClassName,
    _In_opt_ LPCWSTR lpWindowName,
    _In_ DWORD dwStyle,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_opt_ HWND hWndParent,
    _In_opt_ HMENU hMenu,
    _In_opt_ HINSTANCE hInstance,
    _In_opt_ LPVOID lpParam)
{

    HWND hwnd;

    TRACE("CreateWindowExW_hook -  %s, dwExStyle: %x, dwStyle: %x, Size=%d,%d,%d,%d\n", lpWindowName, dwExStyle, dwStyle, X, Y, nWidth, nHeight);




    hwnd = CreateWindowExW_original(
        dwExStyle,
        lpClassName,
        lpWindowName,
        dwStyle,
        X,
        Y,
        nWidth,
        nHeight,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );


    return hwnd;
}


BOOL WINAPI ShowWindow_hook(
    _In_ HWND hWnd,
    _In_ int nCmdShow
)
{
    SetWindowTextA(hWnd, "A3");
    return ShowWindow(hWnd, nCmdShow);
}


BOOL
WINAPI
MoveWindow_hook(
    _In_ HWND hWnd,
    _In_ int X,
    _In_ int Y,
    _In_ int nWidth,
    _In_ int nHeight,
    _In_ BOOL bRepaint)
{
    TRACE("MoveWindow_hook -  Size=%d,%d,%d,%d\n", X, Y, nWidth, nHeight);

    return MoveWindow_original(hWnd, X, Y, nWidth, nHeight, bRepaint);
}


LONG
WINAPI
SetWindowLongA_hook(
    _In_ HWND hWnd,
    _In_ int nIndex,
    _In_ LONG dwNewLong)
{
    TRACE("SetWindowLongA_hook -  %d,%x,\n", nIndex, dwNewLong);
    return SetWindowLongA_original(hWnd, nIndex, dwNewLong);
}



LONG
WINAPI
ChangeDisplaySettingsA_hook(
    _In_opt_ DEVMODEA* lpDevMode,
    _In_ DWORD dwFlags)
{


    TRACE("ChangeDisplaySettingsA_hook -  %d,%x,\n", lpDevMode, dwFlags);

    return DISP_CHANGE_SUCCESSFUL; // ChangeDisplaySettingsA_original(lpDevMode, dwFlags);

}





// ws2_32.dll



PFN_connect connect_original = connect;
PFN_WSAConnect WSAConnect_original = WSAConnect;

int
WSAAPI
connect_hook(
    _In_ SOCKET s,
    _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
    _In_ int namelen
)
{
    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    struct in_addr in = sock->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &in, str, sizeof(str));

    TRACEA("ip:port  %s : %d\n", str, port);

    return connect_original(s, name, namelen);



}



int
WSAAPI
WSAConnect_hook(
    _In_ SOCKET s,
    _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
    _In_ int namelen,
    _In_opt_ LPWSABUF lpCallerData,
    _Out_opt_ LPWSABUF lpCalleeData,
    _In_opt_ LPQOS lpSQOS,
    _In_opt_ LPQOS lpGQOS
)
{

    struct sockaddr_in* sock = (struct sockaddr_in*)name;
    int port = ntohs(sock->sin_port);
    struct in_addr in = sock->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &in, str, sizeof(str));


    struct sockaddr redirect_name = *name;


    TRACEA("ip:port  %s : %d\n", str, port);

    if (port == 4219)
    {
        
        lstrcpyA(str, gAppData.szHost);
        port = gAppData.iPort;
        TRACEA("re-direct  %s : %d\n", str, port);


        struct sockaddr_in* sock2 = (struct sockaddr_in*)&redirect_name;

        sock2->sin_family = AF_INET;
        sock2->sin_port = htons(port);
        inet_pton(AF_INET, str, &sock2->sin_addr);



    }

    return WSAConnect_original(s, &redirect_name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
}


