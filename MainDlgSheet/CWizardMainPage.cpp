// CWizardMainPage.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "CWizardMainPage.h"

#include "../common/common.h"

// CWizardMainPage

IMPLEMENT_DYNAMIC(CWizardMainPage, CPropertyPage)

CWizardMainPage::CWizardMainPage()
	: CPropertyPage(CWizardMainPage::IDD)
{

}

CWizardMainPage::~CWizardMainPage()
{
}

void CWizardMainPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWizardMainPage, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP, &CWizardMainPage::OnBnClickedCheckWizardDonotShowOnStartup)
	ON_BN_CLICKED(IDC_BTN_WIZARD_NEXT, &CWizardMainPage::OnBnClickedBtnWizardNext)
	ON_BN_CLICKED(IDC_BTN_WIZARD_CLOSE, &CWizardMainPage::OnBnClickedBtnWizardClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CInstallerWizard message handlers


BOOL CWizardMainPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

								// TODO:  Add extra initialization here
	CheckDlgButton(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP, g_generalOptions.chkUsingWizard);
	//create sheet//
	m_sheet.Create(this, WS_VISIBLE | WS_CHILD);
	m_sheet.MoveWindow(180, -7, 550, 330);
	m_sheet.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CWizardMainPage::OnBnClickedCheckWizardDonotShowOnStartup()
{
	// TODO: Add your control notification handler code here
	g_generalOptions.chkUsingWizard = IsDlgButtonChecked(IDC_CHECK_WIZARD_DONOT_SHOW_ON_STARTUP);	
}


void CWizardMainPage::OnBnClickedBtnWizardNext()
{
	// TODO: Add your control notification handler code here
	if (!m_sheet.NextPage())
	{
		m_sheet.GetAllInformations();
		SaveProjectOptionsIntoRegistry();
		GetParent()->GetParent()->PostMessage(WM_CLOSE);
	}

	if (m_sheet.m_nCurPage == WIZARD_PAGE_COUNT - 1)
		SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Finish");
	else
		SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Next");
}


void CWizardMainPage::OnBnClickedBtnWizardClose()
{
	// TODO: Add your control notification handler code here
	GetParent()->GetParent()->PostMessage(WM_CLOSE);
}

LRESULT CWizardMainPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

	return CPropertyPage::WindowProc(message, wParam, lParam);
}




void CWizardMainPage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CPropertyPage::OnPaint() for painting messages

	//draw wizard logo image//
	/*CDC* pDC = GetDlgItem(IDC_STATIC_WIZARD_BACK)->GetDC();
	::SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);

	CDC dc_background;
	CBitmap bmp_background, *p_bmp_background;
	BITMAP bmsize_background;
	dc_background.CreateCompatibleDC(&dc);
	//
	bmp_background.LoadBitmap(IDB_BITMAP_BACK);
	bmp_background.GetBitmap(&bmsize_background);

	//
	p_bmp_background = dc_background.SelectObject(&bmp_background);

	CRect re;
	GetDlgItem(IDC_STATIC_WIZARD_BACK)->GetClientRect(&re);
	//draw//
	dc.StretchBlt(re.left, re.top, re.right, re.bottom, &dc_background, 0, 0, bmsize_background.bmWidth, bmsize_background.bmHeight, SRCCOPY);

	pDC->SelectObject(p_bmp_background);
	dc_background.DeleteDC();
	bmp_background.DeleteObject();*/
}
