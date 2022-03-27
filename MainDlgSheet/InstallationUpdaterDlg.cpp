// InstallationUpdaterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationUpdaterDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"


// CInstallationUpdaterDlg dialog

IMPLEMENT_DYNAMIC(CInstallationUpdaterDlg, CPropertyPage)

CInstallationUpdaterDlg::CInstallationUpdaterDlg()
	: CPropertyPage(CInstallationUpdaterDlg::IDD)
{

}

CInstallationUpdaterDlg::~CInstallationUpdaterDlg()
{
}

void CInstallationUpdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstallationUpdaterDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_INCLUDE_UPDATER, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterIncludeUpdater)
	ON_EN_CHANGE(IDC_EDIT_INSTALLATION_UPDATER_URL, &CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterUrl)
	ON_BN_CLICKED(IDC_BTN_INSTALLATION_UPDATER_URL_OPEN, &CInstallationUpdaterDlg::OnBnClickedBtnInstallationUpdaterUrlOpen)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_PRODUCT_NAME_IN_UPDATER, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterProductNameInUpdater)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_ALLOW_USERS_TO_CHANGE_OPTIONS, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterAllowUsersToChangeOptions)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_USE_DOWNLOADER, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterUseDownloader)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_AUTO_CHECK_FOR_UPDATES, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterAutoCheckForUpdates)
	ON_CBN_SELCHANGE(IDC_COMBO_INSTALLATION_UPDATER_AUTO_CHECK_METHOD, &CInstallationUpdaterDlg::OnCbnSelchangeComboInstallationUpdaterAutoCheckMethod)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_INSTALL_UPDATES_SILENTLY, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterInstallUpdatesSilently)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_UPDATER_USE_SMART_NOTIFICATION, &CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterUseSmartNotification)
	ON_CBN_SELCHANGE(IDC_COMBO_INSTALLATION_UPDATER_SMART_NOTIFICATION_METHOD, &CInstallationUpdaterDlg::OnCbnSelchangeComboInstallationUpdaterSmartNotificationMethod)
	ON_EN_CHANGE(IDC_EDIT_INSTALLATION_UPDATER_ADDITIONAL_PARAMS, &CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterAdditionalParams)
	ON_EN_CHANGE(IDC_EDIT_INSTALLATION_UPDATER_PASS_PARAMS_TO_NEW_VERSION_URL, &CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterPassParamsToNewVersionUrl)
END_MESSAGE_MAP()


// CInstallationUpdaterDlg message handlers

void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterIncludeUpdater()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_INCLUDE_UPDATER);
	SetComponents();
	CWnd* pParentWnd = GetParent();
	if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_FILESANDFOLDERS_REFRESH);
}


void CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterUrl()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_URL, str);
	wsprintf(g_globaldata.install_sh2.updater.wszUpdateInformationFileURL, str.GetBuffer());
}


void CInstallationUpdaterDlg::OnBnClickedBtnInstallationUpdaterUrlOpen()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"open", g_globaldata.install_sh2.updater.wszUpdateInformationFileURL, NULL, NULL, SW_SHOWNORMAL);
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterProductNameInUpdater()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkProductNameInUpdaterTitle = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_PRODUCT_NAME_IN_UPDATER);
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterAllowUsersToChangeOptions()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkAllowUsersToChangeTheUpdaterOptions = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_ALLOW_USERS_TO_CHANGE_OPTIONS);
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterUseDownloader()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkUseDownloader = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_USE_DOWNLOADER);
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterAutoCheckForUpdates()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkAutomaticallyCheckForUpdates = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_AUTO_CHECK_FOR_UPDATES);
}


void CInstallationUpdaterDlg::OnCbnSelchangeComboInstallationUpdaterAutoCheckMethod()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nAutoCheckMethod = (AUTO_CHECK_METHOD)((CComboBox*)GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_AUTO_CHECK_METHOD))->GetCurSel();
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterInstallUpdatesSilently()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkInstallUpdatesSilently = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_INSTALL_UPDATES_SILENTLY);
}


void CInstallationUpdaterDlg::OnBnClickedCheckInstallationUpdaterUseSmartNotification()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nChkUseSmartNotification = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_UPDATER_USE_SMART_NOTIFICATION);
}


void CInstallationUpdaterDlg::OnCbnSelchangeComboInstallationUpdaterSmartNotificationMethod()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.updater.nSmartNotificationMethod = (SMART_NOTIFICATION_METHOD)((CComboBox*)GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_SMART_NOTIFICATION_METHOD))->GetCurSel();
}


void CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterAdditionalParams()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_ADDITIONAL_PARAMS, str);
	wsprintf(g_globaldata.install_sh2.updater.wszAdditionalParams, str.GetBuffer());
}


void CInstallationUpdaterDlg::OnEnChangeEditInstallationUpdaterPassParamsToNewVersionUrl()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_PASS_PARAMS_TO_NEW_VERSION_URL, str);
	wsprintf(g_globaldata.install_sh2.updater.wszPassParamsToNewVersionURL, str.GetBuffer());
}

BOOL CInstallationUpdaterDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bRefresh)
	{
		SetComponents();
		
		//check box//
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_INCLUDE_UPDATER, g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_PRODUCT_NAME_IN_UPDATER, g_globaldata.install_sh2.updater.nChkProductNameInUpdaterTitle);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_ALLOW_USERS_TO_CHANGE_OPTIONS, g_globaldata.install_sh2.updater.nChkAllowUsersToChangeTheUpdaterOptions);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_USE_DOWNLOADER, g_globaldata.install_sh2.updater.nChkUseDownloader);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_AUTO_CHECK_FOR_UPDATES, g_globaldata.install_sh2.updater.nChkAutomaticallyCheckForUpdates);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_INSTALL_UPDATES_SILENTLY, g_globaldata.install_sh2.updater.nChkInstallUpdatesSilently);
		CheckDlgButton(IDC_CHECK_INSTALLATION_UPDATER_USE_SMART_NOTIFICATION, g_globaldata.install_sh2.updater.nChkUseSmartNotification);

		//combobox//
		((CComboBox*)GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_AUTO_CHECK_METHOD))->SetCurSel(g_globaldata.install_sh2.updater.nAutoCheckMethod);
		((CComboBox*)GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_SMART_NOTIFICATION_METHOD))->SetCurSel(g_globaldata.install_sh2.updater.nSmartNotificationMethod);

		//edit//
		SetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_URL, g_globaldata.install_sh2.updater.wszUpdateInformationFileURL);
		SetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_ADDITIONAL_PARAMS, g_globaldata.install_sh2.updater.wszAdditionalParams);
		SetDlgItemText(IDC_EDIT_INSTALLATION_UPDATER_PASS_PARAMS_TO_NEW_VERSION_URL, g_globaldata.install_sh2.updater.wszPassParamsToNewVersionURL);
	}

	return CPropertyPage::OnSetActive();
}

void CInstallationUpdaterDlg::SetComponents()
{
	BOOL bEnable = (BOOL)g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool;
	GetDlgItem(IDC_EDIT_INSTALLATION_UPDATER_URL)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_PRODUCT_NAME_IN_UPDATER)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_ALLOW_USERS_TO_CHANGE_OPTIONS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_USE_DOWNLOADER)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_AUTO_CHECK_FOR_UPDATES)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_INSTALL_UPDATES_SILENTLY)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_INSTALLATION_UPDATER_USE_SMART_NOTIFICATION)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_INSTALLATION_UPDATER_ADDITIONAL_PARAMS)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_INSTALLATION_UPDATER_PASS_PARAMS_TO_NEW_VERSION_URL)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_AUTO_CHECK_METHOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_INSTALLATION_UPDATER_SMART_NOTIFICATION_METHOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BTN_INSTALLATION_UPDATER_URL_OPEN)->EnableWindow(bEnable);
}
