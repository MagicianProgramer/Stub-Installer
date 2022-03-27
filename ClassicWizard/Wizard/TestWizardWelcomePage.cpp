
#include "stdafx.h"
#include "TestWizardWelcomePage.h"
#include "../../common/common.h"
#include "TestWizardSheet.h"

LRESULT CTestWizardWelcomePage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	this->InitializeControls();
	this->InitializeValues();

	return 1;
}

void CTestWizardWelcomePage::InitializeControls(void)
{
	//////////////////////////////////
	CFontHandle fontExteriorPageTitleFont(baseClass::GetExteriorPageTitleFont());
	/*CFontHandle fontBulletFont(baseClass::GetBulletFont());*/

	CWindow title = this->GetDlgItem(IDC_STATIC_WELCOME_TITLE);
	CWindow bullet1 = this->GetDlgItem(IDC_STATIC_WELCOME_PRODUCT);
	CWindow bullet2 = this->GetDlgItem(IDC_STATIC_WELCOME_COPYRIGHT);

	title.SetFont(fontExteriorPageTitleFont);

	CString strTxt;
	strTxt.Format(L"Welcome to %s Setup", g_globaldata.general_sh1.info.wszProductName);
	title.SetWindowText(strTxt);
	
	strTxt.Format(L"This will install %s on your computer", g_globaldata.general_sh1.info.wszProductName);
	bullet1.SetWindowText(strTxt);

	//description//
	strTxt.Format(L"%s", g_globaldata.general_sh1.info.wszProductDescription);
	GetDlgItem(IDC_WIZ97_BULLET1_DESCIPTION).SetWindowText(strTxt);
	
	//company link//
	strTxt.Format(L"<a>%s</a>", g_globaldata.general_sh1.info.wszCompanyName);
	GetDlgItem(IDC_SYSLINK_COMPANY).SetWindowText(strTxt);

	if (!g_globaldata.interface_sh3.main.chkOptionsShowCopyright)
	{
		bullet2.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_SYSLINK_COMPANY).ShowWindow(SW_HIDE);
	}

	/*strTxt.Format(L"Copyright (c) 2021 %s", g_globaldata.general_sh1.info.wszComapanyName);
	bullet2.SetWindowText(strTxt);*/
}

void CTestWizardWelcomePage::InitializeValues(void)
{
	bool showWelcome = m_pTestWizardInfo->GetShowWelcome();
}

bool CTestWizardWelcomePage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CTestWizardWelcomePage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_NEXT);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	int result = 0;

	if(m_allowWelcomeToHide)
	{
		// Have it so that the welcome page is only hidden on
		// the first access, but is available if the user goes
		// "back" to visit it.
		m_allowWelcomeToHide = false;
		/*if(m_buttonSkipWelcome.GetCheck() == BST_CHECKED)
		{
			result = IDD_WIZ97_PATHFILTER;
		}*/
	}

	GetParent().PostMessage(UM_SET_TITLE);

	return result;
}

int CTestWizardWelcomePage::OnWizardNext()
{
// 	if (m_pTestWizardInfo->FindNextPage(IDD))
// 	{
// 		m_pTestWizardInfo->FindNextPage(IDD)
// 	}
// 	return 0;
	bool success = this->StoreValues();
	if(!success)
	{
		// Any errors are already reported, and if appropriate,
		// the control that needs attention has been given focus.
		return -1;
	}

	// 0  = goto next page
	// -1 = prevent page change
	// >0 = jump to page by dlg ID

	return m_pTestWizardInfo->FindNextPage(IDD);
}

void CTestWizardWelcomePage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}

LRESULT CTestWizardWelcomePage::OnBnClickedSysLink(int , LPNMHDR , BOOL&)
{
	ShellExecute(NULL, L"open", g_globaldata.general_sh1.info.wszWebSite, NULL, NULL, SW_NORMAL);
	return 0L;
}

