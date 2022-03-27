#pragma once
#include "afxwin.h"


// CInstallationAdvancedDlg dialog

class CInstallationAdvancedDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInstallationAdvancedDlg)

public:
	CInstallationAdvancedDlg();
	virtual ~CInstallationAdvancedDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_ADVANCED };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckAutoLaunchOnWindowsStartup();
	afx_msg void OnCbnSelchangeComboModeSelectDestinationFolder();
	afx_msg void OnEditchangeComboAlternativeInstallationDirectory();
	afx_msg void OnCbnSelchangeComboDefaultLanguageFunction();
	afx_msg void OnBnClickedCheckAllowInstallNoRequiedFreeSpace();
	afx_msg void OnCbnSelchangeComboUninstallerFileName();
	afx_msg void OnEditchangeComboExtractParameterForDataFile();
	afx_msg void OnCbnSelchangeComboAlternativeInstallationDirectory();
	afx_msg void OnCbnSelchangeComboExtractParameterForDataFile();
	afx_msg void OnBnClickedCheckAllowIfFailedPrerequisites();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnEnChangeEditUninstallerFileName();
	DECLARE_MESSAGE_MAP()

private:
	INT m_nModeSelectDestinationFolder;
	INT m_nUninstallerFileName;
	INT m_nDefaultLaguageFunction;	
	CString m_strAlterInstDir;
	CString m_strExtractParam;
	
	wchar_t m_wszAlterInstDir[6][MAX_PATH];
	wchar_t m_wszExtractParam[2][MAX_PATH];

	CButton m_ctrlCheckAuto;
	CComboBox m_ctrlComboSelFolder;
	CComboBox m_ctrlComboAlterInstDir;
	CComboBox m_ctrlComboExtractParam;
	CComboBox m_ctrlComboUninstFileName;
	CComboBox m_ctrlComboDefaultLang;

	CButton m_ctrlCheckAllowInstallNoFreeSpace;
	CButton m_ctrlCheckAllowInstallFailedPrerequisites;
public:
	
};
