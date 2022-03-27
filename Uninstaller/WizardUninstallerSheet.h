#pragma once
#include "afxdlgs.h"
#include "CWizardUninstWelcomePage.h"
#include "CWizardUninstProgPage.h"
#include "CWizardUninstFinishPage.h"

#define  MAX_PAGES_CNT		3

class CWizardUninstallerPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CWizardUninstallerPropertySheet)
public:
	CWizardUninstallerPropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CWizardUninstallerPropertySheet(void);

public:
	CWizardUninstWelcomePage m_welcomePage;
	CWizardUninstProgPage m_progPage;
	CWizardUninstFinishPage m_finishPage;

private:
	int m_nPageIdx;
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	INT SetNextPage();
};

