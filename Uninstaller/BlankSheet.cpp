#include "StdAfx.h"
#include "BlankSheet.h"

IMPLEMENT_DYNAMIC(CBlankPropertySheet, CPropertySheet)
CBlankPropertySheet::CBlankPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_mainPage);
}


CBlankPropertySheet::~CBlankPropertySheet(void)
{
}


BOOL CBlankPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	
	return bResult;
}
BEGIN_MESSAGE_MAP(CBlankPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CBlankPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}