#pragma once
#include "afxwin.h"


// CInterfaceUserInfoDlg dialog

class CInterfaceUserInfoDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfaceUserInfoDlg)

public:
	CInterfaceUserInfoDlg();
	virtual ~CInterfaceUserInfoDlg();
	BOOL m_bRefresh;
// Dialog Data
	enum { IDD = IDD_DLG_INTERFACE_USER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckCompanyName();
	afx_msg void OnBnClickedCheckCustomField();
	afx_msg void OnBnClickedCheckRequiredUserName();
	afx_msg void OnBnClickedCheckRequiredCompanyName();
	afx_msg void OnBnClickedCheckRequiredCustomField();
	afx_msg void OnEnChangeEditFieldText();
	afx_msg void OnCbnSelchangeComboFieldType();
	afx_msg void OnBnClickedCheckFileFolderExist();
	afx_msg void OnBnClickedCheckSerialNumber();
	afx_msg void OnBnClickedCheckRequiredSerialNumber();
	afx_msg void OnEnChangeSerialNumber();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonGenerate();
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedCheckInformationUserEmail();
	afx_msg void OnBnClickedCheckRequiredUserEmail();
	afx_msg void OnBnClickedCheckUserName();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bUserName;
	BOOL m_bCompanyName;
	BOOL m_bCustomField;
	BOOL m_bUserNameRequired;
	BOOL m_bCompanyRequire;
	BOOL m_bCustomRequired;
	BOOL m_bFileFolder;
	BOOL m_bSerialNumber;
	BOOL m_bSNRequired;
	
	CString m_strFieldText;
	CString m_strSNList;
	INT m_nFieldType;

	CButton m_ctrlCheckUserName;
	CButton m_ctrlCheckUserEmail;
	CButton m_ctrlCheckCompany;
	CButton m_ctrlCheckCustomField;
	CComboBox m_ctrlComboFieldType;
	CButton m_ctrlCheckFileFolder;
	CButton m_ctrlCheckSerialNumber;
	CButton m_ctrlCheckSerialReq;
	CButton m_ctrlCheckUserNameReq;
	CButton m_ctrlCheckUserEmailReq;
	CButton m_ctrlCheckCompanyReq;
	CButton m_ctrlCheckCustomReq;
	void SetComponent(BOOL bEnable);
};
