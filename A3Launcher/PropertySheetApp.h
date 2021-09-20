#pragma once
class CPropertySheetApp:
    public CPropertySheetImpl< CPropertySheetApp>
{
public:

	CPropertySheetApp(_U_STRINGorID title = (LPCTSTR)NULL,
		UINT uStartPage = 0, HWND hWndParent = NULL);

	void OnSheetInitialized();

};

