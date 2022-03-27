#pragma once
#include "afxwin.h"


// CInstallationUninstallationDlg dialog

class CInstallationUninstallationDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInstallationUninstallationDlg)

public:
	CInstallationUninstallationDlg();
	virtual ~CInstallationUninstallationDlg();
	BOOL m_bRefresh;
// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_UNINSTALLATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckIncludeUninstaller();
	afx_msg void OnBnClickedCheckAddToControlpanel();
	afx_msg void OnBnClickedCheckSilentMode();
	afx_msg void OnBnClickedCheckCompleteUninstallation();
	afx_msg void OnBnClickedCheckCloseMainExcutable();
	afx_msg void OnBnClickedCheckCloseRunningApplication();
	afx_msg void OnEnChangeEditCraFile();
	afx_msg void OnEnChangeEditCraName();
	afx_msg void OnBnClickedCheckPromptToRemoveProductSettingData();
	afx_msg void OnEnChangeEditCommand();
	afx_msg void OnBnClickedCheckPromptToVisitProductFeedbackPage();
	afx_msg void OnEnChangeEditUrl();
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bIncludeUninstaller;
	BOOL m_bAddToControl;
	BOOL m_bSilentMode;
	BOOL m_bCompleteUninstallation;
	BOOL m_bRestartComputer;
	BOOL m_bCloseMainExcutable;
	BOOL m_bCloseRunningApplication;
	CString m_strCRAFile;
	CString m_strCRAName;
	BOOL m_bPromptToRemoveSettingData;
	CString m_strCommand;
	BOOL m_bPromptToVisitFeadbackPage;
	CString m_strURL;

	void SetComponent();
private:
	CButton m_ctrlCheckIncludeUninst;
	CButton m_ctrlCheckAddToControl;
	CButton m_ctrlCheckSilentMode;
	CButton m_ctrlCheckCompleteUninst;
	CButton m_ctrlCheckRestartCom;
	CButton m_ctrlCheckCloseMainExe;
	CButton m_ctrlCheckCloseRunningApp;
	CButton m_ctrlCheckPromptToRemove;
	CButton m_ctrlCheckPromptToVisit;
public:
	afx_msg void OnBnClickedCheckUninstRestartComputer();
};
