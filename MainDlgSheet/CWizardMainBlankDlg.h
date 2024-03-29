#pragma once
#include "WizardMainBlankSheet.h"

// CWizardMainBlankDlg dialog

class CWizardMainBlankDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWizardMainBlankDlg)

public:
	CWizardMainBlankDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CWizardMainBlankDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALLER_WIZARD_MAIN_BLANK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	CWizardMainBlankPropertySheet m_sht;
public:
	virtual BOOL OnInitDialog();
};
