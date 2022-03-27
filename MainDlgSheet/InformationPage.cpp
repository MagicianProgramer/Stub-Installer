// InformationPage.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InformationPage.h"
#include "afxdialogex.h"

#include "../common/common.h"
#include "WizardPropertySheet.h"


// CInformationPage dialog

IMPLEMENT_DYNAMIC(CInformationPage, CPropertyPage)

CInformationPage::CInformationPage()
	: CPropertyPage(CInformationPage::IDD)
{

}

CInformationPage::~CInformationPage()
{
}

void CInformationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInformationPage, CPropertyPage)
END_MESSAGE_MAP()


// CInformationPage message handlers
void CInformationPage::GetParams()
{
	CString strTmp;
	GetDlgItemText(IDC_EDIT_WIZARD_INFO_PRODUCT_NAME, strTmp);
	wsprintf(g_globaldata.general_sh1.info.wszProductName, strTmp.GetBuffer());

	GetDlgItemText(IDC_EDIT_WIZARD_INFO_VERSION, strTmp);
	wsprintf(g_globaldata.general_sh1.info.wszProductVersion, strTmp.GetBuffer());

	GetDlgItemText(IDC_EDIT_WIZARD_INFO_COMPANY_NAME, strTmp);
	wsprintf(g_globaldata.general_sh1.info.wszCompanyName, strTmp.GetBuffer());

	GetDlgItemText(IDC_EDIT_WIZARD_INFO_WEBSITE, strTmp);
	wsprintf(g_globaldata.general_sh1.info.wszWebSite, strTmp.GetBuffer());
}

BOOL CInformationPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CInformationPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->PostMessage(UM_CURRENT_PAGE);

	SetDlgItemText(IDC_EDIT_WIZARD_INFO_PRODUCT_NAME, g_globaldata.general_sh1.info.wszProductName);
	SetDlgItemText(IDC_EDIT_WIZARD_INFO_VERSION, g_globaldata.general_sh1.info.wszProductVersion);
	SetDlgItemText(IDC_EDIT_WIZARD_INFO_COMPANY_NAME, g_globaldata.general_sh1.info.wszCompanyName);
	SetDlgItemText(IDC_EDIT_WIZARD_INFO_WEBSITE, g_globaldata.general_sh1.info.wszWebSite);

	return CPropertyPage::OnSetActive();
}
