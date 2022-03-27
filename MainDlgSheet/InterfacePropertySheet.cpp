#include "StdAfx.h"
#include "InterfacePropertySheet.h"
#include "../common/common.h"


IMPLEMENT_DYNAMIC(CInterfacePropertySheet, CPropertySheet)
	CInterfacePropertySheet::CInterfacePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CString strTitle;
	m_nTotalDlgs = 0;

	AddPage(&m_InterfaceMainDlg);
	m_nTotalDlgs++;
	strTitle = L"INTERFACE MAIN";
	m_vecStrTitle.push_back(strTitle);

	//AddPage(&m_InterfaceLanguages);

	AddPage(&m_InterfaceUserInfoDlg);
	m_nTotalDlgs++;
	strTitle = L"USER INFORMATION";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_InterfaceCustomDlg);
	m_nTotalDlgs++;
	strTitle = L"CUSTOM";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_InterfaceFinishDlg);
	m_nTotalDlgs++;
	strTitle = L"FINISH";
	m_vecStrTitle.push_back(strTitle);
}



CInterfacePropertySheet::~CInterfacePropertySheet(void)
{
}


BEGIN_MESSAGE_MAP(CInterfacePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInterfacePropertySheet::SetActive1stPage()
{
	m_InterfaceMainDlg.m_bRefresh = TRUE;
	m_InterfaceUserInfoDlg.m_bRefresh = TRUE;
	m_InterfaceFinishDlg.m_bRefresh = TRUE;

	SetActivePage(&m_InterfaceMainDlg);
}

void CInterfacePropertySheet::SetMainExecutable2FinishTab()
{
	m_InterfaceFinishDlg.m_bRefresh = TRUE;
	m_InterfaceFinishDlg.SetExecutable();
}

LRESULT CInterfacePropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UM_INTERFACE_USERINFO_REFRESH:
		{
			m_InterfaceUserInfoDlg.m_bRefresh = TRUE;
			return 0L;
		}
		break;
	}

	return CPropertySheet::WindowProc(message, wParam, lParam);
}


void CInterfacePropertySheet::NextPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage++;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}


void CInterfacePropertySheet::SetPage(INT idx)
{
	SetActivePage(idx % m_nTotalDlgs);
}


void CInterfacePropertySheet::PrevPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage--;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}

BOOL CInterfacePropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	GetTabControl()->ShowWindow(SW_HIDE);

	return bResult;
}

CString CInterfacePropertySheet::GetTitle()
{
	int nCurPage = GetActiveIndex();
	return m_vecStrTitle[nCurPage];
}
