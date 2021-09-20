#pragma once


class CAppConfig
{

public:
    CAppConfig()
        : m_nWindowWidth(0)
        , m_nWindowHeight(0)
    {

    }


    enum WindowMode
    {
        WndModeNull,
        WndModeWindowed,
        WndModeFullScreen,
    };
    CString m_strFriendlyName;
    CString m_strAppPath;
    CString m_strAppArguments;

    WindowMode m_wndMode;

    int m_nWindowWidth;
    int m_nWindowHeight;

    CString m_strHostName; //HostName or IP
    WORD m_wPort;

};

class CAppData
{
private:
    CString m_strAppPath;



public:

    virtual ~CAppData()
    {
        SaveConfig();
    }

    int Initialize();
    
    int AddConfig(CAppConfig cfg);
    int UpdateConfig(CAppConfig &cfg);
    int GetConfig(size_t nIndex, CAppConfig& cfg) const;

    CString& GetAppPath()
    {
        return m_strAppPath;
    }

    int SaveConfig();
private:
    int InitializeAppPath();
    int EnumConfigs();
    std::vector< CAppConfig> m_vecAppConfigs;
};

