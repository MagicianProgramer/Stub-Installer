#pragma once

#include "../common/common.h"
#include "afxwin.h"

// CAddRegistryDlg dialog

class CAddRegistryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddRegistryDlg)

public:
	CAddRegistryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddRegistryDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SYSTEM_REGISTRY_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButtonValueData();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();
	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	BOOL	m_bSetData;
	CString m_strRegKey;
	CString m_strRegValue;
	CString m_strValueData;
	INT		m_nOverwirte;
	INT		m_nRemoveOnUninstall;
	CString m_strBitness;
	CString m_strValueType;

	CComboBox m_ctrlComboBitness;
	CComboBox m_ctrlComboValueType;
	CButton m_ctrlCheckOverWrite;
	CButton m_ctrlCheckRemove;
public:
	void SetData(ADDREGISTRYVALUE *addReg);
	void GetData(ADDREGISTRYVALUE *addReg);

	
};
