#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CGeneralPrequisitesDlg dialog

class CGeneralPrequisitesDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralPrequisitesDlg)

public:
	CGeneralPrequisitesDlg();
	virtual ~CGeneralPrequisitesDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_PREREQUISITES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckNetFramework();
	afx_msg void OnBnClickedCheckInternetExplorer();
	afx_msg void OnBnClickedCheckAdobeAcrobatReader();
	afx_msg void OnBnClickedCheckJava();
	afx_msg void OnBnClickedCheckSqlServerCompact();
	afx_msg void OnBnClickedCheckSqlServerExpress();
	afx_msg void OnBnClickedCheckVisualRuntime();
	afx_msg void OnCbnSelchangeComboNetFramework();
	afx_msg void OnCbnSelchangeComboInternetExplorer();
	afx_msg void OnCbnSelchangeComboAdobeAcrobatReader();
	afx_msg void OnCbnSelchangeComboJava();
	afx_msg void OnCbnSelchangeComboSqlServerCompact();
	afx_msg void OnCbnSelchangeComboSqlServerExpress();
	afx_msg void OnCbnSelchangeComboVisualRuntimeX86();
	afx_msg void OnNMClickSyslinkViewEditLinks(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnDuplicate();
	afx_msg void OnBnClickedBtnApAdd();
	afx_msg void OnBnClickedBtnApEdit();
	afx_msg void OnBnClickedBtnApRemove();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	INT m_nChkDotNet;
	INT m_nChkInterExplorer;
	INT m_nChkAcrobat;
	INT m_nChkJava;
	INT m_nChkSQLCompact;
	INT m_nChkSQLExpress;
	INT m_nChkVCRuntime86;
	INT m_nDotNetIdx;
	INT m_nInterExpIdx;
	INT m_nAcrobatIdx;
	INT m_nJavaIdx;
	INT m_nSqlCompactIdx;
	INT m_nSqlExpressIdx;
	INT m_nVCRuntimeX86Idx;

	INT m_nSelAddPre;

	void SetViewComponent();
	CListCtrl m_listCtrlAddPre;
	void InitLst();
	CString GetChkString(INT k);

protected:
	virtual BOOL OnSetActive();
	afx_msg void OnNMClickListAdditionalPrerequisites(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListAdditionalPrerequisites(NMHDR *pNMHDR, LRESULT *pResult);

private:
	CComboBox m_ctrlComboNetFramework;
	CComboBox m_ctrlComboInternetExplorer;
	CComboBox m_ctrlComboAcrobat;
	CComboBox m_ctrlComboJava;
	CComboBox m_ctrlComboSQLCompact;
	CComboBox m_ctrlComboSQLExpress;
	CComboBox m_ctrlComboVCRuntimeX86;
	CButton m_ctrlCheckNetFramework;
	CButton m_ctrlCheckInternetExplorer;
	CButton m_ctrlCheckAcrobat;
	CButton m_ctrlCheckJava;
	CButton m_ctrlCheckSQLCompact;
	CButton m_ctrlCheckSQLExpress;
	CButton m_ctrlCheckVCRuntimeX86;

	CComboBox m_ctrlComboVCRuntimeX64;
	CComboBox m_ctrlComboVCRuntimeARM64;
	afx_msg void OnBnClickedCheckVisualRuntimeX64();
	afx_msg void OnBnClickedCheckVisualRuntimeArm64();
	afx_msg void OnCbnSelchangeComboVisualRuntimeX64();
	afx_msg void OnCbnSelchangeComboVisualRuntimeArm64();
	CButton m_ctrlCheckVCRuntimeX64;
	CButton m_ctrlCheckVCRuntimeARM64;
};
