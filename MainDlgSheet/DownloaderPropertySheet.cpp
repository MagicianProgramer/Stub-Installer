#include "StdAfx.h"
#include "DownloaderPropertySheet.h"
#include "../common/common.h"

IMPLEMENT_DYNAMIC(CDownloaderPropertySheet, CPropertySheet)
	CDownloaderPropertySheet::CDownloaderPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CString strTitle;
	m_nTotalDlgs = 0;

	AddPage(&m_UpdateDlg);
	m_nTotalDlgs++;
	strTitle = L"UPDATE";
	m_vecStrTitle.push_back(strTitle);
}



CDownloaderPropertySheet::~CDownloaderPropertySheet(void)
{
}


BEGIN_MESSAGE_MAP(CDownloaderPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDownloaderPropertySheet::SetActive1stPage()
{
	m_UpdateDlg.m_bRefresh = TRUE;
	SetActivePage(&m_UpdateDlg);
}

LRESULT CDownloaderPropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == UM_INSTALLATION_FILESANDFOLDERS_REFRESH)
	{
		CWnd* pParentWnd = GetParent();
		if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_FILESANDFOLDERS_REFRESH);
		return 0L;
	}
	else if (message == UM_INSTALLATION_FILESANDFOLDERS_REFRESH)
	{
		m_UpdateDlg.m_bRefresh = TRUE;
		return 0L;
	}

	return CPropertySheet::WindowProc(message, wParam, lParam);
}


void CDownloaderPropertySheet::NextPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage++;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}


void CDownloaderPropertySheet::SetPage(INT idx)
{
	SetActivePage(idx % m_nTotalDlgs);
}



void CDownloaderPropertySheet::PrevPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage--;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}

BOOL CDownloaderPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	GetTabControl()->ShowWindow(SW_HIDE);

	return bResult;
}


CString CDownloaderPropertySheet::GetTitle()
{
	int nCurPage = GetActiveIndex();
	return m_vecStrTitle[nCurPage];
}