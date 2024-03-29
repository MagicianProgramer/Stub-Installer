#pragma once

#include "resource.h"
#include "WizardUninstallerSheet.h"

// CWizardMainPage

class CMainUninstPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMainUninstPage)

public:
	CMainUninstPage();
	virtual ~CMainUninstPage();

	enum {
		IDD = IDD_UNINSTALLER_WIZARD
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CWizardUninstallerPropertySheet m_wzdSht;
private:
	void GetOptions();

protected:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnWizardNext();
	afx_msg void OnBnClickedBtnWizardCancel();
public:
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


