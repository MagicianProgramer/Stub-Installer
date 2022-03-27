#pragma once
#include "afxdlgs.h"
#include "InterfaceMainDlg.h"
#include "InterfaceUserInfoDlg.h"
#include "InterfaceCustomDlg.h"
#include "InterfaceFinishDlg.h"
#include "InterfaceLanguagesDlg.h"

#include <vector>
using namespace std;

class CInterfacePropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CInterfacePropertySheet)
public:

	CInterfacePropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CInterfacePropertySheet(void);
	void SetActive1stPage();
	void SetMainExecutable2FinishTab();
private:
	CInterfaceMainDlg m_InterfaceMainDlg;
	CInterfaceUserInfoDlg m_InterfaceUserInfoDlg;
	CInterfaceCustomDlg m_InterfaceCustomDlg;
	CInterfaceFinishDlg m_InterfaceFinishDlg;
	CInterfaceLanguagesDlg m_InterfaceLanguages;

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

