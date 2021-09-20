#include "stdafx.h"
#include "AppData.h"


int CAppData::Initialize()
{
    // TODO: Add your implementation code here.

    InitializeAppPath();
    EnumConfigs();

    return 0;
}


int CAppData::InitializeAppPath()
{

    TCHAR szFileName[MAX_PATH];
    int nSize = GetModuleFileName(NULL, szFileName, MAX_PATH);
    for (int i = nSize - 1; i > 0; --i)
    {
        if (szFileName[i] == _T('\\'))
        {
            szFileName[i + 1] = 0;
            break;
        }
    }

    m_strAppPath = szFileName;


    return 0;
}

static BOOL IsSuffix(LPCTSTR lpszName, LPCTSTR suffix)
{
    BOOL bMatch = FALSE;

    LPCTSTR p;

    int nSize = lstrlen(lpszName);
    for (int i = nSize - 1; i > 0; --i)
    {
        if (lpszName[i] == _T('.'))	// find the suffix
        {
            p = &lpszName[i];

            if (!lstrcmp(p, suffix))
            {
                bMatch = TRUE;
            }
            break;
        }
    }

    return bMatch;
}

int CAppData::EnumConfigs()
{
    // Enum the configurations
    //	the configuations are in <app path>\A3LauncherData
    //	the configuation file are *.ini
    // 	---------example----------------
    // 	[A3Launcher]
    //	AppFriendlyName=""	; Display name
    //  AppPath=""			; Application path
    //	AppArguments=""		; Application startup arguments
    //	WindowMode=0;		; (N/A | 0) - no function, 1 - window mode, 2 - full screen
    //  WindowWidth=;		; Window width, valid when WindowMode = 1
    //  WindowHeight=;		; Window height, valid when WindowMode = 1
    // 
    // 
    //
    // 
    //
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;

    CString path;
    path = m_strAppPath;
    path += CString(_T("A3LauncherData"));

    hFind = FindFirstFile(path, &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        CreateDirectory(path, NULL);
    }

    // enumerate config files in A3LauncherData folder
    path += CString(_T("\\*"));
    hFind = FindFirstFile(path, &ffd);


	// List all the files in the directory with some info about them.
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{

		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;

			if (IsSuffix(ffd.cFileName,_T(".ini")))
			{

                CString path;
                path = m_strAppPath;
                path += CString(_T("A3LauncherData"));
                path += CString(_T("\\"));
                path += ffd.cFileName;


                // use STL fstream
                USES_CONVERSION;
                std::ifstream in(T2A(path.GetString()), std::ios::in);


                std::string lineStr;

                CAppConfig cfg;

                cfg.m_strHostName = _T("");
                cfg.m_wPort = 4219;

                int line = 0;
                while (std::getline(in, lineStr))
                {        
                    std::stringstream ss(lineStr);

                    if (line == 0)
                    {
                        if (lineStr != "[A3Launcher]")
                        {
                            //
                            break;
                        }
                        else
                        {
                            line++;
                            continue;
                        }
                    }

                    std::string str;

                    std::vector<std::string> lineArray;

                    while (std::getline(ss, str, '='))
                    {
                        lineArray.push_back(str);
                    }

                    //lineArray.0, keyname, lineArray.1, keyvalue
                    if (lineArray.size() == 2)
                    {

                        if (lineArray[0] == "AppFriendlyName")
                        {
                            USES_CONVERSION;
                            cfg.m_strFriendlyName = A2T(lineArray[1].c_str());
                        }

                        if (lineArray[0] == "AppPath")
                        {
                            USES_CONVERSION;
                            cfg.m_strAppPath = A2T(lineArray[1].c_str());
                        }
                        if (lineArray[0] == "AppArguments")
                        {
                            USES_CONVERSION;
                            cfg.m_strAppArguments = A2T(lineArray[1].c_str());
                        }

                        if (lineArray[0] == "AppArguments")
                        {
                            USES_CONVERSION;
                            cfg.m_strAppArguments = A2T(lineArray[1].c_str());
                        }


                        if (lineArray[0] == "WindowMode")
                        {
                            int i = atoi(lineArray[1].c_str());
                            switch (i)
                            {
                            case 0:
                                cfg.m_wndMode = CAppConfig::WndModeNull;
                                break;
                            case 1:
                                cfg.m_wndMode = CAppConfig::WndModeWindowed;
                                break;
                            case 2:
                                cfg.m_wndMode = CAppConfig::WndModeFullScreen;
                                break;
                            }

                        }

                        if (lineArray[0] == "WindowWidth")
                        {
                            cfg.m_nWindowWidth = atoi(lineArray[1].c_str());
                        }

                        if (lineArray[0] == "WindowHeight")
                        {
                            cfg.m_nWindowHeight = atoi(lineArray[1].c_str());
                        }

                        if (lineArray[0] == "ServerIP")
                        {
                            USES_CONVERSION;
                            cfg.m_strHostName = A2T(lineArray[1].c_str());
                        }

                        if (lineArray[0] == "ServerPort")
                        {
                            cfg.m_wPort = atoi(lineArray[1].c_str());
                        }
                    }

                    line++;
                }


                AddConfig(cfg);
			}

		}
	} while (FindNextFile(hFind, &ffd) != 0);


	FindClose(hFind);
    
    return 0;
}



int CAppData::SaveConfig()
{
    for (std::vector<CAppConfig>::iterator it = m_vecAppConfigs.begin();
        it != m_vecAppConfigs.end();
        ++it)
    {
        CString path;
        path = m_strAppPath;
        path += CString(_T("A3LauncherData"));
        path += CString(_T("\\"));
        path += it->m_strFriendlyName;
        path += CString(_T(".ini"));

        
        // use STL fstream
        USES_CONVERSION;
        std::ofstream out(T2A(path.GetString()),  std::ios::out);

        out << "[A3Launcher]" << std::endl;
        out << "AppFriendlyName=" << T2A(it->m_strFriendlyName.GetString()) << std::endl;
        out << "AppPath=" << T2A(it->m_strAppPath.GetString()) << std::endl;
        out << "AppArguments=" << T2A(it->m_strAppArguments.GetString()) << std::endl;
        out << "WindowMode=" << it->m_wndMode << std::endl;
        out << "WindowWidth=" << it->m_nWindowWidth << std::endl;
        out << "WindowHeight=" << it->m_nWindowHeight << std::endl;
        out << "ServerIP=" << T2A(it->m_strHostName.GetString()) << std::endl;
        out << "ServerPort=" << it->m_wPort << std::endl;


        
    }


    return 0;
}


int CAppData::AddConfig(CAppConfig cfg)
{
    m_vecAppConfigs.push_back(cfg);
    return 0;
}

int CAppData::UpdateConfig(CAppConfig& cfg)
{
    for (std::vector<CAppConfig>::iterator it = m_vecAppConfigs.begin();
        it != m_vecAppConfigs.end();
        ++it)
    {
        if (it->m_strFriendlyName.GetString() == cfg.m_strFriendlyName)
        {
            it->m_strAppPath = cfg.m_strAppPath;
            it->m_strAppArguments = cfg.m_strAppArguments;
            it->m_wndMode = cfg.m_wndMode;
            it->m_nWindowWidth = cfg.m_nWindowWidth;
            it->m_nWindowHeight = cfg.m_nWindowHeight;

            it->m_strHostName = cfg.m_strHostName;
            it->m_wPort = cfg.m_wPort;
        }
    }
    return 0;
}

int CAppData::GetConfig(size_t nIndex, CAppConfig& cfg) const
{
    if (nIndex < m_vecAppConfigs.size())
    {
        cfg = m_vecAppConfigs.at(nIndex);
        return 0;
    }
    return -1;
}
