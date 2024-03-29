#pragma once

#include "resource.h"

// CWizardMainPage

class CWizardUninstFinishPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWizardUninstFinishPage)

public:
	CWizardUninstFinishPage();
	virtual ~CWizardUninstFinishPage();

	enum {IDD = IDD_DLG_FINISH};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CFont m_Font;
protected:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	void SetNoticeText(CString strTxt);
};


