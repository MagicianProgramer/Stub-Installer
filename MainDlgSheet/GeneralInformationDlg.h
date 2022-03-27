#pragma once
#include "resource.h"
// CGeneralInformationDlg dialog

class CGeneralInformationDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralInformationDlg)

public:
	CGeneralInformationDlg();
	virtual ~CGeneralInformationDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_INFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnEnChangeEditProductName();
	afx_msg void OnEnChangeEditProductVersion();
	afx_msg void OnEnChangeEditProductDescription();
	afx_msg void OnEnChangeEditProductCompanyName();
	afx_msg void OnEnChangeEditProductWebSite();
	afx_msg void OnEnChangeEditProductSupportPage();
	virtual BOOL OnSetActive();

	DECLARE_MESSAGE_MAP()
};
