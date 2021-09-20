#pragma once
class CPropertyPageAdvanced:
    public CPropertyPageImpl<CPropertyPageAdvanced>
{
public:
    enum { IDD = IDD_PROP_ADVANCED};

    // Construction
    CPropertyPageAdvanced();
    ~CPropertyPageAdvanced();

    // Maps
    BEGIN_MSG_MAP(CPropertyPageAdvanced)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        CHAIN_MSG_MAP(CPropertyPageImpl<CPropertyPageAdvanced>)
    END_MSG_MAP()


    // Message handlers
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    //////////////////////////////////////////////////////////////////////
    // Notification handlers
    int OnSetActive();
    int OnKillActive();
    
    // DDX variables
    bool m_bWindowMode;
    WORD m_wPort;
    CString m_strIp;
};

