#pragma once

#include "../common/common.h"
#include "afxwin.h"
// CAddIniValue dialog

class CAddIniValue : public CDialogEx
{
	DECLARE_DYNAMIC(CAddIniValue)

public:
	CAddIniValue(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddIniValue();

// Dialog Data
	enum { IDD = IDD_DLG_SYSTEM_INI_FILES_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();
	afx_msg void OnBnClickedButtonIni();
	afx_msg void OnBnClickedButtonValue();
	
	DECLARE_MESSAGE_MAP()

public:
	void SetData(ADDINIVALUE *addIni);
	void GetData(ADDINIVALUE *addIni);
private:
	CString m_strIniFile;
	CString m_strSection;
	CString m_strKey;
	CString m_strValue;
	INT m_nSkip;
	INT m_nOverwrite;
	INT m_nRemoveUninstall;
	CButton m_ctrlCheckSkip;
	CButton m_ctrlOverwrite;
	CButton m_ctrlRemove;
};
