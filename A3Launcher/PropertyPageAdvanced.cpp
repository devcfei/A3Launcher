#include "stdafx.h"
#include "resource.h"
#include "PropertyPageAdvanced.h"


CPropertyPageAdvanced::CPropertyPageAdvanced()
    :m_bWindowMode(FALSE)
    ,m_wPort(4219)
{
    m_strIp = _T("127.0.0.1");
}

CPropertyPageAdvanced::~CPropertyPageAdvanced()
{

}

LRESULT CPropertyPageAdvanced::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CButton& btn = static_cast<CButton&>(GetDlgItem(IDC_CHECK_WINDOWER));

    int chk = m_bWindowMode ? BST_CHECKED : BST_UNCHECKED;
    btn.SetCheck(chk);

    CString str;
    str.Format(_T("%d"), m_wPort);
    CEdit& editPort = static_cast<CEdit&>(GetDlgItem(IDC_EDIT_PORT));
    editPort.SetWindowText(str);


    CIPAddressCtrl& ipctl = static_cast<CIPAddressCtrl&>(GetDlgItem(IDC_IPADDRESS));
    ipctl.SetWindowText(m_strIp);

    return TRUE;
}

int CPropertyPageAdvanced::OnSetActive()
{
    SetWizardButtons(PSWIZB_BACK| PSWIZB_FINISH);
    return 0;
}

int CPropertyPageAdvanced::OnKillActive()
{
    CButton& btn = static_cast<CButton&>(GetDlgItem(IDC_CHECK_WINDOWER));
    if (BST_CHECKED == btn.GetCheck())
        m_bWindowMode = TRUE;
    else
        m_bWindowMode = FALSE;

    CEdit& editPort = static_cast<CEdit&>(GetDlgItem(IDC_EDIT_PORT));

    TCHAR szPort[8];
    editPort.GetWindowText(szPort, 6);
    UINT port= StrToInt(szPort);
    if (port < 0x10000)
    {
        m_wPort = port;
    }


    CIPAddressCtrl& ipctl = static_cast<CIPAddressCtrl&>(GetDlgItem(IDC_IPADDRESS));
    ipctl.GetWindowText(m_strIp);


	return 0;
}
