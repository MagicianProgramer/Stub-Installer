// CWizardMainPage.cpp : implementation file
//

#include "stdafx.h"
#include "CWizardUninstWelcomePage.h"

// CWizardMainPage

IMPLEMENT_DYNAMIC(CWizardUninstWelcomePage, CPropertyPage)

CWizardUninstWelcomePage::CWizardUninstWelcomePage()
	: CPropertyPage(CWizardUninstWelcomePage::IDD)
{

}

CWizardUninstWelcomePage::~CWizardUninstWelcomePage()
{
}

void CWizardUninstWelcomePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWizardUninstWelcomePage, CPropertyPage)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_COMPANY, &CWizardUninstWelcomePage::OnNMClickSyslinkCompany)
END_MESSAGE_MAP()

// CInstallerWizard message handlers


BOOL CWizardUninstWelcomePage::OnInitDialog()
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

	GetDlgItem(IDC_STATIC_WELCOME_TITLE)->SetFont(&m_Font);
	SetDlgItemText(IDC_STATIC_WELCOME_TITLE, L"Uninstallation");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CWizardUninstWelcomePage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CPropertyPage::WindowProc(message, wParam, lParam);
}


void CWizardUninstWelcomePage::SetNoticeText(CString strText, CString strCompanyURL)
{
	SetDlgItemText(IDC_STATIC_NOTICE_WELCOME, strText);
	m_strCompanyURL = strCompanyURL;
}

void CWizardUninstWelcomePage::OnNMClickSyslinkCompany(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"open", m_strCompanyURL, NULL, NULL, SW_SHOW);

	*pResult = 0;
}
