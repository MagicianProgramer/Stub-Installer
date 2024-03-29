// CWizardMainPage.cpp : implementation file
//

#include "stdafx.h"
#include "CWizardUninstFinishPage.h"

// CWizardMainPage

IMPLEMENT_DYNAMIC(CWizardUninstFinishPage, CPropertyPage)

CWizardUninstFinishPage::CWizardUninstFinishPage()
	: CPropertyPage(CWizardUninstFinishPage::IDD)
{

}

CWizardUninstFinishPage::~CWizardUninstFinishPage()
{
}

void CWizardUninstFinishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWizardUninstFinishPage, CPropertyPage)
END_MESSAGE_MAP()

// CInstallerWizard message handlers


BOOL CWizardUninstFinishPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here
	//set font//
	m_Font.CreateFont(18,                            // Height
		0,                             // Width
		0,                             // Escapement
		0,                             // Orientation
		FW_BOLD,                       // Weight
		FALSE,                         // Italic
		FALSE,                          // Underline
		0,                             // StrikeOut
		DEFAULT_CHARSET,                  // CharSet
		OUT_DEFAULT_PRECIS,            // OutPrecision
		CLIP_DEFAULT_PRECIS,           // ClipPrecision
		DEFAULT_QUALITY,               // Quality
		DEFAULT_PITCH | FF_SWISS,      // PitchAndFamily
		L"Arial");                     // Facename

	GetDlgItem(IDC_STATIC_FINISH_TITLE)->SetFont(&m_Font);
	SetDlgItemText(IDC_STATIC_FINISH_TITLE, L"Uninstallation Completed!");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CWizardUninstFinishPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CPropertyPage::WindowProc(message, wParam, lParam);
}


void CWizardUninstFinishPage::SetNoticeText(CString strTxt)
{
	SetDlgItemText(IDC_STATIC_FINISH_NOTICE, strTxt);
}