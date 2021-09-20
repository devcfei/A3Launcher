// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	CenterWindow(GetParent());


	DWORD	dwHandle;

	// Determines whether the operating system can retrieve version information
	// for a specified file.
	TCHAR szFileName[MAX_PATH];
	int nSize = GetModuleFileName(NULL, szFileName, MAX_PATH);

	DWORD	dwFileVersionInfoSize =
		GetFileVersionInfoSize(szFileName, &dwHandle);

	if (NULL == dwFileVersionInfoSize)
		return false;	// Can't retrieve version information size.

	// Allocate space to retrieve version information using vector to prevent
	// memory leaks
	std::vector<BYTE>	pData(dwFileVersionInfoSize);

	// Retrieves version information for the specified file.
	if (false == GetFileVersionInfo(szFileName
		, dwHandle
		, dwFileVersionInfoSize
		, static_cast<LPVOID>(&pData[0])))
		return false; // Can't retrieve version information.

	// The memory of ptFileInfo is freed when pData is freed.
	VS_FIXEDFILEINFO* ptFileInfo;
	UINT	uintSize;

	// Retrieves version information from the version-information resource
	if (false == VerQueryValue(static_cast<LPVOID>(&pData[0])
		, _T("\\")
		, reinterpret_cast<LPVOID*> (&ptFileInfo)
		, &uintSize))
		return FALSE; // Can't retrieve version information


	CString str;

	str.Format(_T("A3Launcher -v%d.%d.%d.%d \n\n(c) Copyright 2012\n"),
		static_cast<unsigned short>((ptFileInfo->dwFileVersionMS >> 16) & 0xffff), //major
		static_cast<unsigned short>(ptFileInfo->dwFileVersionMS & 0xffff),
		static_cast<unsigned short>((ptFileInfo->dwFileVersionLS >> 16) & 0xffff),
		static_cast<unsigned short>(ptFileInfo->dwFileVersionLS & 0xffff)
	);
	CStatic& t = static_cast<CStatic>(GetDlgItem(IDC_STATIC_APPINFO));
	t.SetWindowText(str);

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);


	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
