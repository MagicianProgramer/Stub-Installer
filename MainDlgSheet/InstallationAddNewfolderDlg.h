#pragma once


// CInstallationAddNewfolderDlg dialog

class CInstallationAddNewfolderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInstallationAddNewfolderDlg)

public:
	CInstallationAddNewfolderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallationAddNewfolderDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_ADD_NEWFOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
public:
	CString m_strAddNewFolderName;
};
