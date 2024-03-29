// CWizardMainPage.cpp : implementation file
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "MainUninstPage.h"

#include "uninst.h"

// CWizardMainPage

IMPLEMENT_DYNAMIC(CMainUninstPage, CPropertyPage)

CMainUninstPage::CMainUninstPage()
	: CPropertyPage(CMainUninstPage::IDD)
{

}

CMainUninstPage::~CMainUninstPage()
{
}

void CMainUninstPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainUninstPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_WIZARD_NEXT, &CMainUninstPage::OnBnClickedBtnWizardNext)
	ON_BN_CLICKED(IDC_BTN_WIZARD_CANCEL, &CMainUninstPage::OnBnClickedBtnWizardCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CInstallerWizard message handlers


BOOL CMainUninstPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here
	//get params//
	GetOptions();
	GetInfoFromRegistry();

	//create property sheet//
	CRect rect;
	GetClientRect(&rect);
	rect.left = 180;
	rect.bottom = rect.bottom - 50;

	m_wzdSht.Create(this, WS_VISIBLE | WS_CHILD/*|WS_BORDER*/);
	m_wzdSht.GetTabControl()->ShowWindow(SW_HIDE);

	m_wzdSht.MoveWindow(&rect);
	m_wzdSht.ShowWindow(SW_SHOW);

	//set page text//
	CString strTxt;
	strTxt.Format(L"This will uninstall %s %s (%s) on your computer.\n", g_strProductName, g_strProductVersion, g_strInstallPath);
	strTxt.Append(L"Click Next to continue, or Cancel to exit Uninstaller.");
	m_wzdSht.m_welcomePage.SetNoticeText(strTxt, (LPCTSTR)g_UninstOption.wszPrompt2VisitUrl);	

	if (g_UninstOption.chkOptionsSilentMode)
	{
		PostMessageW(UM_NEXT_PAGE);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CMainUninstPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == UM_NEXT_PAGE)
	{
		OnBnClickedBtnWizardNext();
		return 0L;
	}

	return CPropertyPage::WindowProc(message, wParam, lParam);
}




void CMainUninstPage::OnBnClickedBtnWizardNext()
{
	// TODO: Add your control notification handler code here
	INT pgidx = m_wzdSht.SetNextPage();

	//set component//
	if (pgidx == MAX_PAGES_CNT - 1)
	{
		SetDlgItemText(IDC_BTN_WIZARD_NEXT, L"Finish");
		GetDlgItem(IDC_BTN_WIZARD_CANCEL)->ShowWindow(SW_HIDE);

		CString strTxt;
		strTxt.Format(L"%s %s has been uninstalled on your computer.\nClick Finish to exit.", g_strProductName, g_strProductVersion);
		m_wzdSht.m_finishPage.SetNoticeText(strTxt);
	}

	if (pgidx >= MAX_PAGES_CNT)
	{
		//option//
		if (g_UninstOption.chkRestartCom)
		{
			CString strtxt;
			strtxt = L"You should restart computer after this uninstallation. Do you want to restart your computer now?";
			if (::MessageBox(this->m_hWnd, strtxt, L"warning", MB_YESNO) == IDYES)
			{
				system("shutdown /r /t 0");
			}			
		}

		if (g_UninstOption.chkOptionsPrompt2Remove)
		{
			CString strprompt(g_UninstOption.wszPrompt2RemoveCommand);
			CStringA strcmdA(strprompt);
			system(strcmdA.GetBuffer());
		}

		if (g_UninstOption.chkPrompt2Visit)
		{
			ShellExecute(NULL, NULL, g_UninstOption.wszPrompt2VisitUrl, NULL, NULL, SW_NORMAL);
		}

		CWnd* pMainWnd = AfxGetMainWnd();
		pMainWnd->PostMessageW(WM_CLOSE);
	}
}


void CMainUninstPage::OnBnClickedBtnWizardCancel()
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	strMsg = L"Do you really want to exit uninstaller?";
	if (::MessageBox(this->m_hWnd, strMsg, L"Uninstaller", MB_YESNO) == IDYES)
	{
		CWnd* pMainWnd = AfxGetMainWnd();
		pMainWnd->PostMessageW(WM_CLOSE);
	}
}



void CMainUninstPage::GetOptions()
{
	int nSize = sizeof(g_UninstOption);
	CFile rfile;
	if (rfile.Open(g_wszModulePathName, CFile::modeRead | CFile::shareDenyNone | CFile::modeNoInherit))
	{
		rfile.Seek(rfile.GetLength() - nSize, CFile::begin);
		nSize = rfile.Read(&g_UninstOption, nSize);
		rfile.Close();
	}
}



void CMainUninstPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CPropertyPage::OnTimer(nIDEvent);
}
