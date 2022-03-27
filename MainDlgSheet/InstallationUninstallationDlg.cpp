// InstallationUninstallationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationUninstallationDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

// CInstallationUninstallationDlg dialog

IMPLEMENT_DYNAMIC(CInstallationUninstallationDlg, CPropertyPage)

CInstallationUninstallationDlg::CInstallationUninstallationDlg()
	: CPropertyPage(CInstallationUninstallationDlg::IDD)
{
	m_bRefresh = TRUE;
}

CInstallationUninstallationDlg::~CInstallationUninstallationDlg()
{
}

void CInstallationUninstallationDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_UNINSTALLER, m_ctrlCheckIncludeUninst);
	DDX_Control(pDX, IDC_CHECK_ADD_TO_CONTROLPANEL, m_ctrlCheckAddToControl);
	DDX_Control(pDX, IDC_CHECK_SILENT_MODE, m_ctrlCheckSilentMode);
	DDX_Control(pDX, IDC_CHECK_COMPLETE_UNINSTALLATION, m_ctrlCheckCompleteUninst);
	DDX_Control(pDX, IDC_CHECK_UNINST_RESTART_COMPUTER, m_ctrlCheckRestartCom);
	DDX_Control(pDX, IDC_CHECK_CLOSE_MAIN_EXCUTABLE, m_ctrlCheckCloseMainExe);
	DDX_Control(pDX, IDC_CHECK_CLOSE_RUNNING_APPLICATION, m_ctrlCheckCloseRunningApp);
	DDX_Control(pDX, IDC_CHECK_PROMPT_TO_REMOVE_PRODUCT_SETTING_DATA, m_ctrlCheckPromptToRemove);
	DDX_Control(pDX, IDC_CHECK_PROMPT_TO_VISIT_PRODUCT_FEEDBACK_PAGE, m_ctrlCheckPromptToVisit);
}


BEGIN_MESSAGE_MAP(CInstallationUninstallationDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_UNINSTALLER, &CInstallationUninstallationDlg::OnBnClickedCheckIncludeUninstaller)
	ON_BN_CLICKED(IDC_CHECK_ADD_TO_CONTROLPANEL, &CInstallationUninstallationDlg::OnBnClickedCheckAddToControlpanel)
	ON_BN_CLICKED(IDC_CHECK_SILENT_MODE, &CInstallationUninstallationDlg::OnBnClickedCheckSilentMode)
	ON_BN_CLICKED(IDC_CHECK_COMPLETE_UNINSTALLATION, &CInstallationUninstallationDlg::OnBnClickedCheckCompleteUninstallation)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_MAIN_EXCUTABLE, &CInstallationUninstallationDlg::OnBnClickedCheckCloseMainExcutable)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_RUNNING_APPLICATION, &CInstallationUninstallationDlg::OnBnClickedCheckCloseRunningApplication)
	ON_EN_CHANGE(IDC_EDIT_CRA_FILE, &CInstallationUninstallationDlg::OnEnChangeEditCraFile)
	ON_EN_CHANGE(IDC_EDIT_CRA_NAME, &CInstallationUninstallationDlg::OnEnChangeEditCraName)
	ON_BN_CLICKED(IDC_CHECK_PROMPT_TO_REMOVE_PRODUCT_SETTING_DATA, &CInstallationUninstallationDlg::OnBnClickedCheckPromptToRemoveProductSettingData)
	ON_EN_CHANGE(IDC_EDIT_COMMAND, &CInstallationUninstallationDlg::OnEnChangeEditCommand)
	ON_BN_CLICKED(IDC_CHECK_PROMPT_TO_VISIT_PRODUCT_FEEDBACK_PAGE, &CInstallationUninstallationDlg::OnBnClickedCheckPromptToVisitProductFeedbackPage)
	ON_EN_CHANGE(IDC_EDIT_URL, &CInstallationUninstallationDlg::OnEnChangeEditUrl)
	ON_BN_CLICKED(IDC_CHECK_UNINST_RESTART_COMPUTER, &CInstallationUninstallationDlg::OnBnClickedCheckUninstRestartComputer)
END_MESSAGE_MAP()


// CInstallationUninstallationDlg message handlers


void CInstallationUninstallationDlg::OnBnClickedCheckIncludeUninstaller()
{
	m_bIncludeUninstaller = m_ctrlCheckIncludeUninst.GetCheck();
	g_globaldata.install_sh2.uninst.chkIncludeUninstaller = m_bIncludeUninstaller;

	SetComponent();

	CWnd* pParentWnd = GetParent();
	if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_FILESANDFOLDERS_REFRESH);
}


void CInstallationUninstallationDlg::OnBnClickedCheckAddToControlpanel()
{
	m_bAddToControl = m_ctrlCheckAddToControl.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs = m_bAddToControl;
}


void CInstallationUninstallationDlg::OnBnClickedCheckSilentMode()
{
	m_bSilentMode = m_ctrlCheckSilentMode.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsSilentMode = m_bSilentMode;
}


void CInstallationUninstallationDlg::OnBnClickedCheckCompleteUninstallation()
{
	m_bCompleteUninstallation = m_ctrlCheckCompleteUninst.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsCompleteUninstalltion = m_bCompleteUninstallation;
}




void CInstallationUninstallationDlg::OnBnClickedCheckCloseMainExcutable()
{
	m_bCloseMainExcutable = m_ctrlCheckCloseMainExe.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsCloseMainExecutable = m_bCloseMainExcutable;
}


void CInstallationUninstallationDlg::OnBnClickedCheckCloseRunningApplication()
{
	m_bCloseRunningApplication = m_ctrlCheckCloseRunningApp.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsCloseRunningApp = m_bCloseRunningApplication;

	GetDlgItem(IDC_EDIT_CRA_FILE)->EnableWindow(m_bCloseRunningApplication);
	GetDlgItem(IDC_EDIT_CRA_NAME)->EnableWindow(m_bCloseRunningApplication);
}


void CInstallationUninstallationDlg::OnEnChangeEditCraFile()
{
	GetDlgItemText(IDC_EDIT_CRA_FILE, m_strCRAFile);
	wcscpy(g_globaldata.install_sh2.uninst.wszOptionsCloseRunningAppFile, m_strCRAFile);
}


void CInstallationUninstallationDlg::OnEnChangeEditCraName()
{
	GetDlgItemText(IDC_EDIT_CRA_NAME, m_strCRAName);
	wcscpy(g_globaldata.install_sh2.uninst.wszOptionsCloseRunningAppName, m_strCRAName);
}


void CInstallationUninstallationDlg::OnBnClickedCheckPromptToRemoveProductSettingData()
{
	m_bPromptToRemoveSettingData = m_ctrlCheckPromptToRemove.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsPrompt2Remove = m_bPromptToRemoveSettingData;
	GetDlgItem(IDC_EDIT_COMMAND)->EnableWindow(m_bPromptToRemoveSettingData);
}


void CInstallationUninstallationDlg::OnEnChangeEditCommand()
{
	GetDlgItemText(IDC_EDIT_COMMAND, m_strCommand);
	wcscpy(g_globaldata.install_sh2.uninst.wszOptionsPrompt2RemoveCommand, m_strCommand);
}


void CInstallationUninstallationDlg::OnBnClickedCheckPromptToVisitProductFeedbackPage()
{
	m_bPromptToVisitFeadbackPage = m_ctrlCheckPromptToVisit.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsPrompt2Visit = m_bPromptToVisitFeadbackPage;
	GetDlgItem(IDC_EDIT_URL)->EnableWindow(m_bPromptToVisitFeadbackPage);
}


void CInstallationUninstallationDlg::OnEnChangeEditUrl()
{
	GetDlgItemText(IDC_EDIT_URL, m_strURL);
	wcscpy(g_globaldata.install_sh2.uninst.wszOptionsPrompt2VisitUrl, m_strURL);
}


BOOL CInstallationUninstallationDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bRefresh)
	{
		m_bIncludeUninstaller = g_globaldata.install_sh2.uninst.chkIncludeUninstaller;
		m_bAddToControl = g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs;
		m_bSilentMode = g_globaldata.install_sh2.uninst.chkOptionsSilentMode;
		m_bCompleteUninstallation = g_globaldata.install_sh2.uninst.chkOptionsCompleteUninstalltion;
		m_bRestartComputer = g_globaldata.install_sh2.uninst.chkOptionsRestartCom;
		m_bCloseMainExcutable = g_globaldata.install_sh2.uninst.chkOptionsCloseMainExecutable;
		m_bCloseRunningApplication = g_globaldata.install_sh2.uninst.chkOptionsCloseRunningApp;
		m_bPromptToRemoveSettingData = g_globaldata.install_sh2.uninst.chkOptionsPrompt2Remove;
		m_bPromptToVisitFeadbackPage = g_globaldata.install_sh2.uninst.chkOptionsPrompt2Visit;

		m_strCommand = g_globaldata.install_sh2.uninst.wszOptionsPrompt2RemoveCommand;
		m_strCRAFile = g_globaldata.install_sh2.uninst.wszOptionsCloseRunningAppFile;
		m_strCRAName = g_globaldata.install_sh2.uninst.wszOptionsCloseRunningAppName;
		m_strURL = g_globaldata.install_sh2.uninst.wszOptionsPrompt2VisitUrl;

		SetComponent();
	}

	return CPropertyPage::OnSetActive();
}


void CInstallationUninstallationDlg::SetComponent()
{
	if (!m_bIncludeUninstaller)
	{
		g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsSilentMode = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsCompleteUninstalltion = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsRestartCom = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsCloseMainExecutable = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsCloseRunningApp = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsPrompt2Remove = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsPrompt2Visit = FALSE;
	}
	else
	{
		g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs = TRUE;
		g_globaldata.install_sh2.uninst.chkOptionsSilentMode = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsCompleteUninstalltion = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsRestartCom = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsCloseMainExecutable = TRUE;
		g_globaldata.install_sh2.uninst.chkOptionsCloseRunningApp = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsPrompt2Remove = FALSE;
		g_globaldata.install_sh2.uninst.chkOptionsPrompt2Visit = FALSE;
	}

	m_bAddToControl = g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs;
	m_bSilentMode = g_globaldata.install_sh2.uninst.chkOptionsSilentMode;
	m_bCompleteUninstallation = g_globaldata.install_sh2.uninst.chkOptionsCompleteUninstalltion;
	m_bRestartComputer = g_globaldata.install_sh2.uninst.chkOptionsRestartCom;
	m_bCloseMainExcutable = g_globaldata.install_sh2.uninst.chkOptionsCloseMainExecutable;
	m_bCloseRunningApplication = g_globaldata.install_sh2.uninst.chkOptionsCloseRunningApp;
	m_bPromptToRemoveSettingData = g_globaldata.install_sh2.uninst.chkOptionsPrompt2Remove;
	m_bPromptToVisitFeadbackPage = g_globaldata.install_sh2.uninst.chkOptionsPrompt2Visit;

	m_ctrlCheckIncludeUninst.SetCheck(m_bIncludeUninstaller);
	m_ctrlCheckAddToControl.SetCheck(m_bAddToControl);
	m_ctrlCheckSilentMode.SetCheck(m_bSilentMode);
	m_ctrlCheckCompleteUninst.SetCheck(m_bCompleteUninstallation);
	m_ctrlCheckRestartCom.SetCheck(m_bRestartComputer);
	m_ctrlCheckCloseMainExe.SetCheck(m_bCloseMainExcutable);
	m_ctrlCheckCloseRunningApp.SetCheck(m_bCloseRunningApplication);
	m_ctrlCheckPromptToRemove.SetCheck(m_bPromptToRemoveSettingData);
	m_ctrlCheckPromptToVisit.SetCheck(m_bPromptToVisitFeadbackPage);

	SetDlgItemText(IDC_EDIT_CRA_FILE, m_strCRAFile);
	SetDlgItemText(IDC_EDIT_CRA_NAME, m_strCRAName);
	SetDlgItemText(IDC_EDIT_COMMAND, m_strCommand);
	SetDlgItemText(IDC_EDIT_URL, m_strURL);

	m_ctrlCheckAddToControl.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckSilentMode.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckCompleteUninst.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckRestartCom.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckCloseMainExe.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckCloseRunningApp.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckPromptToRemove.EnableWindow(m_bIncludeUninstaller);
	m_ctrlCheckPromptToVisit.EnableWindow(m_bIncludeUninstaller);

	GetDlgItem(IDC_EDIT_CRA_FILE)->EnableWindow(m_bCloseRunningApplication);
	GetDlgItem(IDC_EDIT_CRA_NAME)->EnableWindow(m_bCloseRunningApplication);
	GetDlgItem(IDC_EDIT_COMMAND)->EnableWindow(m_bPromptToRemoveSettingData);
	GetDlgItem(IDC_EDIT_URL)->EnableWindow(m_bPromptToVisitFeadbackPage);

	if (m_bRefresh)		m_bRefresh = FALSE;
}

void CInstallationUninstallationDlg::OnBnClickedCheckUninstRestartComputer()
{
	// TODO: Add your control notification handler code here
	m_bRestartComputer = m_ctrlCheckRestartCom.GetCheck();
	g_globaldata.install_sh2.uninst.chkOptionsRestartCom = m_bRestartComputer;
}
