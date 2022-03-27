#pragma once
#include "afxwin.h"


// CInstallationShortcutDetailDlg dialog

class CInstallationShortcutDetailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInstallationShortcutDetailDlg)

public:
	CInstallationShortcutDetailDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallationShortcutDetailDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_SHORTUT_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnTargetFile();
	afx_msg void OnBnClickedBtnIconFile();
	virtual BOOL OnInitDialog();

private:
	CComboBox m_ctrlComboShortcutLocation;
	CComboBox m_ctrlComboShow;

public:
	CString m_strTargetFile;
	CString m_strShortcutLocation;
	CString m_strShortcutName;
	CString m_strDescription;
	CString m_strParameters;
	CString m_strWorkingDirectory;
	CString m_strIconFile;
	INT m_nIndex;
	CString m_strShow;
	
};
