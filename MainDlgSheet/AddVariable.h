#pragma once

#include "../common/common.h"
#include "afxwin.h"
// CAddVariable dialog

class CAddVariable : public CDialogEx
{
	DECLARE_DYNAMIC(CAddVariable)

public:
	CAddVariable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddVariable();

// Dialog Data
	enum { IDD = IDD_DLG_SYSTEM_VARIABLES_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDefaultData();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHelp();
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()

public:
	void SetData(ADDVARIABLE *addVar);
	void GetData(ADDVARIABLE *addVar);
private:
	CString m_strVariable;
	CString m_strRegKey;
	CString m_strRegValue;
	CString m_strDefaultData;
	CString m_strBitness;
	CString m_strValueType;
	BOOL	m_bSetData;

	CComboBox m_ctrlComboRegBitness;
	CComboBox m_ctrlComboValueType;
};
