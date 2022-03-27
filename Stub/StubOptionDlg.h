#pragma once

extern CString strUserName;
extern CString strUserEmail;
extern CString strCompany;
extern CString strFieldText;
extern CString strSN;
// CStubOptionDlg dialog
class CStubOptionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStubOptionDlg)

public:
	CStubOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStubOptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STUB_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnBnClickedBtnStubOptionDestinationFolderOpen();
	afx_msg void OnEnChangeEditStubOptionDestinationFolder();
	virtual BOOL OnInitDialog();
	void SetUserInformation();
	afx_msg void OnEnChangeEditStubOptionUserName();
	afx_msg void OnEnChangeEditStubOptionUserEmail();
	afx_msg void OnEnChangeEditStubOptionCompany();
	afx_msg void OnEnChangeEditStubOptionCustomField();
	afx_msg void OnEnChangeEditStubOptionSerialNumber();
	afx_msg void OnBnClickedOk();
	int m_nInstallProductFor;
	afx_msg void OnBnClickedRadioInstallProductForCurrentUser();
public:
	afx_msg void OnBnClickedRadioInstallProductForAllUsers();
};

