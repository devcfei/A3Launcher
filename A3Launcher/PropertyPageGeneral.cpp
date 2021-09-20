#include "stdafx.h"
#include "resource.h"
#include "PropertyPageGeneral.h"


CPropertyPageGeneral::CPropertyPageGeneral()
{

}

CPropertyPageGeneral::~CPropertyPageGeneral()
{

}

LRESULT CPropertyPageGeneral::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	GetDlgItem(IDC_EDIT_PATH).SetWindowText(m_strPath);
	GetDlgItem(IDC_EDIT_NAME).SetWindowText(m_strName);
	GetDlgItem(IDC_EDIT_ARGUMENTS).SetWindowText(m_strArgument);

    return TRUE;
}


int CPropertyPageGeneral::OnSetActive()
{
    SetWizardButtons( PSWIZB_NEXT);
    return 0;
}
int CPropertyPageGeneral::OnKillActive()
{
	CString str;
	GetDlgItem(IDC_EDIT_PATH).GetWindowText(str);

	m_strPath = str;

	GetDlgItem(IDC_EDIT_NAME).GetWindowText(str);
	m_strName = str;

	GetDlgItem(IDC_EDIT_ARGUMENTS).GetWindowText(str);
	m_strArgument = str;

	return 0;
}



LRESULT CPropertyPageGeneral::OnBnClickedBtnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your command handler code here
	COMDLG_FILTERSPEC filterSpecs[] =
	{
		{ _T("Executable(*.exe)"),_T("*.exe") },
	};
	CShellFileOpenDialog fileOpenDlg(
		NULL,
		FOS_FORCEFILESYSTEM | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST,
		_T("exe"),
		filterSpecs,
		_countof(filterSpecs));
	if (IDOK == fileOpenDlg.DoModal(m_hWnd))
	{
		CString strPath;

		TCHAR str[MAX_PATH];
		fileOpenDlg.GetFilePath(str, MAX_PATH);
		strPath = str;
		int length = strPath.GetLength();
		if (length > 0)
		{
			GetDlgItem(IDC_EDIT_PATH).SetWindowText(strPath);
		}
	}

    return 0;
}
