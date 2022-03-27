#pragma once
#include "afxdlgs.h"
#include "MainUninstPage.h"

class CBlankPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CBlankPropertySheet)
public:
	CBlankPropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CBlankPropertySheet(void);

private:
	CMainUninstPage m_mainPage;
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};

