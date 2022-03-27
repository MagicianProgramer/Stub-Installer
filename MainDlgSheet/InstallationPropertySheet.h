#pragma once
#include "afxdlgs.h"

#include "InstallationFilesAndFoldersDlg.h"
#include "InstallationShortcutDlg.h"
#include "InstallationUninstallationDlg.h"
#include "InstallationAdvancedDlg.h"

#include <vector>
using namespace std;

class CInstallationPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CInstallationPropertySheet)
public:

	CInstallationPropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CInstallationPropertySheet(void);
	void SetActive1stPage(BOOL bNew = FALSE);
	void AddFontFileIntoFilesAndFoldersPage(CString strFontFileFullPath);
private:
	CInstallationFilesAndFoldersDlg m_InstallationFilesAndFoldersDlg;
	CInstallationShortcutDlg m_InstallationShortcutsDlg;
	CInstallationUninstallationDlg m_InstallationUninstallationDlg;
	CInstallationAdvancedDlg m_InstallationAdvancedDlg;
	/*CInstallationUpdaterDlg  m_InstallationUpdaterDlg;*/

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void NextPage();
	void PrevPage();
	void SetPage(INT idx);
	CString GetTitle();
private:
	INT m_nTotalDlgs;
	vector<CString> m_vecStrTitle;
public:
	virtual BOOL OnInitDialog();
};

