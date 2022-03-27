#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CInterfaceCustomDlg dialog

class CInterfaceCustomDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfaceCustomDlg)

public:
	CInterfaceCustomDlg();
	virtual ~CInterfaceCustomDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INTERFACE_CUSTOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckBoxChk();
	afx_msg void OnBnClickedCheckedChk();
	afx_msg void OnBnClickedComboBoxChk();
	afx_msg void OnBnClickedSelectComponentsChk();
	afx_msg void OnBnClickedSelComponentsAtLeastChk();
	afx_msg void OnBnClickedSelComponentsCalculateChk();
	afx_msg void OnCbnSelchangeCalculateMethod();
	afx_msg void OnEnChangeEditCheckText();
	afx_msg void OnEnChangeEditComboText();
	afx_msg void OnLbnSelchangeListItems();
	afx_msg void OnEnChangeEditComboDefault();
	afx_msg void OnEnChangeEditComponentName();
	afx_msg void OnEnChangeEditComponentDescription();
	afx_msg void OnEnChangeListItems();
	
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCheckboxChk;
	BOOL m_bCheckedChk;
	BOOL m_bComboBoxChk;
	BOOL m_bSelectComponentsChk;
	BOOL m_bSelComponentAtLeastChk;
	BOOL m_bSelComponentCalculateChk;

	CListCtrl m_ctrlList;
	CString m_strCheckBoxText;
	CString m_strComboBoxText;
	CListBox m_ctrlListItems;
	CString m_strSelComponentName;
	CString m_strSelComponentDesc;
	INT m_nCustomComboDefault;
	INT m_nCalculateMethod;
	CString m_strItems;

	CComboBox m_ctrlComboComponent;
	CButton m_ctrlCheckComponentAtLeast;
	CButton m_ctrlCheckComponentCalculate;
	CButton m_ctrlCheckChecked;
	CButton m_ctrlCheckBoxChk;
	CButton m_ctrlComboBoxChk;
	CButton m_ctrlCheckComponent;

protected:
	virtual BOOL OnSetActive();	
public:
	
};
