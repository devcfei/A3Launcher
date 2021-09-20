#pragma once
class CPropertyPageGeneral:
    public CPropertyPageImpl<CPropertyPageGeneral>
{
public:
    enum { IDD = IDD_PROP_GENERAL};

    // Construction
    CPropertyPageGeneral();
    ~CPropertyPageGeneral();

    // Maps
    BEGIN_MSG_MAP(CPropertyPageGeneral)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CPropertyPageGeneral>)
        COMMAND_HANDLER(IDC_BTN_BROWSE, BN_CLICKED, OnBnClickedBtnBrowse)
    END_MSG_MAP()


    // Message handlers
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    //////////////////////////////////////////////////////////////////////
    // Notification handlers
    int OnSetActive();
    int OnKillActive();

    // DDX variables
    CString m_strPath;
    CString m_strName;
    CString m_strArgument;
    LRESULT OnBnClickedBtnBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

