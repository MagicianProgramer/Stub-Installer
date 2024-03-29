
// UninstallerDlg.h : header file
//

#pragma once


// CUninstallerDlg dialog
class CUninstallerDlg : public CDialogEx
{
// Construction
public:
	CUninstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UNINSTALLER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnUninstall();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	void GetOptions();
	void CompleteUninstall();
	void LstUninstall();
};
