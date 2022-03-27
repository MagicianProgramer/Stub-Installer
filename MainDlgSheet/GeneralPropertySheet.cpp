#include "StdAfx.h"
#include "GeneralPropertySheet.h"


IMPLEMENT_DYNAMIC(CGeneralPropertySheet, CPropertySheet)
CGeneralPropertySheet::CGeneralPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CString strTitle;
	m_nTotalDlgs = 0;

	AddPage(&m_GeneralInformationDlg);
	m_nTotalDlgs++;
	strTitle = L"INFORMATION";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_GeneralParameterDlg);
	m_nTotalDlgs++;
	strTitle = L"PARAMETERS";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_GeneralRequirementsDlg);
	m_nTotalDlgs++;
	strTitle = L"REQUIREMENTS";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_GeneralPrequisitesDlg);
	m_nTotalDlgs++;
	strTitle = L"PREREQUISITES";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_GeneralOutputDlg);	
	m_nTotalDlgs++;
	strTitle = L"OUTPUT";
	m_vecStrTitle.push_back(strTitle);
}


CGeneralPropertySheet::~CGeneralPropertySheet(void)
{
	m_vecStrTitle.clear();
}


BOOL CGeneralPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	GetTabControl()->ShowWindow(SW_HIDE);
	
	return bResult;
}
BEGIN_MESSAGE_MAP(CGeneralPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CGeneralPropertySheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void CGeneralPropertySheet::SetActive1stPage(BOOL bNewGuid)
{
	SetActivePage(&m_GeneralInformationDlg);
	
	if (bNewGuid)
	{
		m_GeneralParameterDlg.GenerateProductGuid();
	}
}


void CGeneralPropertySheet::NextPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage++;
	nCurPage = nCurPage % m_nTotalDlgs;

	SetActivePage(nCurPage);
}


void CGeneralPropertySheet::PrevPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage--;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}

void CGeneralPropertySheet::SetPage(INT idx)
{
	SetActivePage(idx % m_nTotalDlgs);
}

CString CGeneralPropertySheet::GetTitle()
{
	int nCurPage = GetActiveIndex();
	return m_vecStrTitle[nCurPage];
}

LRESULT CGeneralPropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CPropertySheet::WindowProc(message, wParam, lParam);
}
