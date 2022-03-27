#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CInstallationShortcutDlg dialog

class CInstallationShortcutDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInstallationShortcutDlg)

public:
	CInstallationShortcutDlg();
	virtual ~CInstallationShortcutDlg();
	BOOL m_bRefresh;
	void AddCreateShortcutFromFilesAndFolders(CString strTargetFileName);
	void SetMainShortcut();
// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_SHORTCUTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnCbnSelchangeComboProgramGroupName();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListInstallationShortcut(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListInstallationShortcut(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditchangeComboProgramGroupName();

	DECLARE_MESSAGE_MAP()
private:
	CString m_strProgramGroupName;	
	CListCtrl m_listCtrlShortcuts;
	INT m_nSelectedListIdx;
	wchar_t m_wszComboContent[4][MAX_PATH];
	CComboBox m_ctrlComboProGroupName;
};
