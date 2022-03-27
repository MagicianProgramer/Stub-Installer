// InstallerWizard.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallerWizardDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"


// CInstallerWizard dialog

IMPLEMENT_DYNAMIC(CInstallerWizardDlg, CDialogEx)

CInstallerWizardDlg::CInstallerWizardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstallerWizardDlg::IDD, pParent)
{

}

CInstallerWizardDlg::~CInstallerWizardDlg()
{
}

void CInstallerWizardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstallerWizardDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP, &CInstallerWizardDlg::OnBnClickedCheckWizardDonotShowOnStartup)
	ON_BN_CLICKED(IDC_BTN_WIZARD_NEXT, &CInstallerWizardDlg::OnBnClickedBtnWizardNext)
	ON_BN_CLICKED(IDC_BTN_WIZARD_CLOSE, &CInstallerWizardDlg::OnBnClickedBtnWizardClose)
END_MESSAGE_MAP()


// CInstallerWizard message handlers


BOOL CInstallerWizardDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here
	CheckDlgButton(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP, g_generalOptions.chkUsingWizard);
	//create sheet//
	m_sheet.Create(this, WS_VISIBLE|WS_CHILD);
	m_sheet.MoveWindow(180, -7, 550, 320);
	m_sheet.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInstallerWizardDlg::OnBnClickedCheckWizardDonotShowOnStartup()
{
	// TODO: Add your control notification handler code here
	g_generalOptions.chkUsingWizard = IsDlgButtonChecked(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP);
}


void CInstallerWizardDlg::OnBnClickedBtnWizardNext()
{
	// TODO: Add your control notification handler code here
	if(!m_sheet.NextPage())
	{
		m_sheet.GetAllInformations();
		PostMessage(WM_CLOSE);
	}

	if (m_sheet.m_nCurPage == WIZARD_PAGE_COUNT - 1)
		SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Finish");
	else
		SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Next");
}


void CInstallerWizardDlg::OnBnClickedBtnWizardClose()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE);
}

LRESULT CInstallerWizardDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == UM_CURRENT_PAGE)
	{
		if (m_sheet.m_nCurPage == WIZARD_PAGE_COUNT - 1)
			SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Finish");
		else
			SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Next");
		return 0L;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
