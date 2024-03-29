#pragma once

#include "resource.h"

// CWizardMainPage

class CWizardUninstWelcomePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardUninstWelcomePage)

public:
	CWizardUninstWelcomePage();
	virtual ~CWizardUninstWelcomePage();

	enum {IDD = IDD_DLG_WELCOME};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CString m_strCompanyURL;
	CFont m_Font;
protected:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void SetNoticeText(CString strText, CString strCompanyURL);
	afx_msg void OnNMClickSyslinkCompany(NMHDR *pNMHDR, LRESULT *pResult);
};


