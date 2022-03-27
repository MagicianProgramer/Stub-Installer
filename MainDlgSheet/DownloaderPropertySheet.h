#pragma once
#include "afxdlgs.h"
#include "InstallationUpdaterDlg.h"

#include <vector>
using namespace std;

class CDownloaderPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CDownloaderPropertySheet)
public:

	CDownloaderPropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CDownloaderPropertySheet(void);
	void SetActive1stPage();

private:	
	DECLARE_MESSAGE_MAP()
	CInstallationUpdaterDlg m_UpdateDlg;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void NextPage();
	void PrevPage();
	CString GetTitle();
	void SetPage(INT idx);
private:
	INT m_nTotalDlgs;
	vector<CString> m_vecStrTitle;
protected:
	virtual BOOL OnInitDialog();
};

