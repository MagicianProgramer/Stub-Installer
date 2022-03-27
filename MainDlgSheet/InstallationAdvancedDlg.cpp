// InstallationAdvancedDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationAdvancedDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"

// CInstallationAdvancedDlg dialog

IMPLEMENT_DYNAMIC(CInstallationAdvancedDlg, CPropertyPage)

CInstallationAdvancedDlg::CInstallationAdvancedDlg()
	: CPropertyPage(CInstallationAdvancedDlg::IDD)
{
	
}

CInstallationAdvancedDlg::~CInstallationAdvancedDlg()
{
	
}

void CInstallationAdvancedDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ALLOW_INSTALL_NO_REQUIED_FREE_SPACE, m_ctrlCheckAllowInstallNoFreeSpace);
	DDX_Control(pDX, IDC_CHECK_ALLOW_IF_FAILED_PREREQUISITES, m_ctrlCheckAllowInstallFailedPrerequisites);
}


BEGIN_MESSAGE_MAP(CInstallationAdvancedDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_INSTALL_NO_REQUIED_FREE_SPACE, &CInstallationAdvancedDlg::OnBnClickedCheckAllowInstallNoRequiedFreeSpace)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_IF_FAILED_PREREQUISITES, &CInstallationAdvancedDlg::OnBnClickedCheckAllowIfFailedPrerequisites)
	ON_EN_CHANGE(IDC_EDIT_UNINSTALLER_FILE_NAME, &CInstallationAdvancedDlg::OnEnChangeEditUninstallerFileName)
END_MESSAGE_MAP()


// CInstallationAdvancedDlg message handlers


void CInstallationAdvancedDlg::OnBnClickedCheckAutoLaunchOnWindowsStartup()
{
	// TODO: Add your control notification handler code here
	//g_globaldata.install_sh2.advanced.chkAutoLaunch = m_ctrlCheckAuto.GetCheck();
}


void CInstallationAdvancedDlg::OnCbnSelchangeComboModeSelectDestinationFolder()
{
	m_nModeSelectDestinationFolder = m_ctrlComboSelFolder.GetCurSel();
	//g_globaldata.install_sh2.advanced.nModeSelDestFolder = m_nModeSelectDestinationFolder;
}


void CInstallationAdvancedDlg::OnCbnSelchangeComboDefaultLanguageFunction()
{
	m_nDefaultLaguageFunction = m_ctrlComboDefaultLang.GetCurSel();
	//g_globaldata.install_sh2.advanced.nDefaultLanguageFuncMode = m_nDefaultLaguageFunction;
}


void CInstallationAdvancedDlg::OnBnClickedCheckAllowInstallNoRequiedFreeSpace()
{
	g_globaldata.install_sh2.advanced.chkAllowInstallNoFreeSpace = m_ctrlCheckAllowInstallNoFreeSpace.GetCheck();
}


void CInstallationAdvancedDlg::OnCbnSelchangeComboUninstallerFileName()
{
	m_nUninstallerFileName = m_ctrlComboUninstFileName.GetCurSel();
	//g_globaldata.install_sh2.advanced.nUninstFileMode = m_nUninstallerFileName;
}


BOOL CInstallationAdvancedDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	/*wsprintf(m_wszAlterInstDir[0], L"#PROGRAM_FILES#\\#APPLICATION_NAME#");
	wsprintf(m_wszAlterInstDir[1], L"#PROGRAM_FILES#\\#COMPANY_NAME#\\#APPLICATION_NAME#");
	wsprintf(m_wszAlterInstDir[2], L"#APP_DATA#\\#APPLICATION_NAME#");
	wsprintf(m_wszAlterInstDir[3], L"#APP_DATA#\\#COMPANY_NAME#\\#APPLICATION_NAME#");
	wsprintf(m_wszAlterInstDir[4], L"#COMMON_APP_DATA#\\#APPLICATION_NAME#");
	wsprintf(m_wszAlterInstDir[5], L"#COMMON_APP_DATA#\\#COMPANY_NAME#\\#APPLICATION_NAME#");*/

	/*wsprintf(m_wszExtractParam[0], L"-o\"#INSTALL_DIR#\" -aoa");
	wsprintf(m_wszExtractParam[1], L"s-o\"#INSTALL_DIR#\" -aoa");*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



BOOL CInstallationAdvancedDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	/*m_strAlterInstDir = g_globaldata.install_sh2.advanced.wszAlternativeInstDir;
	m_strExtractParam = g_globaldata.install_sh2.advanced.wszExtractParams;
	UpdateData(FALSE);

	m_nModeSelectDestinationFolder = g_globaldata.install_sh2.advanced.nModeSelDestFolder;
	m_nUninstallerFileName = g_globaldata.install_sh2.advanced.nUninstFileMode;
	m_nDefaultLaguageFunction = g_globaldata.install_sh2.advanced.nDefaultLanguageFuncMode;
	
	
	m_ctrlCheckAuto.SetCheck(g_globaldata.install_sh2.advanced.chkAutoLaunch);
	m_ctrlCheckAllowInstall.SetCheck(g_globaldata.install_sh2.advanced.chkAllowInstallNoFreeSpace);
	m_ctrlComboSelFolder.SetCurSel(m_nModeSelectDestinationFolder);
	m_ctrlComboUninstFileName.SetCurSel(m_nUninstallerFileName);
	m_ctrlComboDefaultLang.SetCurSel(m_nDefaultLaguageFunction);
	m_ctrlComboAlterInstDir.SetWindowText(g_globaldata.install_sh2.advanced.wszAlternativeInstDir);
	m_ctrlComboExtractParam.SetWindowText(g_globaldata.install_sh2.advanced.wszExtractParams);*/

	SetDlgItemText(IDC_EDIT_UNINSTALLER_FILE_NAME, g_globaldata.install_sh2.advanced.wszUninstallerFileName);
	m_ctrlCheckAllowInstallNoFreeSpace.SetCheck(g_globaldata.install_sh2.advanced.chkAllowInstallNoFreeSpace);
	m_ctrlCheckAllowInstallFailedPrerequisites.SetCheck(g_globaldata.install_sh2.advanced.chkAllowInstallFailedPrerequisites);

	return CPropertyPage::OnSetActive();
}


void CInstallationAdvancedDlg::OnCbnSelchangeComboAlternativeInstallationDirectory()
{
	// TODO: Add your control notification handler code here
	m_strAlterInstDir = m_wszAlterInstDir[m_ctrlComboAlterInstDir.GetCurSel()];
	//wsprintf(g_globaldata.install_sh2.advanced.wszAlternativeInstDir, m_strAlterInstDir);
}


void CInstallationAdvancedDlg::OnCbnSelchangeComboExtractParameterForDataFile()
{
	// TODO: Add your control notification handler code here
	m_strExtractParam = m_wszExtractParam[m_ctrlComboExtractParam.GetCurSel()];
	//wsprintf(g_globaldata.install_sh2.advanced.wszExtractParams, m_strExtractParam);
}

void CInstallationAdvancedDlg::OnEditchangeComboAlternativeInstallationDirectory()
{
	m_ctrlComboAlterInstDir.GetWindowTextW(m_strAlterInstDir);
	//wcscpy(g_globaldata.install_sh2.advanced.wszAlternativeInstDir, m_strAlterInstDir);
}

void CInstallationAdvancedDlg::OnEditchangeComboExtractParameterForDataFile()
{
	m_ctrlComboExtractParam.GetWindowText(m_strExtractParam);
	//wsprintf(g_globaldata.install_sh2.advanced.wszExtractParams, m_strExtractParam.GetBuffer());
}


void CInstallationAdvancedDlg::OnBnClickedCheckAllowIfFailedPrerequisites()
{
	// TODO: Add your control notification handler code here
	g_globaldata.install_sh2.advanced.chkAllowInstallFailedPrerequisites = m_ctrlCheckAllowInstallFailedPrerequisites.GetCheck();
}


void CInstallationAdvancedDlg::OnEnChangeEditUninstallerFileName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_EDIT_UNINSTALLER_FILE_NAME, str);
	wsprintf(g_globaldata.install_sh2.advanced.wszUninstallerFileName, str.GetBuffer());

	CWnd* pParentWnd = GetParent();
	if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_FILESANDFOLDERS_REFRESH);
}
