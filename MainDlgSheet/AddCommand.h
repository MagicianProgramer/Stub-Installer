#pragma once
#include "../common/common.h"
#include "afxwin.h"

// CAddCommand dialog

class CAddCommand : public CDialogEx
{
	DECLARE_DYNAMIC(CAddCommand)

public:
	CAddCommand(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddCommand();

// Dialog Data
	enum { IDD = IDD_DLG_SYSTEM_COMMANDS_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonParameters();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();

	DECLARE_MESSAGE_MAP()
private:
	CString m_strLaunchOS;
	CString m_strShow;
	CString m_strTiming;
	CString m_strFile;
	CString m_strParams;
	
	INT m_nWait;
	INT m_nRun;
	CComboBox m_ctrlLaunch;
	CComboBox m_ctrlShow;
	CComboBox m_ctrlTiming;
	CComboBox m_ctrlFile;
	CButton m_ctrlWait;
	CButton m_ctrlRun;
	BOOL	m_bSetData;

public:
	void SetData(ADDCOMMAND *addCmd);
	void GetData(ADDCOMMAND *addCmd);
};
