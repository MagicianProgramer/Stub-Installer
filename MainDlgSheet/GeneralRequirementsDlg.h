#pragma once
#include "afxwin.h"

#define  OS_CNT		13
// CGeneralRequirementsDlg dialog

class CGeneralRequirementsDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralRequirementsDlg)

public:
	CGeneralRequirementsDlg();
	virtual ~CGeneralRequirementsDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_REQUIREMENTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCbnSelchangeComboBits3264();
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCheckAdministratorPrivileges();
	afx_msg void OnBnClickedCheckInternetConnection();
	afx_msg void OnBnClickedCheckCloseRunningApp1();
	afx_msg void OnEnChangeEditCra1File();
	afx_msg void OnEnChangeEditCra1Name();
	afx_msg void OnBnClickedCheckCloseRunningApp2();
	afx_msg void OnEnChangeEditCra2File();
	afx_msg void OnEnChangeEditCra2Name();

	DECLARE_MESSAGE_MAP()
private:
	INT m_pChkedOS[OS_CNT];
	INT m_nChkAdminPriv;
	INT m_nChkInternetConnect;
	INT m_nChkCloseApp_1;
	INT m_nChkCloseApp_2;
	CString m_strCloseApp_1_file;
	CString m_strCloseApp_1_name;
	CString m_strCloseApp_2_file;
	CString m_strCloseApp_2_name;
	
	INT m_nOSBitIdx;
	void GetOeratingSystemParam();
	
protected:
	virtual BOOL OnSetActive();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
	CComboBox m_ctrlComboBits;
	CButton m_pctrlCheckOS[OS_CNT];
	CButton m_ctrlCheckAdminPrivileges;
	CButton m_ctrlCheckInternetConnection;
	CButton m_ctrlCheckCloseRunningApp1;
	CButton m_ctrlCheckCloseRunningApp2;
};
