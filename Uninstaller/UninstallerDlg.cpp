
// UninstallerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "UninstallerDlg.h"
#include "afxdialogex.h"

#include <string>
#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
#include <vector>
using namespace std;

#include "uninst.h"

#define  TIMER_SILENTMODE	10
#define  UNINSTLST		L"uninst.lst"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUninstallerDlg dialog




CUninstallerDlg::CUninstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUninstallerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUninstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUninstallerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_UNINSTALL, &CUninstallerDlg::OnBnClickedBtnUninstall)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CUninstallerDlg message handlers

BOOL CUninstallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetOptions();
	GetInfoFromRegistry();

#ifndef TEST
	if (g_UninstOption.chkOptionsSilentMode)
		SetTimer(TIMER_SILENTMODE, 50, NULL);
#endif // !TEST	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUninstallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUninstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUninstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CUninstallerDlg::OnBnClickedBtnUninstall()
{
	// TODO: Add your control notification handler code here
	CString strMsg;
	if (!g_UninstOption.chkOptionsSilentMode)
	{
		strMsg.Format(L"%s (ver %s) will be removed(%s).\nDo you really want to remove?", g_strProductName, g_strProductVersion, g_strInstallPath);

		if (MessageBox(strMsg, L"Warning", MB_YESNO) != IDYES)			return;
	}
	

	if (g_UninstOption.chkCloseMainExecutable)
	{
		CloseMainExecutable();
	}

	if (g_UninstOption.chkCloseRunningApp)
	{
		DWORD dwPid;
		int nCnt = ProcessFind(g_UninstOption.wszRunningAppFile, dwPid);
		if (nCnt > 0)
		{
			HANDLE	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
			BOOL bRet = TerminateProcess(hProc, 0);
		}
	}

	//////////Delete all files///////////
	if (g_UninstOption.chkOptionsCompleteUninstalltion)
		CompleteUninstall();
	else
		LstUninstall();

	/////////Delete All Shortcuts//////////
	DeleteShortcut();

	////////Unregist///////////////
	UnregUninstaller();
	DelStartup();
	DeleteInfoFromRegistry();

	//option//
	if (g_UninstOption.chkRestartCom)
	{
		system("shutdown /r /t 0");
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

	if (!g_UninstOption.chkOptionsSilentMode)
		MessageBox(L"Uninstall Complete!", L"", MB_OK);

	PostMessage(WM_CLOSE);
}


void CUninstallerDlg::GetOptions()
{
	int nSize = sizeof(g_UninstOption);
	CFile rfile;
	if(rfile.Open(g_wszModulePathName, CFile::modeRead|CFile::shareDenyNone|CFile::modeNoInherit))
	{
		rfile.Seek(rfile.GetLength() - nSize, CFile::begin);
		nSize = rfile.Read(&g_UninstOption, nSize);
		rfile.Close();
	}
}

void CUninstallerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_SILENTMODE)
	{
		KillTimer(TIMER_SILENTMODE);
		ShowWindow(SW_HIDE);
		OnBnClickedBtnUninstall();
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CUninstallerDlg::CompleteUninstall()
{
	/*CString strModulePath = g_wszModulePath;
	strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));*/
	if (g_strDeleteDir.IsEmpty())
		return;
	DeleteDirectory(g_strDeleteDir, TRUE);
}


void CUninstallerDlg::LstUninstall()
{
	CString strUninstLst = g_strDeleteDir + L"\\" + UNINSTLST;

	vector<CString> vecDelLst;

	CStringA strFileA(strUninstLst);
	ifstream inputFile(strFileA.GetBuffer());
	int l = 0;

	while (inputFile) {
		l++;
		string s;
		if (!getline(inputFile, s)) break;

		CStringA strFullPathA;
		strFullPathA.Format("%s", s.c_str());

		CString str(strFullPathA);
		vecDelLst.push_back(str);
	}

	if (!inputFile.eof()) {
	}

	for (INT i = 0; i < vecDelLst.size(); i++)
	{
		//AfxMessageBox(vecDelLst[i]);
		if (PathFileExists(vecDelLst[i]))	DeleteFile(vecDelLst[i]);		
	}
}
