#pragma once
#include "afxcmn.h"
#include "../common/common.h"
#include "afxwin.h"
#define ADD_TYPE	0
#define EDIT_TYPE	1
// CAddPrerequisite dialog

class CAddPrerequisite : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPrerequisite)

public:
	CAddPrerequisite(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddPrerequisite();

	void SetData(ADDPREREQUISITE *pAddData);

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_PREREQUISITES_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedBtnAddPrerequisiteOpenCommand();
	afx_msg void OnBnClickedBtnAddToList();
	afx_msg void OnBnClickedBtnAddPrerequisiteChange();
	afx_msg void OnBnClickedBtnAddPrerequisiteRemove();
	afx_msg void OnBnClickedBtnAddPrerequisiteOk();
	afx_msg void OnBnClickedBtnAddPrerequisiteCancel();
	afx_msg void OnBnClickedBtnAddPrerequisiteHelp();
	afx_msg void OnNMClickListAddPrerequisiteCond(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CString m_strName;
	CString m_strCommand;
	CString m_strOS;
	CString m_strConditionMatch;
	INT		m_nChkSkip;
	CString m_strConditions;
	
	INT m_nSelCond;
	BOOL m_bType;
	void InitLst();

	CComboBox m_ctrlComboCondMatch;
	CButton m_ctrlCheckSkip;
	CComboBox m_ctrlComboName;

public:
	CListCtrl m_listCtrlCond;
	vector<ADDPREREQUISITE> m_vecAddedPre;
};
