#pragma once

#include "resource.h"

// CWizardMainPage

class CWizardUninstProgPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardUninstProgPage)

public:
	CWizardUninstProgPage();
	virtual ~CWizardUninstProgPage();

	enum {IDD = IDD_DLG_PROG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void UninstallProc();
	void CompleteUninstall();
	void LstUninstall();

protected:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnSetActive();
public:
	CProgressCtrl m_ctrlProgUninstall;
	
	CString m_strMsg;
	void DeleteFolder(CString szDir, BOOL bDeleteSelf, int *pCnt = NULL);
};


