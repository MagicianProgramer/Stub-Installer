#include "StdAfx.h"
#include "InstallationPropertySheet.h"
#include "../common/common.h"

IMPLEMENT_DYNAMIC(CInstallationPropertySheet, CPropertySheet)
CInstallationPropertySheet::CInstallationPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	CString strTitle;
	m_nTotalDlgs = 0;

	AddPage(&m_InstallationFilesAndFoldersDlg);
	m_nTotalDlgs++;
	strTitle = L"FILES AND FOLDERS";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_InstallationShortcutsDlg);
	m_nTotalDlgs++;
	strTitle = L"SHORTCUTS";
	m_vecStrTitle.push_back(strTitle);

	AddPage(&m_InstallationUninstallationDlg);
	m_nTotalDlgs++;
	strTitle = L"UNINSTALLATION";
	m_vecStrTitle.push_back(strTitle);

	//AddPage(&m_InstallationUpdaterDlg);//correct dialog in rc
	
	AddPage(&m_InstallationAdvancedDlg);
	m_nTotalDlgs++;
	strTitle = L"ADVANCED";
	m_vecStrTitle.push_back(strTitle);
}



CInstallationPropertySheet::~CInstallationPropertySheet(void)
{
}


BEGIN_MESSAGE_MAP(CInstallationPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CHighColorPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInstallationPropertySheet::SetActive1stPage(BOOL bNew)
{
	m_InstallationFilesAndFoldersDlg.m_bRefresh = TRUE;
	m_InstallationUninstallationDlg.m_bRefresh = TRUE;
	m_InstallationShortcutsDlg.m_bRefresh = TRUE;
	//m_InstallationUpdaterDlg.m_bRefresh = TRUE;
	SetActivePage(&m_InstallationFilesAndFoldersDlg);

	if (bNew)	m_InstallationFilesAndFoldersDlg.SettingInstallFilesFromWizard();
}

LRESULT CInstallationPropertySheet::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (message)
	{
	case UM_INSTALLATION_SHORTCUT_REFRESH:
		{
			m_InstallationShortcutsDlg.m_bRefresh = TRUE;

			return 0L;
		}
		break;
	case UM_INSTALLATION_CREATE_SHORTCUT:
		{
			wchar_t* wszTargetFile = (wchar_t*)wParam;
			CString strTarget;
			strTarget.Format(L"%s", wszTargetFile);
			delete[] wszTargetFile;

			//shortcut page//
			SetActivePage(&m_InstallationShortcutsDlg);
			m_InstallationShortcutsDlg.AddCreateShortcutFromFilesAndFolders(strTarget);
			return 0L;
		}
		break;
	case UM_INSTALLATION_MAIN_SHORTCUT:
		{
			m_InstallationShortcutsDlg.SetMainShortcut();
			return 0L;
		}
		break;
	case UM_INSTALLATION_UNINSTALLATION_REFRESH:
		{
			m_InstallationUninstallationDlg.m_bRefresh = TRUE;
			return 0L;
		}
		break;
	case UM_INSTALLATION_UPDATER_REFRESH:
		{
			CWnd* pParentWnd = GetParent();
			if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UPDATER_REFRESH);
			return 0L;
		}
		break;
	case UM_INSTALLATION_FILESANDFOLDERS_REFRESH:
		{
			m_InstallationFilesAndFoldersDlg.m_bRefresh = TRUE;
			return 0L;
		}
		break;
	}

	return CPropertySheet::WindowProc(message, wParam, lParam);
}


void CInstallationPropertySheet::AddFontFileIntoFilesAndFoldersPage(CString strFontFileFullPath)
{
	m_InstallationFilesAndFoldersDlg.AddFontFile(strFontFileFullPath);
	m_InstallationFilesAndFoldersDlg.m_bRefresh = TRUE;
	SetActivePage(&m_InstallationFilesAndFoldersDlg);
}

void CInstallationPropertySheet::NextPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage++;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}

void CInstallationPropertySheet::SetPage(INT idx)
{
	SetActivePage(idx % m_nTotalDlgs);
}


void CInstallationPropertySheet::PrevPage()
{
	int nCurPage = GetActiveIndex();

	nCurPage--;
	nCurPage = (nCurPage + m_nTotalDlgs) % m_nTotalDlgs;

	SetActivePage(nCurPage);
}

BOOL CInstallationPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here
	GetTabControl()->ShowWindow(SW_HIDE);

	return bResult;
}

CString CInstallationPropertySheet::GetTitle()
{
	int nCurPage = GetActiveIndex();
	return m_vecStrTitle[nCurPage];
}