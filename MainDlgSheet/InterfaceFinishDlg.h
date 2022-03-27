#pragma once
#include "afxwin.h"


// CInterfaceFinishDlg dialog
class CMyBrowseEdit1 : public CMFCEditBrowseCtrl
{
	virtual void OnBrowse();	
};

class CInterfaceFinishDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfaceFinishDlg)

public:
	CInterfaceFinishDlg();
	virtual ~CInterfaceFinishDlg();
	BOOL m_bRefresh;
	void SetExecutable();
// Dialog Data
	enum { IDD = IDD_DLG_INTERFACE_FINISH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckLaunchApplication();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedCheckLaunchDefault();
	afx_msg void OnBnClickedCheckLaunchRun();
	afx_msg void OnBnClickedCheckCustomAtion();
	afx_msg void OnEnChangeEditProductGuid3();
	afx_msg void OnEnChangeEditProductGuid();
	afx_msg void OnBnClickedButtonCommand();
	afx_msg void OnBnClickedCheckCustomDefault();
	afx_msg void OnBnClickedCheckCustomRun();
	afx_msg void OnBnClickedCheckRestartComputer2();
	afx_msg void OnEnChangeEditLaunchAppFile();
	afx_msg void OnEnChangeEditCustomActionCommand();
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bLaunchApp;
	CString m_strLaunchFile;
	BOOL m_bLaunchDefault;
	BOOL m_bLauchRunAsAdmin;
	BOOL m_bCustomAction;
	CString m_strCustomName;
	CString m_strCustomCommand;
	BOOL m_bCustomDefault;
	BOOL m_bCustomAdmin;
	BOOL m_bRestartComputer;	

	CButton m_ctrlCheckLaunchApp;
	CButton m_ctrlCheckLaunchDef;
	CButton m_ctrlCheckLaunchRun;
	CButton m_ctrlCheckCustom;
	CButton m_ctrlCheckCustomDef;
	CButton m_ctrlCheckCustomRun;
	CButton m_ctrlCheckRestart;

	CMyBrowseEdit1 m_FinishLaunchAppFileEditCtrl;
	CMyBrowseEdit1 m_FinishCustomActionCommandEditCtrl;
protected:
	virtual BOOL OnInitDialog();
};
