#pragma once
#include "afxwin.h"
#include <objbase.h>

// CGeneralParametersDlg dialog

class CGeneralParametersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralParametersDlg)

public:
	CGeneralParametersDlg();   // standard constructor
	virtual ~CGeneralParametersDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_PARAMETERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedBtnChange();
	afx_msg void OnCbnSelchangeComboInstallProductFor();
	afx_msg void OnEnChangeEditProductGuid();
	afx_msg void OnBnClickedCheckUninstallPreviousVersion();
	afx_msg void OnBnClickedCheckUninstallSilently();
	afx_msg void OnBnClickedCheckAllowMultipleVersions();
	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()

private:
	INT m_nInstallModeIdx;
	INT m_nInstallProductIdx;
	CString m_strProductGUID;
	INT m_nCHkUninstPrev;
	INT m_nChkUninstSilently;
	INT m_nChkAllowMulti;
	INT m_nChangeBtnEnable;
	
protected:
	virtual BOOL OnInitDialog();

public:
	wchar_t *GenerateProductGuid();

private:
	CComboBox m_ctrlComboInstallPro;
	CButton m_ctrlCheckUninstPrev;
	CButton m_ctrlCheckUninstSilently;
	CButton m_ctrlCheckAllow;
};
