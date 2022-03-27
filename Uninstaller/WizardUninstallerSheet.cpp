#include "StdAfx.h"
#include "WizardUninstallerSheet.h"



IMPLEMENT_DYNAMIC(CWizardUninstallerPropertySheet, CPropertySheet)
CWizardUninstallerPropertySheet::CWizardUninstallerPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_welcomePage);
	AddPage(&m_progPage);
	AddPage(&m_finishPage);

	SetActivePage(&m_welcomePage);
	m_nPageIdx = 0;
}


CWizardUninstallerPropertySheet::~CWizardUninstallerPropertySheet(void)
{
}


BOOL CWizardUninstallerPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	
	return bResult;
}
BEGIN_MESSAGE_MAP(CWizardUninstallerPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWizardUninstallerPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

INT CWizardUninstallerPropertySheet::SetNextPage()
{
	m_nPageIdx++;
	if (m_nPageIdx >= 3)
	{
		return 3;
	}

	SetActivePage(m_nPageIdx);

	return m_nPageIdx;
}