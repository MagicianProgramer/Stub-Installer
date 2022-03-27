#pragma once

#include "WizardPropertySheet.h"

// CInstallerWizard dialog

class CInstallerWizardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInstallerWizardDlg)

public:
	CInstallerWizardDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallerWizardDlg();

// Dialog Data
	enum { IDD = IDD_INSTALLER_WIZARD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CWizardPropertySheet m_sheet;
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckWizardDonotShowOnStartup();
	afx_msg void OnBnClickedBtnWizardNext();
	afx_msg void OnBnClickedBtnWizardClose();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
