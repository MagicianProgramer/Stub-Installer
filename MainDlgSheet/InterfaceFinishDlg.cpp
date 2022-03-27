// InterfaceFinishDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceFinishDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

#include "InterfaceSelectFileFolderDlg.h"
// CInterfaceFinishDlg dialog

IMPLEMENT_DYNAMIC(CInterfaceFinishDlg, CPropertyPage)

CInterfaceFinishDlg::CInterfaceFinishDlg()
	: CPropertyPage(CInterfaceFinishDlg::IDD)
{
	m_bRefresh = TRUE;
}

CInterfaceFinishDlg::~CInterfaceFinishDlg()
{
}

void CInterfaceFinishDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LAUNCH_APPLICATION, m_ctrlCheckLaunchApp);
	DDX_Control(pDX, IDC_CHECK_LAUNCH_DEFAULT, m_ctrlCheckLaunchDef);
	DDX_Control(pDX, IDC_CHECK_LAUNCH_RUN, m_ctrlCheckLaunchRun);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_ATION, m_ctrlCheckCustom);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_DEFAULT, m_ctrlCheckCustomDef);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_RUN, m_ctrlCheckCustomRun);
	DDX_Control(pDX, IDC_CHECK_RESTART_COMPUTER2, m_ctrlCheckRestart);
	DDX_Control(pDX, IDC_EDIT_LAUNCH_APP_FILE, m_FinishLaunchAppFileEditCtrl);
	DDX_Control(pDX, IDC_EDIT_CUSTOM_ACTION_COMMAND, m_FinishCustomActionCommandEditCtrl);
}


BEGIN_MESSAGE_MAP(CInterfaceFinishDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_LAUNCH_APPLICATION, &CInterfaceFinishDlg::OnBnClickedCheckLaunchApplication)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CInterfaceFinishDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_CHECK_LAUNCH_DEFAULT, &CInterfaceFinishDlg::OnBnClickedCheckLaunchDefault)
	ON_BN_CLICKED(IDC_CHECK_LAUNCH_RUN, &CInterfaceFinishDlg::OnBnClickedCheckLaunchRun)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_ATION, &CInterfaceFinishDlg::OnBnClickedCheckCustomAtion)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_GUID3, &CInterfaceFinishDlg::OnEnChangeEditProductGuid3)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_GUID, &CInterfaceFinishDlg::OnEnChangeEditProductGuid)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND, &CInterfaceFinishDlg::OnBnClickedButtonCommand)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_DEFAULT, &CInterfaceFinishDlg::OnBnClickedCheckCustomDefault)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_RUN, &CInterfaceFinishDlg::OnBnClickedCheckCustomRun)
	ON_BN_CLICKED(IDC_CHECK_RESTART_COMPUTER2, &CInterfaceFinishDlg::OnBnClickedCheckRestartComputer2)
	ON_EN_CHANGE(IDC_EDIT_LAUNCH_APP_FILE, &CInterfaceFinishDlg::OnEnChangeEditLaunchAppFile)
	ON_EN_CHANGE(IDC_EDIT_CUSTOM_ACTION_COMMAND, &CInterfaceFinishDlg::OnEnChangeEditCustomActionCommand)
END_MESSAGE_MAP()


// CInterfaceFinishDlg message handlers


void CInterfaceFinishDlg::OnBnClickedCheckLaunchApplication()
{
	m_bLaunchApp = m_ctrlCheckLaunchApp.GetCheck();
	g_globaldata.interface_sh3.finish.chkLaunchApp = m_bLaunchApp;

	GetDlgItem(IDC_EDIT_LAUNCH_APP_FILE)->EnableWindow(m_bLaunchApp);
	m_ctrlCheckLaunchDef.EnableWindow(m_bLaunchApp);
	m_ctrlCheckLaunchRun.EnableWindow(m_bLaunchApp);
	
	if (m_bLaunchApp)
	{
		m_ctrlCheckLaunchDef.SetCheck(BST_CHECKED);
		g_globaldata.interface_sh3.finish.chkLaunchAppCheckedByDefault = 1;
		m_bLaunchDefault = 1;
	}
}


void CInterfaceFinishDlg::OnBnClickedButtonFile()
{
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		m_strLaunchFile = strPathName;
		wcscpy(g_globaldata.interface_sh3.finish.wszLaunchAppFile, m_strLaunchFile);
		//UpdateData(FALSE);
		SetDlgItemText(IDC_EDIT_LAUNCH_APP_FILE, m_strLaunchFile);
	}
}


void CInterfaceFinishDlg::OnBnClickedCheckLaunchDefault()
{
	m_bLaunchDefault = m_ctrlCheckLaunchDef.GetCheck();
	g_globaldata.interface_sh3.finish.chkLaunchAppCheckedByDefault = m_bLaunchDefault;
}


void CInterfaceFinishDlg::OnBnClickedCheckLaunchRun()
{
	m_bLauchRunAsAdmin = m_ctrlCheckLaunchRun.GetCheck();
	g_globaldata.interface_sh3.finish.chkLaunchAppRunAsAdmin = m_bLauchRunAsAdmin;
}


void CInterfaceFinishDlg::OnBnClickedCheckCustomAtion()
{
	m_bCustomAction = m_ctrlCheckCustom.GetCheck();
	g_globaldata.interface_sh3.finish.chkCustomAction = m_bCustomAction;

	GetDlgItem(IDC_EDIT_PRODUCT_GUID3)->EnableWindow(m_bCustomAction);
	GetDlgItem(IDC_EDIT_CUSTOM_ACTION_COMMAND)->EnableWindow(m_bCustomAction);
	m_ctrlCheckCustomDef.EnableWindow(m_bCustomAction);
	m_ctrlCheckCustomRun.EnableWindow(m_bCustomAction);

	if (m_bCustomAction)
	{
		m_ctrlCheckCustomDef.SetCheck(BST_CHECKED);
		g_globaldata.interface_sh3.finish.chkCustomActionCheckedByDefault = 1;
		m_bCustomDefault = 1;
	}
	
}


void CInterfaceFinishDlg::OnEnChangeEditProductGuid3()
{
	GetDlgItemText(IDC_EDIT_PRODUCT_GUID3, m_strCustomName);
	wcscpy(g_globaldata.interface_sh3.finish.wszCustomActionName, m_strCustomName);
}


void CInterfaceFinishDlg::OnEnChangeEditProductGuid()
{
	GetDlgItemText(IDC_EDIT_CUSTOM_ACTION_COMMAND, m_strCustomCommand);
	wcscpy(g_globaldata.interface_sh3.finish.wszCustomActionCommand, m_strCustomCommand);
}


void CInterfaceFinishDlg::OnBnClickedButtonCommand()
{
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		m_strCustomCommand = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_CUSTOM_ACTION_COMMAND, m_strCustomCommand);
		wsprintf(g_globaldata.interface_sh3.finish.wszCustomActionCommand, m_strCustomCommand.GetBuffer());
	}
}


void CInterfaceFinishDlg::OnBnClickedCheckCustomDefault()
{
	m_bCustomDefault = m_ctrlCheckCustomDef.GetCheck();
	g_globaldata.interface_sh3.finish.chkCustomActionCheckedByDefault = m_bCustomDefault;
}


void CInterfaceFinishDlg::OnBnClickedCheckCustomRun()
{
	m_bCustomAdmin = m_ctrlCheckCustomRun.GetCheck();
	g_globaldata.interface_sh3.finish.chkCustomActionRunAsAdmin = m_bCustomAdmin;
}


void CInterfaceFinishDlg::OnBnClickedCheckRestartComputer2()
{
	m_bRestartComputer = m_ctrlCheckRestart.GetCheck();
	g_globaldata.interface_sh3.finish.chkRestartComputer = m_bRestartComputer;

	m_ctrlCheckLaunchApp.SetCheck(FALSE);
	m_ctrlCheckCustom.SetCheck(FALSE);

	if(m_bRestartComputer == TRUE)
	{
		m_ctrlCheckLaunchApp.EnableWindow(FALSE);
		m_ctrlCheckCustom.EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_LAUNCH_APP_FILE)->EnableWindow(FALSE);
		m_ctrlCheckLaunchDef.EnableWindow(FALSE);
		m_ctrlCheckLaunchRun.EnableWindow(FALSE);
		
		GetDlgItem(IDC_EDIT_PRODUCT_GUID3)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CUSTOM_ACTION_COMMAND)->EnableWindow(FALSE);
		m_ctrlCheckCustomDef.EnableWindow(FALSE);
		m_ctrlCheckCustomRun.EnableWindow(FALSE);
	}
	else
	{
		m_ctrlCheckLaunchApp.EnableWindow(TRUE);
		m_ctrlCheckCustom.EnableWindow(TRUE);
	}

}

void CInterfaceFinishDlg::OnEnChangeEditLaunchAppFile()
{
	//UpdateData(TRUE);
	GetDlgItemText(IDC_EDIT_LAUNCH_APP_FILE, m_strLaunchFile);
	wcscpy(g_globaldata.interface_sh3.finish.wszLaunchAppFile, m_strLaunchFile);
}


//BOOL CInterfaceFinishDlg::OnInitDialog()
//{
//	CPropertyPage::OnInitDialog();
//
//	// TODO:  Add extra initialization here
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// EXCEPTION: OCX Property Pages should return FALSE
//}


BOOL CInterfaceFinishDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bRefresh)
	{
		m_bLaunchApp = g_globaldata.interface_sh3.finish.chkLaunchApp;
		m_strLaunchFile = g_globaldata.interface_sh3.finish.wszLaunchAppFile;
		m_bLaunchDefault = g_globaldata.interface_sh3.finish.chkLaunchAppCheckedByDefault;
		m_bLauchRunAsAdmin = g_globaldata.interface_sh3.finish.chkLaunchAppRunAsAdmin;
		m_bCustomAction = g_globaldata.interface_sh3.finish.chkCustomAction;
		m_strCustomName = g_globaldata.interface_sh3.finish.wszCustomActionName;
		m_strCustomCommand = g_globaldata.interface_sh3.finish.wszCustomActionCommand;
		m_bCustomDefault = g_globaldata.interface_sh3.finish.chkCustomActionCheckedByDefault;
		m_bCustomAdmin = g_globaldata.interface_sh3.finish.chkCustomActionRunAsAdmin;
		m_bRestartComputer = g_globaldata.interface_sh3.finish.chkRestartComputer;

		m_ctrlCheckLaunchApp.SetCheck(m_bLaunchApp);

		SetDlgItemText(IDC_EDIT_LAUNCH_APP_FILE, m_strLaunchFile);
		m_ctrlCheckLaunchDef.SetCheck(m_bLaunchDefault);
		m_ctrlCheckLaunchRun.SetCheck(m_bLauchRunAsAdmin);
		m_ctrlCheckCustom.SetCheck(m_bCustomAction);
		m_ctrlCheckCustomDef.SetCheck(m_bCustomDefault);
		m_ctrlCheckCustomRun.SetCheck(m_bCustomAdmin);

		SetDlgItemText(IDC_EDIT_PRODUCT_GUID3, m_strCustomName);
		SetDlgItemText(IDC_EDIT_CUSTOM_ACTION_COMMAND, m_strCustomCommand);
		m_ctrlCheckRestart.SetCheck(m_bRestartComputer);

		GetDlgItem(IDC_EDIT_LAUNCH_APP_FILE)->EnableWindow(m_bLaunchApp);
		m_ctrlCheckLaunchDef.EnableWindow(m_bLaunchApp);
		m_ctrlCheckLaunchRun.EnableWindow(m_bLaunchApp);

		GetDlgItem(IDC_EDIT_PRODUCT_GUID3)->EnableWindow(m_bCustomAction);
		GetDlgItem(IDC_EDIT_CUSTOM_ACTION_COMMAND)->EnableWindow(m_bCustomAction);
		m_ctrlCheckCustomDef.EnableWindow(m_bCustomAction);
		m_ctrlCheckCustomRun.EnableWindow(m_bCustomAction);

		m_bRefresh = FALSE;
	}

	return CPropertyPage::OnSetActive();
}

void CInterfaceFinishDlg::OnEnChangeEditCustomActionCommand()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_CUSTOM_ACTION_COMMAND, m_strCustomCommand);
	wsprintf(g_globaldata.interface_sh3.finish.wszCustomActionCommand, m_strCustomCommand.GetBuffer());
}


void CInterfaceFinishDlg::SetExecutable()
{
	g_globaldata.interface_sh3.finish.chkLaunchApp = 1;
	g_globaldata.interface_sh3.finish.chkLaunchAppCheckedByDefault = 1;
		
	if (m_ctrlCheckLaunchApp.m_hWnd)
	{
		m_ctrlCheckLaunchApp.SetCheck(BST_CHECKED);
		m_ctrlCheckLaunchDef.SetCheck(BST_CHECKED);
		SetDlgItemText(IDC_EDIT_LAUNCH_APP_FILE, g_globaldata.interface_sh3.finish.wszLaunchAppFile);
		GetDlgItem(IDC_EDIT_LAUNCH_APP_FILE)->EnableWindow(TRUE);
		m_ctrlCheckLaunchDef.EnableWindow(TRUE);
		m_ctrlCheckLaunchRun.EnableWindow(TRUE);
	}
}

BOOL CInterfaceFinishDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_FinishLaunchAppFileEditCtrl.EnableBrowseButton();
	m_FinishCustomActionCommandEditCtrl.EnableBrowseButton();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyBrowseEdit1::OnBrowse()
{
	CInterfaceSelectFileFolderDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		SetWindowText(dlg.m_strSectedFullName);
		wsprintf(g_globaldata.interface_sh3.finish.wszCustomActionCommand, dlg.m_strSectedFullName.GetBuffer());
	}	
}