// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "PropertyPageGeneral.h"
#include "PropertyPageAdvanced.h"
#include "PropertySheetApp.h"

#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// init the app
	AppInit();


	// reight bottom the dialog on the screen
	RightBottomWindow();


	// init tray icon
	InitTrayIcon();





	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

int CMainDlg::InitTrayIcon()
{
	InstallIcon(_T("A3Launcher"), m_hIconSmall, IDR_MENU_SYSTRAY);
	return 0;
}


LRESULT CMainDlg::OnAppExit(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	CloseDialog(wID);

	return 0;
}


LRESULT CMainDlg::OnSize(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: Add your message handler code here and/or call default
	if (wParam == SIZE_MINIMIZED)
	{
		ShowWindow(SW_HIDE);
	}
	return 0;
}


LRESULT CMainDlg::OnPopupOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	ShowWindow(SW_SHOWNORMAL);

	RightBottomWindow();
	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	AppAdd();

	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here

	AppDelete();

	return 0;
}


LRESULT CMainDlg::OnBnClickedBtnProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	AppProperties();
	return 0;
}



int CMainDlg::AppAdd()
{
	// TODO: Add your implementation code here.

	CPropertySheetApp sheet(_T("New"));
	CPropertyPageGeneral pgGeneral;
	CPropertyPageAdvanced pgAdvanced;

	sheet.AddPage(pgGeneral);
	sheet.AddPage(pgAdvanced);
	//sheet.SetWizardMode();


	if (IDOK == sheet.DoModal(*this))
	{
		// check propertiess
		int nSize = pgGeneral.m_strPath.GetLength();
		if (!nSize)
		{
			return 0;
		}

		if (!pgGeneral.m_strName.GetLength())
		{
			LPCTSTR lpszPath = pgGeneral.m_strPath.GetBuffer();
			for (int i = nSize - 1; i > 0; --i)
			{
				if (lpszPath[i] == _T('\\'))
				{
					pgGeneral.m_strName = &lpszPath[i + 1];
					break;
				}
			}
		}



		// add properties

		CListViewCtrl& listview = static_cast<CListViewCtrl&>(GetDlgItem(IDC_LIST_APP));

		CImageList imglist;
		imglist.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1);
		imglist.AddIcon(m_hIcon);

		listview.SetImageList(imglist, LVSIL_NORMAL);
		int count = listview.GetItemCount();
		listview.AddItem(count, 0, pgGeneral.m_strName, 0);

		CAppConfig* cfg = new CAppConfig;
		cfg->m_strAppPath = pgGeneral.m_strPath;
		cfg->m_strFriendlyName = pgGeneral.m_strName;
		cfg->m_strAppArguments = pgGeneral.m_strArgument;

		if (pgAdvanced.m_bWindowMode)
		{
			cfg->m_wndMode = CAppConfig::WindowMode::WndModeWindowed;
			cfg->m_nWindowWidth = 1024;	// TODO: need extra data in property sheet
			cfg->m_nWindowHeight = 768;	// TODO: need extra data in property sheet
		}
		else
			cfg->m_wndMode = CAppConfig::WindowMode::WndModeNull;



		cfg->m_strHostName = pgAdvanced.m_strIp;
		cfg->m_wPort = pgAdvanced.m_wPort;

		listview.SetItemData(count, reinterpret_cast<DWORD_PTR>(cfg));


		m_AppData.AddConfig(*cfg);

	}

	return 0;
}


int CMainDlg::AppDelete()
{
	CListViewCtrl& listview = static_cast<CListViewCtrl&>(GetDlgItem(IDC_LIST_APP));
	int count = listview.GetItemCount();
	LVITEMW item;
	if (listview.GetSelectedItem(&item))
	{

		CAppConfig* cfg = (CAppConfig*)(listview.GetItemData(item.iItem));

		delete cfg;

		listview.SetItemData(item.iItem, NULL);

		listview.DeleteItem(item.iItem);

	}


	return 0;

}





int CMainDlg::AppProperties()
{
	// TODO: Add your implementation code here.

	CListViewCtrl& listview = static_cast<CListViewCtrl&>(GetDlgItem(IDC_LIST_APP));
	int count = listview.GetItemCount();
	LVITEMW item;
	if (listview.GetSelectedItem(&item))
	{

		CAppConfig* cfg = (CAppConfig*)(listview.GetItemData(item.iItem));

		ATLASSERT(cfg);


		CPropertySheetApp sheet(_T("Properties"));
		CPropertyPageGeneral pgGeneral;
		CPropertyPageAdvanced pgAdvanced;

		pgGeneral.m_strName = cfg->m_strFriendlyName;
		pgGeneral.m_strPath = cfg->m_strAppPath;
		pgGeneral.m_strArgument = cfg->m_strAppArguments;
		pgAdvanced.m_bWindowMode = cfg->m_wndMode == CAppConfig::WindowMode::WndModeWindowed ? TRUE : FALSE;

		pgAdvanced.m_strIp = cfg->m_strHostName;
		pgAdvanced.m_wPort = cfg->m_wPort;

		sheet.AddPage(pgGeneral);
		sheet.AddPage(pgAdvanced);


		if (IDOK == sheet.DoModal(*this))
		{
			// update properties
			cfg->m_strAppPath = pgGeneral.m_strPath;
			cfg->m_strFriendlyName = pgGeneral.m_strName;
			cfg->m_strAppArguments = pgGeneral.m_strArgument;
			if (pgAdvanced.m_bWindowMode)
			{
				cfg->m_wndMode = CAppConfig::WindowMode::WndModeWindowed;
				cfg->m_nWindowWidth = 1024;	// TODO: need extra data in property sheet
				cfg->m_nWindowHeight = 768;	// TODO: need extra data in property sheet
			}
			else
				cfg->m_wndMode = CAppConfig::WindowMode::WndModeNull;


			cfg->m_strHostName = pgAdvanced.m_strIp;
			cfg->m_wPort = pgAdvanced.m_wPort;



			// update the list view item name
			listview.SetItemText(item.iItem, 0, pgGeneral.m_strName);

			// find the config in AppData and update it

			m_AppData.UpdateConfig(*cfg);

		}


	}



	return 0;
}


int CMainDlg::AppInit()
{
	// initialize the app data
	m_AppData.Initialize();

	// TODO: the icon will be read from the target application
	m_hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(m_hIcon, TRUE);
	m_hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(m_hIconSmall, FALSE);

	// initialize the listview
	CListViewCtrl& listview = static_cast<CListViewCtrl&>(GetDlgItem(IDC_LIST_APP));

	int i = 0;
	while (1)
	{
		CAppConfig* cfg = new CAppConfig;
		int r = m_AppData.GetConfig(i, *cfg);
		if (r)
		{
			delete cfg;
			break;
		}

		// TODO: currently use the dxrun app icon, should use the target app icon in the future
		CImageList imglist;
		imglist.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1);
		imglist.AddIcon(m_hIcon);

		listview.SetImageList(imglist, LVSIL_NORMAL);
		int count = listview.GetItemCount();
		listview.AddItem(count, 0, cfg->m_strFriendlyName, 0);

		listview.SetItemData(count, reinterpret_cast<DWORD_PTR>(cfg));

		i++;

	}

	return 0;
}


LRESULT CMainDlg::OnLvnItemchangedListApp(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here


	return 0;
}


LRESULT CMainDlg::OnNMDblclkListApp(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	CListViewCtrl& listview = static_cast<CListViewCtrl&>(GetDlgItem(IDC_LIST_APP));

	if (pNMLV->iItem >= 0)
	{
		// valid item double clicked
		CAppConfig* cfg = (CAppConfig*)(listview.GetItemData(pNMLV->iItem));

		ATLASSERT(cfg);

		//if (cfg->m_wndMode == CAppConfig::WindowMode::WndModeWindowed)
		//{
		//	//dxxhook
		//	StartA3ProcessWithHook(*cfg);

		//}

		StartA3ProcessWithHook(*cfg);
	}

	return 0;
}


int CMainDlg::StartA3ProcessWithHookA(CAppConfig& cfg)
{
	return 0;
}


int CMainDlg::StartA3ProcessWithHookW(CAppConfig& cfg)
{

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;


	//
	// hook DLL path
	//
	CString strHooksPath = m_AppData.GetAppPath();
	CString strHooksPath_a3hooks = strHooksPath + _T("a3hooks.dll");


	//strHooksPath += _T("dx9hook.dll");
	LPCSTR rpszDllsOut[256];
	int rpszSize = 0;

	USES_CONVERSION;

	rpszDllsOut[rpszSize] = T2A(strHooksPath_a3hooks.GetBuffer());
	rpszSize++;





	//rpszDllsOut[2] = T2A(strHooksPath_d3d8to9.GetBuffer());
	//rpszSize++;

	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));
	si.cb = sizeof(si);

	DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

	SetLastError(0);

	CString strAppDir;
	CString strAppName;
	CString strAppFullPath;
	CString strAppPath = cfg.m_strAppPath;
	CString strArgument = cfg.m_strAppArguments;

	LPTSTR lpszPath = strAppPath.GetBuffer();
	strAppFullPath = lpszPath;
	int nSize = cfg.m_strAppPath.GetLength();
	for (int i = nSize - 1; i > 0; --i)
	{
		if (lpszPath[i] == _T('\\'))
		{
			strAppName = &lpszPath[i + 1];
			lpszPath[i + 1] = 0;
			break;
		}
	}
	strAppDir = lpszPath;

	// strArgument need to pass the extra command line
	
	if (cfg.m_wndMode == CAppConfig::WindowMode::WndModeWindowed)
	{
		CString str;

		str.Format(_T(" -windowmode 1 -width %d -height %d"), cfg.m_nWindowWidth, cfg.m_nWindowHeight );
		strArgument += str;

	}

	{
		CString str;
		str.Format(_T(" -host %s -port %d"), cfg.m_strHostName, cfg.m_wPort);
		strArgument += str;
	}



	strAppFullPath += _T(" ");
	strAppFullPath += strArgument;

	if (!DetourCreateProcessWithDllsW(NULL,
		strAppFullPath.GetBuffer(),
		NULL, NULL, TRUE, dwFlags, NULL, strAppDir.GetBuffer(),
		&si, &pi, rpszSize, rpszDllsOut, NULL)) {
		DWORD dwError = GetLastError();
		//printf("withdll.exe: DetourCreateProcessWithDllEx failed: %ld\n", dwError);
		if (dwError == ERROR_INVALID_HANDLE) {
#if DETOURS_64BIT
			ATLTRACE("withdll.exe: Can't detour a 32-bit target process from a 64-bit parent process.\n");
#else
			ATLTRACE("withdll.exe: Can't detour a 64-bit target process from a 32-bit parent process.\n");
#endif

			return -1; /**/
		}
	}

	ResumeThread(pi.hThread);

	return 0;
}

int CMainDlg::StartA3ProcessWithHook(CAppConfig& cfg)
{
#ifdef  UNICODE                     // r_winnt
	return StartA3ProcessWithHookW(cfg);
#else
	return StartA3ProcessWithHookA(cfg);
#endif
	return 0;
}


int CMainDlg::RightBottomWindow()
{
	// TODO: Add your implementation code here.

	CRect rc;
	GetClientRect(&rc);
	int x = GetSystemMetrics(SM_CXFULLSCREEN) - rc.Width();
	int y = GetSystemMetrics(SM_CYFULLSCREEN) - rc.Height();
	SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);

	return 0;
}


LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// hide to system tray
	ShowWindow(SW_HIDE);


	CloseDialog(1);
	return TRUE;
}
