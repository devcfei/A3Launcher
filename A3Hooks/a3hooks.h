#pragma once



extern "C"
{

    // kernel32.dll

    typedef HANDLE(WINAPI* PFN_CreateMutexA)(
        _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
        _In_ BOOL bInitialOwner,
        _In_opt_ LPCSTR lpName
        );

    extern PFN_CreateMutexA CreateMutexA_original;


    extern
        HANDLE
        WINAPI
        CreateMutexA_hook(
            _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
            _In_ BOOL bInitialOwner,
            _In_opt_ LPCSTR lpName
        );










    // user32.dll

    typedef HWND(WINAPI* PFN_CreateWindowExA)(
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
        _In_opt_ LPVOID lpParam);


    extern PFN_CreateWindowExA CreateWindowExA_original;



    extern
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
            _In_opt_ LPVOID lpParam);




    typedef HWND(WINAPI* PFN_CreateWindowExW)(
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
        _In_opt_ LPVOID lpParam);


    extern PFN_CreateWindowExW CreateWindowExW_original;



    extern
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
            _In_opt_ LPVOID lpParam);




    typedef BOOL(WINAPI* PFN_MoveWindow)(
        _In_ HWND hWnd,
        _In_ int X,
        _In_ int Y,
        _In_ int nWidth,
        _In_ int nHeight,
        _In_ BOOL bRepaint);


    extern PFN_MoveWindow MoveWindow_original;

    extern
        BOOL
        WINAPI
        MoveWindow_hook(
            _In_ HWND hWnd,
            _In_ int X,
            _In_ int Y,
            _In_ int nWidth,
            _In_ int nHeight,
            _In_ BOOL bRepaint);



    typedef LONG(WINAPI* PFN_SetWindowLongA)(
        _In_ HWND hWnd,
        _In_ int nIndex,
        _In_ LONG dwNewLong);

    extern PFN_SetWindowLongA SetWindowLongA_original;


    extern
        LONG
        WINAPI
        SetWindowLongA_hook(
            _In_ HWND hWnd,
            _In_ int nIndex,
            _In_ LONG dwNewLong);




    typedef LONG(WINAPI* PFN_ChangeDisplaySettingsA)(
        _In_opt_ DEVMODEA* lpDevMode,
        _In_ DWORD dwFlags);

    extern PFN_ChangeDisplaySettingsA ChangeDisplaySettingsA_original;


    extern
        LONG
        WINAPI
        ChangeDisplaySettingsA_hook(
            _In_opt_ DEVMODEA* lpDevMode,
            _In_ DWORD dwFlags);



    typedef BOOL(WINAPI* PFN_ShowWindow)(
        _In_ HWND hWnd,
        _In_ int nCmdShow
        );
    extern PFN_ShowWindow ShowWindow_original;

    extern BOOL WINAPI ShowWindow_hook(
        _In_ HWND hWnd,
        _In_ int nCmdShow
    );



    // ws2_32.dll

    typedef int (WSAAPI* PFN_connect)(
        _In_ SOCKET s,
        _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
        _In_ int namelen
        );



    extern PFN_connect connect_original;
    extern int
        WSAAPI
        connect_hook(
            _In_ SOCKET s,
            _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
            _In_ int namelen
        );



    typedef int (WSAAPI* PFN_WSAConnect)(
        _In_ SOCKET s,
        _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
        _In_ int namelen,
        _In_opt_ LPWSABUF lpCallerData,
        _Out_opt_ LPWSABUF lpCalleeData,
        _In_opt_ LPQOS lpSQOS,
        _In_opt_ LPQOS lpGQOS
        );


    extern PFN_WSAConnect WSAConnect_original;
    extern int
        WSAAPI
        WSAConnect_hook(
            _In_ SOCKET s,
            _In_reads_bytes_(namelen) const struct sockaddr FAR* name,
            _In_ int namelen,
            _In_opt_ LPWSABUF lpCallerData,
            _Out_opt_ LPWSABUF lpCalleeData,
            _In_opt_ LPQOS lpSQOS,
            _In_opt_ LPQOS lpGQOS
        );
}