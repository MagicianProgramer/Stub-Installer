// GeneralInformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GeneralInformationDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"


// CGeneralInformationDlg dialog

IMPLEMENT_DYNAMIC(CGeneralInformationDlg, CPropertyPage)

CGeneralInformationDlg::CGeneralInformationDlg()
	: CPropertyPage(CGeneralInformationDlg::IDD)
{
	
}

CGeneralInformationDlg::~CGeneralInformationDlg()
{
}

void CGeneralInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGeneralInformationDlg, CPropertyPage)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_NAME, &CGeneralInformationDlg::OnEnChangeEditProductName)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_VERSION, &CGeneralInformationDlg::OnEnChangeEditProductVersion)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_DESCRIPTION, &CGeneralInformationDlg::OnEnChangeEditProductDescription)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_COMPANY_NAME, &CGeneralInformationDlg::OnEnChangeEditProductCompanyName)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_WEB_SITE, &CGeneralInformationDlg::OnEnChangeEditProductWebSite)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_SUPPORT_PAGE, &CGeneralInformationDlg::OnEnChangeEditProductSupportPage)
END_MESSAGE_MAP()


// CGeneralInformationDlg message handlers


void CGeneralInformationDlg::OnEnChangeEditProductName()
{
	// TODO:  Add your control notification handler code her
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_NAME, str);
	wsprintf(g_globaldata.general_sh1.info.wszProductName, str.GetBuffer());
}


void CGeneralInformationDlg::OnEnChangeEditProductVersion()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_VERSION, str);
	wsprintf(g_globaldata.general_sh1.info.wszProductVersion, str.GetBuffer());
}


void CGeneralInformationDlg::OnEnChangeEditProductDescription()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_DESCRIPTION, str);
	wsprintf(g_globaldata.general_sh1.info.wszProductDescription, str.GetBuffer());
}


void CGeneralInformationDlg::OnEnChangeEditProductCompanyName()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_COMPANY_NAME, str);
	wsprintf(g_globaldata.general_sh1.info.wszCompanyName, str.GetBuffer());
}


void CGeneralInformationDlg::OnEnChangeEditProductWebSite()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_WEB_SITE, str);
	wsprintf(g_globaldata.general_sh1.info.wszWebSite, str.GetBuffer());
}


void CGeneralInformationDlg::OnEnChangeEditProductSupportPage()
{
	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_PRODUCT_SUPPORT_PAGE, str);
	wsprintf(g_globaldata.general_sh1.info.wszSupportPage, str.GetBuffer());
}


BOOL CGeneralInformationDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	SetDlgItemText(IDC_EDIT_PRODUCT_NAME, g_globaldata.general_sh1.info.wszProductName);
	SetDlgItemText(IDC_EDIT_PRODUCT_VERSION, g_globaldata.general_sh1.info.wszProductVersion);
	SetDlgItemText(IDC_EDIT_PRODUCT_DESCRIPTION, g_globaldata.general_sh1.info.wszProductDescription);
	SetDlgItemText(IDC_EDIT_PRODUCT_COMPANY_NAME, g_globaldata.general_sh1.info.wszCompanyName);
	SetDlgItemText(IDC_EDIT_PRODUCT_WEB_SITE, g_globaldata.general_sh1.info.wszWebSite);
	SetDlgItemText(IDC_EDIT_PRODUCT_SUPPORT_PAGE, g_globaldata.general_sh1.info.wszSupportPage);

	return CPropertyPage::OnSetActive();
}
