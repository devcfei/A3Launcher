#include "stdafx.h"
#include "resource.h"
#include "PropertySheetApp.h"


CPropertySheetApp::CPropertySheetApp(_U_STRINGorID title, UINT uStartPage,
    HWND hWndParent) :
    CPropertySheetImpl<CPropertySheetApp>(title, uStartPage, hWndParent)
{
    m_psh.dwFlags |= PSH_NOAPPLYNOW | PSH_NOCONTEXTHELP ;

}


void CPropertySheetApp::OnSheetInitialized()
{

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
	
	SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIconSmall);
    
}