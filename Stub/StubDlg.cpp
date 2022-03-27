
// StubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Stub.h"
#include "StubDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"
#include "../common/zlib/Process.h"
#include "../common/other.h"
#include "../common/TRegistry.h"
#include "../common/regstr.h"
#include "../common/SHA1.h"

#include "CDownloaderDlg1.h"
int32_t CheckPrerequistes(HWND hWnd, CString& strPrerequisiteName);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  TIMER_NOINTERFACE	2
#define  TIMER_INSTALL_CONTINUE  1
#define  TIMER_REPAIR_CONTINUE  3


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


// CStubDlg dialog




CStubDlg::CStubDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStubDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bComplete = FALSE;
	m_bRun = FALSE;
	m_bPause = FALSE;
}

void CStubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_EXTRACT, m_ctrlPogExtract);
	DDX_Control(pDX, IDC_BTN_START_INSTALL, m_btnInstall);
	DDX_Control(pDX, IDC_SYSLINK_STUB_OPTION, m_lnkOption);
	DDX_Control(pDX, IDC_SYSLINK_STUB_LICENSE_LINK, m_lnkLicense);
	DDX_Control(pDX, IDC_SYSLINK_STUB_COPYRIGHT_LINK, m_lnkCompany);
}

BEGIN_MESSAGE_MAP(CStubDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_INSTALL, &CStubDlg::OnBnClickedBtnStartInstall)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SYSLINK_STUB_LICENSE_LINK, &CStubDlg::OnBnClickedSyslinkStubLicenseLink)
	ON_BN_CLICKED(IDC_SYSLINK_STUB_OPTION, &CStubDlg::OnBnClickedSyslinkStubOption)
	ON_BN_CLICKED(IDC_SYSLINK_STUB_COPYRIGHT_LINK, &CStubDlg::OnBnClickedSyslinkStubCopyrightLink)
END_MESSAGE_MAP()


// CStubDlg message handlers

BOOL CStubDlg::OnInitDialog()
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

	///////////////////////////////
	char szbuffer[MAX_PATH];
	DWORD len = MAX_PATH;
	GetUserNameA(szbuffer, &len);
	CString str(szbuffer);
	strUserName = str;

	//
	m_bOpened = FALSE;
	m_CprPtr = NULL;
	m_DataPtr = NULL;

	m_pLogoBitmapBuff = NULL;
	m_pLicBuff = NULL;
	m_pReadmeBuff = NULL;
	
	m_ctrlPogExtract.SetRange(0, 100);
	m_ctrlPogExtract.SetPos(0);
	m_ctrlPogExtract.ShowWindow(SW_HIDE);

	GetModuleFullPathName();
	init_log();

	//read file//
	m_bOpened = m_rfile.Open(g_wszModulePathName, CFile::modeRead|CFile::shareDenyNone|CFile::modeNoInherit);
	if (m_bOpened)
	{
		if (g_pLog)		g_pLog->Write(L"finding header.");
		//get stub size//
		UINT64 dwStubSize = 0;
		m_rfile.Seek(m_rfile.GetLength() - sizeof(dwStubSize), CFile::begin);
		m_rfile.Read(&dwStubSize, sizeof(dwStubSize));

		//seek to global option param//
		m_rfile.Seek(dwStubSize, CFile::begin);
		
		//get global option param//
		SetStatusText(L"Getting option param...");

		if (g_pLog)		g_pLog->Write(L"finding option marker.");
		GetGlobalOptionParam();
		

		//if (g_pLog)		g_pLog->Write(L"finding logo bitmaps marker.");
		GetBitmaps();
		
		if (g_pLog)		g_pLog->Write(L"finding readme and license marker.");
		GetLicAndReadme();
		

		/////template editor//////
		//set font//
		m_Fontbtn.CreatePointFont(g_templateEditor.nButtonFontSize * 5, g_templateEditor.wszFontName); 
		m_btnInstall.SetFont(&m_Fontbtn);
		m_Fontlnk.CreatePointFont(g_templateEditor.nLicAndCopyrightSize * 5, g_templateEditor.wszFontName); 

		////////////proc////////////
		SetStatusText(L"Ready");

		GetDirNamesAndVariables();
		//SetDlgItemText(IDC_EDIT_INSTALL_DIR, g_strInstDir);

		//extract setup prerequisites exe//
		//ExtractSetupPrerequisites();

		//no interface : silent(hide)//
		if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)//no interface//
		{
			if (g_pLog)		g_pLog->Write(L"silent install mode.");
			this->MoveWindow(0, 0, 0, 0);
			SetTimer(TIMER_NOINTERFACE, 10, NULL);
			//installing//
			m_bRun = TRUE;
			StartMainInstall();

			return TRUE;
		}

		if (g_pLog)		g_pLog->Write(L"modern install mode.");
		m_strText.Format(L"This will install %s %s on your computer.",
			g_globaldata.general_sh1.info.wszProductName,
			g_globaldata.general_sh1.info.wszProductVersion);
		
		m_strBtn = L"INSTALL";
		m_strHeader.Format(L"Welcome to %s Setup", g_globaldata.general_sh1.info.wszProductName);	
	}
	else
	{
		g_pLog->Write(L"setup file not opened!");
		m_strText = L"Read Failed. Please retry.";
	}	

	//window size//
	if (g_templateEditor.nChkWindowSizeBorderless)
	{
		this->ModifyStyle(WS_BORDER, 0);
		this->ModifyStyle(0, WS_CAPTION);
	}
	else
		this->ModifyStyle(0, WS_BORDER|WS_CAPTION);

	MoveWindow(0, 0, g_templateEditor.nWindowSizeWidth, g_templateEditor.nWindowSizeHeight + 30);
	CenterWindow();
	SetBackgroundColor((COLORREF)g_templateEditor.nWindowColor);
	SetTimer(10, 100, NULL);

	m_lnkCompany.SetURL(g_globaldata.general_sh1.info.wszWebSite);
	m_lnkOption.SetURL(L"option");
	m_lnkLicense.SetURL(L"license");
	

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStubDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStubDlg::OnPaint()
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
		DrawWindowComponents();

		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStubDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStubDlg::OnBnClickedBtnStartInstall()
{
	// TODO: Add your control notification handler code here
	
	CString strAlreadyProName, strAlreadyProVer;
	INT nAlreadyInstalled = CheckAlreadyInstalled(strAlreadyProName, strAlreadyProVer);

	//not installed but install folder exists//
	if (nAlreadyInstalled == 0 && PathFileExists(g_mapDirList[L"#INSTALL_DIR#"]))
	{
		CString strtxt;
		strtxt.Format(L"The installation folder already exists, would you like to continue installing?");
		if (::MessageBox(NULL, strtxt, L"Warning", MB_YESNO|MB_ICONWARNING) != IDYES)
		{
			PostMessage(UM_SHOW_STUB_OPTION);
			return;
		}
	}

	//lower version installed//
	if (nAlreadyInstalled == -1)
	{
		CString strMsg;
		strMsg.Format(L"%s %s has been already installed on your computer. But this version is %s.\nDo you want to install this low version?",
			strAlreadyProName, strAlreadyProVer, g_globaldata.general_sh1.info.wszProductVersion);
		if (::MessageBox(NULL, strMsg, L"Warning", MB_YESNO) == IDNO)
		{
			PostMessage(WM_CLOSE);
			return;
		}

		nAlreadyInstalled = 0;
	}

	if (m_bRun)
	{
		m_bPause = TRUE;
		if(MessageBox(L"Do you really want to stop?", L"Stub", MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			g_pLog->Write(L"setup is aborted.");
			
			//delete half-copied files//
			DeleteDirectory(g_mapDirList[L"#INSTALL_DIR#"], TRUE);
			g_pLog->Write(L"delete directory <%s>", g_mapDirList[L"#INSTALL_DIR#"]);

			//delete registry keys//
			UnregisterValues();

			g_pLog->Write(L"Unregist all values.");
			//delete shortcut files//
			DeleteShortcuts();
			

			PostMessage(WM_CLOSE);
		}
		else
		{
			m_bPause = FALSE;
			return;
		}
	}
	else if (m_bComplete || !m_bOpened)
	{
		if (m_bComplete)
		{
			FinishTab();
		}

		PostMessage(WM_CLOSE);
	}
	else if (nAlreadyInstalled == 0)
	{
		//uninst list param init//
		init_uninstlst();

		int ret = CheckUserInfo();
		if (ret < 0)
		{
			CString strMsg = L"";
			switch(ret)
			{
			case -1:
				strMsg = L"Please input correct user name.";
				break;
			case -2:
				strMsg = L"Please input correct user email.";
				break;
			case -3:
				strMsg = L"Please input correct company name.";
				break;
			case -4:
				strMsg = L"Please input correct custom field.";
				break;
			case -5:
				strMsg = L"Please input correct serial number.";
				break;
			default:
				strMsg = L"Information is not correct or License Key does not match. Please input correct information.";
				break;
			}
			::MessageBox(this->m_hWnd, strMsg, L"Warnning", MB_OK);
			PostMessage(UM_SHOW_STUB_OPTION);
			return;
		}


		BOOL bClose = FALSE;
		int nRet = 0;
		ExtractTempFiles();//extract temp files//
		while(1)
		{			
			CString strPrereauisiteName;
			nRet = CheckPrerequistes(this->m_hWnd, strPrereauisiteName);

			CString strMsg = L"Please wait for installation of prerequisites.\n\nIf prerequisites are installed successfully, please click Retry to continue.\nOr if you don't want to install this application, please click Cancel to exit.";
			strMsg = L"Please wait for installation of prerequisites.\n\n";
			strMsg.Append(L"If you press Continue, the installer continues without waiting for the pre-requisites to be installed.\n");
			strMsg.Append(L"Or if you press Cancel, it cancels installation of main Installer.");

			if (nRet > 0 || nRet < 0)	
				break;
			
			CString strTitle;
			strTitle.Format(L"Installer \"%s\"", strPrereauisiteName);
			UINT nMsgRet = ::MessageBox(this->m_hWnd, strMsg, strTitle, MB_CANCELTRYCONTINUE | MB_ICONWARNING);
			if(nMsgRet == IDCANCEL)
			{
				bClose = TRUE;
				break;
			}
			else if (nMsgRet == IDCONTINUE)
			{
				break;
			}
		}
		
		if (bClose)
		{
			PostMessage(WM_CLOSE);
		}
		else
		{
			//installing//		
			m_bRun = TRUE;
			StartMainInstall();
		}
	}
	else if (nAlreadyInstalled == 1)
	{
		CString strmsg;
		strmsg.Format(L"%s has been already installed.\r\n", g_globaldata.general_sh1.info.wszProductName);
		strmsg = strmsg + L"If you press yes, this application will be repaired.\r\n";
		strmsg = strmsg + L"Or if you press no, this application will be uninstalled.";
		INT nCmd = MessageBox(strmsg, L"Installer", MB_YESNOCANCEL);
		if (nCmd == IDYES)//repair//
		{
			m_strText.Format(L"Please wait while %s %s is being repaired on your computer.",
				g_globaldata.general_sh1.info.wszProductName,
				g_globaldata.general_sh1.info.wszProductVersion);
			m_strBtn = L"STOP";
			m_strHeader.Format(L"Repairing...");

			m_btnInstall.ShowWindow(SW_HIDE);
			m_ctrlPogExtract.ShowWindow(SW_SHOW);
			DrawWindowComponents();

			DWORD dwThID;
			m_hExtractThread = CreateThread(NULL, 0, thRepairing, this, 0, &dwThID);

			SetTimer(TIMER_REPAIR_CONTINUE, 100, NULL);
		}
		else if (nCmd == IDNO)//uninstall//
		{
			m_strText.Format(L"Please wait while %s %s is being uninstalled on your computer.",
				g_globaldata.general_sh1.info.wszProductName,
				g_globaldata.general_sh1.info.wszProductVersion);
			m_strBtn = L"STOP";
			m_strHeader.Format(L"Uninstalling...");

			m_btnInstall.ShowWindow(SW_HIDE);
			m_ctrlPogExtract.ShowWindow(SW_SHOW);
			DrawWindowComponents();

			DWORD dwThID;
			m_hExtractThread = CreateThread(NULL, 0, thUninstalling, this, 0, &dwThID);
		}
		else
			PostMessage(WM_CLOSE);
	}
}

void CStubDlg::ExtractTempFiles()
{
	for (INT i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
	{
		CString strPathName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		if (strPathName.Find(L"#SETUP_TEMP_DIR#") >= 0)
		{
			UnZipFile(strPathName.GetBuffer());
			if (g_pLog)		g_pLog->Write(L"extracted temp file : <%s>.", strPathName);
		}		
	}
}

void CStubDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CloseFunction();

	CDialogEx::OnClose();
}

void CStubDlg::SetStatusText(CString strSts)
{
	/*SetDlgItemText(IDC_STATIC_STATUS, strSts);*/
}

void CStubDlg::StartMainInstall()
{
	////////////////////////
	m_strText.Format(L"Please wait while %s %s is being installed on your computer.",
		g_globaldata.general_sh1.info.wszProductName,
		g_globaldata.general_sh1.info.wszProductVersion);
	m_strBtn = L"STOP";
	m_strHeader.Format(L"Installing...");

	m_btnInstall.ShowWindow(SW_HIDE);
	m_ctrlPogExtract.ShowWindow(SW_SHOW);
	DrawWindowComponents();

	////////condition////////
	General_Requirements();

	//set install dir//
	if (g_mapDirList.find(L"#INSTALL_DIR#") != g_mapDirList.end())
		g_mapDirList[L"#INSTALL_DIR#"] = g_strInstDir;

	SetStatusText(L"Extracting zip files...");
	ExtractZipFiles();

	SetTimer(TIMER_INSTALL_CONTINUE, 100, NULL);
}

void CStubDlg::CloseFunction()
{
	if (m_bRun)
	{
		m_bRun = FALSE;
		Sleep(300);

		KillTimer(1);
		//KillTimer(TIMER_PREREQUISITES);
	}

	//close file//
	if (m_bOpened)
	{
		m_rfile.Close();
	}

	if (m_CprPtr)
	{
		free(m_CprPtr);
		m_CprPtr = NULL;
	}

	if (m_DataPtr)
	{
		free(m_DataPtr);
		m_DataPtr = NULL;
	}

	if (m_pLogoBitmapBuff)
	{
		delete[] m_pLogoBitmapBuff;
		m_pLogoBitmapBuff = NULL;
	}

	if (m_pLicBuff)
	{
		delete[] m_pLicBuff;
		m_pLicBuff = NULL;
	}

	if (m_pReadmeBuff)
	{
		delete[] m_pReadmeBuff;
		m_pReadmeBuff = NULL;
	}

	close_uninstlst();
	close_log();

	if (g_pLog)		g_pLog->Write(L"finished installation.");
}

BOOL CStubDlg::GetGlobalOptionParam()
{
	//get MARKER_OPTION
	UINT64 nMarkerOption;
	m_rfile.Read(&nMarkerOption, sizeof(nMarkerOption));
	if (nMarkerOption != MARKER_OPTION)
	{
		if (g_pLog)		g_pLog->Write(L"failed option marker.");
		return FALSE;		
	}
	else
	{
		//get global param(options)//
		INT nCnt = 0;

		/************sheet1*************/
		//load sh1-pg1
		m_rfile.Read(&g_globaldata.general_sh1.info, sizeof(g_globaldata.general_sh1.info));
		//load sh1-pg2
		m_rfile.Read(&g_globaldata.general_sh1.params, sizeof(g_globaldata.general_sh1.params));
		//load sh1-pg3
		m_rfile.Read(&g_globaldata.general_sh1.requires, sizeof(g_globaldata.general_sh1.requires));
		//load sh1-pg4
		m_rfile.Read(&g_globaldata.general_sh1.prerequisites, sizeof(g_globaldata.general_sh1.prerequisites) - VECSIZE);
		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDPREREQUISITE value;
			m_rfile.Read(&value, sizeof(ADDPREREQUISITE));
			g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.push_back(value);
		}
		//load sh1-pg5
		m_rfile.Read(&g_globaldata.general_sh1.output, sizeof(g_globaldata.general_sh1.output));

		/************sheet2*************/
		//load sh2-pg1
		m_rfile.Read(&g_globaldata.install_sh2.filesandfolders, sizeof(g_globaldata.install_sh2.filesandfolders) - VECSIZE);
		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDINSTALLATIONFILES value;
			m_rfile.Read(&value, sizeof(ADDINSTALLATIONFILES));
			g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(value);
		}

		//load sh2-pg2
		m_rfile.Read(&g_globaldata.install_sh2.shortcuts, sizeof(g_globaldata.install_sh2.shortcuts) - VECSIZE);
		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDSHORTCUTDETAIL value;
			m_rfile.Read(&value, sizeof(ADDSHORTCUTDETAIL));
			g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
		}
		//load sh2-pg3
		m_rfile.Read(&g_globaldata.install_sh2.uninst, sizeof(g_globaldata.install_sh2.uninst));
		//load sh2-pg4
		m_rfile.Read(&g_globaldata.install_sh2.updater, sizeof(g_globaldata.install_sh2.updater));
		//load sh2-pg5
		m_rfile.Read(&g_globaldata.install_sh2.advanced, sizeof(g_globaldata.install_sh2.advanced));

		/************sheet3*************/
		//load sh3-pg1
		m_rfile.Read(&g_globaldata.interface_sh3.main, sizeof(g_globaldata.interface_sh3.main));
		//load sh3-pg2
		m_rfile.Read(&g_globaldata.interface_sh3.lang, sizeof(g_globaldata.interface_sh3.lang));
		//load sh3-pg5
		m_rfile.Read(&g_globaldata.interface_sh3.userinfo, sizeof(g_globaldata.interface_sh3.userinfo));
		//load sh3-pg6
		m_rfile.Read(&g_globaldata.interface_sh3.custom, sizeof(g_globaldata.interface_sh3.custom));
		//load sh3-pg7
		m_rfile.Read(&g_globaldata.interface_sh3.finish, sizeof(g_globaldata.interface_sh3.finish));

		/************sheet4*************/
		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDREGISTRYVALUE value;
			m_rfile.Read(&value, sizeof(ADDREGISTRYVALUE));
			g_globaldata.sys_sh4.registry.vecAddRegvalue.push_back(value);
		}


		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDINIVALUE value;
			m_rfile.Read(&value, sizeof(ADDINIVALUE));
			g_globaldata.sys_sh4.inifiles.vecAddInivalue.push_back(value);
		}

		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDVARIABLE value;
			m_rfile.Read(&value, sizeof(ADDVARIABLE));
			g_globaldata.sys_sh4.variables.vecAddVariable.push_back(value);
		}

		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDCOMMAND value;
			m_rfile.Read(&value, sizeof(ADDCOMMAND));
			g_globaldata.sys_sh4.commands.vecAddCommand.push_back(value);
		}

		/************sheet5*************/
		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDEXTENSION value;
			m_rfile.Read(&value, sizeof(ADDEXTENSION));
			g_globaldata.register_sh5.fileassoc.vecAddExtension.push_back(value);
		}

		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDENVIRONMENTVARIABLE value;
			m_rfile.Read(&value, sizeof(ADDENVIRONMENTVARIABLE));
			g_globaldata.register_sh5.envvar.vecAddEnvironmentVariable.push_back(value);
		}

		m_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			_REGISTER value;
			m_rfile.Read(&value, sizeof(_REGISTER));
			g_globaldata.register_sh5.others.vecRegister.push_back(value);
		}

		//get template editor information//
		if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 2)//modern interface//
		{
			m_rfile.Read(&g_templateEditor, sizeof(g_templateEditor));
		}

		if (g_pLog)		g_pLog->Write(L"found option marker.");
		return TRUE;
	}

}

void CStubDlg::DrawWindowComponents(BOOL bLinkChange)
{	
	if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 2)//modern interface//
	{
		//draw//
		GetRects();

		DrawLogo();	
		DrawHeader();	
		DrawText();
		DrawButton();
		DrawLicenseAndCopyright(bLinkChange);
	}	
	else
	{
		m_lnkLicense.ShowWindow(SW_HIDE);
		m_lnkCompany.ShowWindow(SW_HIDE);
		m_lnkOption.ShowWindow(SW_HIDE);		
	}
}

void CStubDlg::DisplayText(HDC dc, wchar_t* wsztext, RECT r, wchar_t* wszFontFaceName,  int fontsize, COLORREF clr, int dwStyle, int bTop)
{
	::SetBkMode(dc, TRANSPARENT);
	::SetTextColor(dc, clr);

	DWORD bItalic = 0;
	if (dwStyle == 0)
	{
		dwStyle = FW_REGULAR;
	}
	else if (dwStyle == 1)
	{
		dwStyle = FW_BOLD;
	}
	else if (dwStyle == 2)
	{
		dwStyle = FW_REGULAR;
		bItalic = 1;
	}
	else
	{
		dwStyle = FW_BOLD;
		bItalic = 1;
	}


	HFONT MyFont = CreateFont(fontsize, 0, 0, 0, dwStyle, bItalic, 0, 0, 0, 0, 0, 0, 0, wszFontFaceName);
	::SelectObject(dc, MyFont);
	DWORD dwPos = DT_CENTER;
	if (bTop == 0)
	{
		dwPos |= DT_VCENTER;
	}
	else if (bTop = 1)
	{
		dwPos |= DT_TOP;
	}
	else if (bTop = -1)
	{
		dwPos |= DT_BOTTOM;
	}

	::DrawTextW(dc, wsztext, -1, &r, dwPos);
	::DeleteObject(MyFont);
}

HBITMAP GetHbitmap(byte* buff)
{
	DWORD nFileSize, dwRead;
	BITMAPFILEHEADER fh;
	BITMAPINFO* ih;

	memcpy(&fh, buff, sizeof(BITMAPFILEHEADER));
	nFileSize = fh.bfOffBits - sizeof(BITMAPFILEHEADER);
	ih = (BITMAPINFO*)malloc(nFileSize);
	memcpy(ih, buff + sizeof(BITMAPFILEHEADER), nFileSize);

	PVOID pRaster;
	HBITMAP hBit = CreateDIBSection(NULL, ih, DIB_RGB_COLORS, &pRaster, NULL, 0);
	//pRaster = buff + sizeof(BITMAPFILEHEADER) + nFileSize;
	memcpy(pRaster, buff + sizeof(BITMAPFILEHEADER) + nFileSize, fh.bfSize - fh.bfOffBits);
	free(ih);

	return hBit;
}

void CStubDlg::DrawLogo()
{
	CClientDC dc(this);
	CRect rect;
	GetClientRect(rect);
	HBRUSH hbr = CreateSolidBrush(g_templateEditor.nWindowColor);
	::SelectObject(dc, hbr);
	FillRect(dc.GetSafeHdc(), &rect, hbr);

	::SetStretchBltMode(dc.m_hDC, COLORONCOLOR);

	//transparent//
	CDC dc_background;
	CDC dc_image;
	CDC dc_mask;
	CBitmap bmp_background, *p_bmp_background;
	CBitmap bmp_image, *p_bmp_image;
	CBitmap bmp_mask, *p_bmp_mask;
	BITMAP bmsize_background;
	BITMAP bmsize_image;
	COLORREF clr_mask = RGB(255,255,255);	// BACKGROUND COLOR : WHITE
	COLORREF old_clr_bkgnd;
	COLORREF old_clr_text;

	///
	dc_background.CreateCompatibleDC(&dc);
	dc_image.CreateCompatibleDC(&dc);
	dc_mask.CreateCompatibleDC(&dc);

	//	
	if (m_pLogoBitmapBuff)
	{
		HBITMAP hbmp1, hbmp2;
		hbmp1 = GetHbitmap(m_pLogoBitmapBuff);
		hbmp2 = GetHbitmap(m_pLogoBitmapBuff);
		bmp_background.Attach(hbmp1);
		bmp_image.Attach(hbmp2);
	}
	else
	{
		bmp_background.LoadBitmap(IDB_BITMAP_DEFAULT_LOGO);
		bmp_image.LoadBitmap(IDB_BITMAP_DEFAULT_LOGO);
	}

	bmp_background.GetBitmap(&bmsize_background);
	bmp_image.GetBitmap(&bmsize_image);
	bmp_mask.CreateBitmap(bmsize_image.bmWidth, bmsize_image.bmHeight, 1, 1, NULL);

	//
	p_bmp_background = dc_background.SelectObject(&bmp_background);
	p_bmp_image = dc_image.SelectObject(&bmp_image);
	p_bmp_mask = dc_mask.SelectObject(&bmp_mask);

	/////////////////////////
	RECT re;

	if (g_templateEditor.nLogoSettingAutoSize)
	{
	}

	if (g_templateEditor.nLogoSettingTransparent)
	{
		/////////////////////////
		//1
		old_clr_bkgnd = dc_image.SetBkColor(clr_mask);
		dc_mask.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, &dc_image, 0, 0, SRCCOPY);
		dc_image.SetBkColor(old_clr_bkgnd);

		//2
		RECT re1;
		re1.left = 0;		re1.right = bmsize_image.bmWidth;
		re1.top = 0;			re1.bottom = bmsize_image.bmHeight;
		HBRUSH hbr = CreateSolidBrush(g_templateEditor.nWindowColor);
		::SelectObject(dc, hbr);
		FillRect(dc_background.GetSafeHdc(), &re1, hbr);
		::DeleteObject(hbr);
		dc_background.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, &dc_mask, 0, 0, SRCAND);

		//3
		old_clr_bkgnd = dc_image.SetBkColor(RGB(0, 0, 0));
		old_clr_text = dc_image.SetTextColor(RGB(255, 255, 255));
		dc_image.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, &dc_mask, 0, 0, SRCAND);
		dc_image.SetBkColor(old_clr_bkgnd);
		dc_image.SetTextColor(old_clr_text);

		/*4. draw background*/
		dc_background.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, &dc_image, 0, 0, SRCPAINT);
	}

	if (g_templateEditor.nLogoSettingProportioal)
	{
		re.top = 0;
		re.bottom = bmsize_image.bmHeight;
		re.left = (m_reLogo.Width() - bmsize_image.bmWidth) / 2;
		re.right = (m_reLogo.Width() + bmsize_image.bmWidth) / 2;
	}
	else if (g_templateEditor.nLogoSettingStretch)
	{
		CRect rect;
		GetClientRect(&rect);
		re.left = 0;		re.right = rect.Width();
		re.top = 0;			re.bottom = rect.Height();
	}
	else
	{
		re.top = 0;
		re.bottom = m_reLogo.bottom;
		re.left = m_reLogo.left;
		re.right = m_reLogo.right;
	}	

	//draw//
	dc.StretchBlt(re.left, re.top, re.right, re.bottom, &dc_background, 0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, SRCCOPY);

	dc_image.SelectObject(p_bmp_image);
	dc_mask.SelectObject(p_bmp_mask);

	dc_image.DeleteDC();
	dc_mask.DeleteDC();

	bmp_image.DeleteObject();
	bmp_mask.DeleteObject();
}

void CStubDlg::DrawHeader()
{
	//get header rect//
	RECT re;
	re.left = m_reHdr.left;		re.right = m_reHdr.right;
	re.top = m_reHdr.top + g_templateEditor.nHeaderOffset;
	re.bottom = m_reHdr.bottom + g_templateEditor.nHeaderOffset;

	DisplayText(this->GetDC()->m_hDC, 
		m_strHeader.GetBuffer(),
		re, 
		g_templateEditor.wszFontName,
		g_templateEditor.nHeaderSize,
		(COLORREF)g_templateEditor.nHeaderColor,
		g_templateEditor.nHeaderStyle);
}


void CStubDlg::DrawText()
{
	//get text rect//
	RECT re;
	re.left = m_reText.left;		re.right = m_reText.right;
	re.top = m_reText.top + g_templateEditor.nTextOffset;
	re.bottom = m_reText.bottom + g_templateEditor.nTextOffset;	

	if (wcslen(g_globaldata.general_sh1.info.wszProductDescription) > 0)
	{
		m_strText.Format(L"%s", g_globaldata.general_sh1.info.wszProductDescription);
	}

	DisplayText(this->GetDC()->m_hDC, 
		m_strText.GetBuffer(),
		re, 
		g_templateEditor.wszFontName,
		g_templateEditor.nTextSize,
		(COLORREF)g_templateEditor.nTextColor,
		g_templateEditor.nTextStyle);
}


void CStubDlg::DrawButton()
{
	CRect rect;
	GetClientRect(&rect);
	m_reButton.left = (rect.Width() - g_templateEditor.nButtonWidth) / 2;		m_reButton.right = (rect.Width() + g_templateEditor.nButtonWidth) / 2;
	m_reButton.top = rect.Height() * 2 / 3 - 15 + g_templateEditor.nButtonOffset;
	m_reButton.bottom = m_reButton.top + 26;

	m_btnInstall.MoveWindow(&m_reButton);
	m_ctrlPogExtract.MoveWindow(20, m_reButton.top, rect.Width() - 20, m_reButton.bottom - m_reButton.top);
	m_btnInstall.SetWindowTextW(m_strBtn);
}


void CStubDlg::DrawLicenseAndCopyright(BOOL bLinkChange)
{
	CString strText;
	//draw option//
	CRect rect;
	GetClientRect(&rect);

	RECT re;
	re.left = rect.Width() / 2 - g_templateEditor.nLicAndCopyrightSize / 1.6 * 3;		
	re.right = m_reButton.right - 30;
	re.top = m_reButton.top + 31;
	re.bottom = re.top + g_templateEditor.nLicAndCopyrightSize;
	m_lnkOption.MoveWindow(&re);

	if (bLinkChange)
	{
		m_lnkOption.SetHyperlinkFont(g_templateEditor.wszFontName,	(int)(g_templateEditor.nLicAndCopyrightSize / 1.6), 0, 0);
		m_lnkOption.SetHyperlinkColors(				// Set the hyperlink colors:
			g_templateEditor.nLicAndCopyrightLinkColor,			//   ... normal link,
			g_templateEditor.nLicAndCopyrightLinkColor,			//   ... hover,
			g_templateEditor.nLicAndCopyrightLinkColor);			//   ... visited link.
		m_lnkOption.SizeToContent(TRUE, FALSE);
		m_lnkOption.SetTextAlignment(m_lnkOption.Center);	
	}
	


	//draw copyright//
	re.bottom = rect.bottom - 10;
	re.top = re.bottom - g_templateEditor.nLicAndCopyrightSize;
	INT nLen = 0;
	if (g_templateEditor.nLicAndCopyrightPosition == 0)
	{
		re.left = rect.left + 30;
		re.right = g_templateEditor.nLicAndCopyrightSize * 9  + re.left;
	}
	else
	{
		nLen = g_templateEditor.nLicAndCopyrightSize * 9 + g_templateEditor.nLicAndCopyrightSize * 4;
		if (g_templateEditor.nLicAndCopyrightSize < 18)			nLen -= 12;
		re.left = (rect.Width() - nLen) / 2;
		re.right = g_templateEditor.nLicAndCopyrightSize * 9  + re.left;
	}

	strText = L"Copyright(C)2021";

	DisplayText(this->GetDC()->m_hDC, 
		strText.GetBuffer(),
		re, 
		g_templateEditor.wszFontName,
		g_templateEditor.nLicAndCopyrightSize,
		(COLORREF)g_templateEditor.nLicAndCopyrightTextColor);

	//draw company link//
	if (g_templateEditor.nLicAndCopyrightSize < 18)
		re.right = re.right - 12;
	re.left = re.right;
	re.right = re.left + g_templateEditor.nLicAndCopyrightSize * 4;
	re.top = re.top - 1;

	strText.Format(L"%s", g_globaldata.general_sh1.info.wszCompanyName);
	m_lnkCompany.SetWindowTextW(strText);	

	m_lnkCompany.MoveWindow(&re);

	if (bLinkChange)
	{
		m_lnkCompany.SetHyperlinkFont(g_templateEditor.wszFontName,	(int)(g_templateEditor.nLicAndCopyrightSize / 1.6), 0, 0);
		m_lnkCompany.SetHyperlinkColors(				// Set the hyperlink colors:
			g_templateEditor.nLicAndCopyrightLinkColor,			//   ... normal link,
			g_templateEditor.nLicAndCopyrightLinkColor,			//   ... hover,
			g_templateEditor.nLicAndCopyrightLinkColor);			//   ... visited link.
		m_lnkCompany.SizeToContent(TRUE, FALSE);
		m_lnkCompany.SetTextAlignment(m_lnkOption.Center);
	}

	//////////////////////////////////
	//////////////////////////////////
	//////////////////////////////////
	if (g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement)
	{
		//draw license//
		re.bottom = rect.bottom - 10 - g_templateEditor.nLicAndCopyrightSize - 3;
		re.top = re.bottom - g_templateEditor.nLicAndCopyrightSize;
		nLen = 0;
		if (g_templateEditor.nLicAndCopyrightPosition == 0)
		{
			re.left = rect.left + 30;
			re.right = g_templateEditor.nLicAndCopyrightSize * 9  + re.left;
		}
		else
		{
			nLen = g_templateEditor.nLicAndCopyrightSize * 9 + g_templateEditor.nLicAndCopyrightSize * 4;
			if (g_templateEditor.nLicAndCopyrightSize < 18)			nLen -= 12;
			re.left = (rect.Width() - nLen) / 2;
			re.right = g_templateEditor.nLicAndCopyrightSize * 9  + re.left;
		}

		strText = L"You agree to our ";

		DisplayText(this->GetDC()->m_hDC, 
			strText.GetBuffer(),
			re, 
			g_templateEditor.wszFontName,
			g_templateEditor.nLicAndCopyrightSize,
			(COLORREF)g_templateEditor.nLicAndCopyrightTextColor);

		//draw license link//
		if (g_templateEditor.nLicAndCopyrightSize < 18)
			re.right = re.right - 12;
		re.left = re.right;
		re.right = re.left + g_templateEditor.nLicAndCopyrightSize * 4;
		re.top = re.top - 1;

		strText.Format(L"License Agreement");
		m_lnkLicense.SetWindowTextW(strText);	

		m_lnkLicense.MoveWindow(&re);

		if (bLinkChange)
		{
			m_lnkLicense.SetHyperlinkFont(g_templateEditor.wszFontName,	(int)(g_templateEditor.nLicAndCopyrightSize / 1.6), 0, 0);
			m_lnkLicense.SetHyperlinkColors(				// Set the hyperlink colors:
				g_templateEditor.nLicAndCopyrightLinkColor,			//   ... normal link,
				g_templateEditor.nLicAndCopyrightLinkColor,			//   ... hover,
				g_templateEditor.nLicAndCopyrightLinkColor);			//   ... visited link.
			m_lnkLicense.SizeToContent(TRUE, FALSE);
			m_lnkLicense.SetTextAlignment(m_lnkOption.Center);
		}		
	}
	else
	{
		m_lnkLicense.ShowWindow(SW_HIDE);
	}	
}

BOOL CStubDlg::ExtractZipFiles()
{
	if (g_pLog)		g_pLog->Write(L"finding zip marker.");
	//get zip marker//
	UINT64 nZipOption;
	m_rfile.Read(&nZipOption, sizeof(nZipOption));
	if (nZipOption != MARKER_ZIP)
	{
		if (g_pLog)		g_pLog->Write(L"failed zip marker.");
		return FALSE;
	}

	//extract zip files//
	DWORD dwThID;
	m_hExtractThread = CreateThread(NULL, 0, thStartUnzipProc, this, 0, &dwThID);

	return TRUE;
}

DWORD WINAPI thStartUnzipProc(LPVOID lParam)
{
	CStubDlg* pDlg = (CStubDlg*)lParam;
	pDlg->UnzipProc();
	return 0;
}

CFile rPackageFile;
void CStubDlg::UnzipProc(BOOL bRepair)
{
	INT nFileCnt = g_globaldata.install_sh2.filesandfolders.vecInstallationData.size();
	
	//check package//
	BOOL bPkgFileOpened = FALSE;
	if (nFileCnt > 0 && g_globaldata.general_sh1.output.nPackageTypeMode != 0)//no data inside//
	{
		CString strPackageFileFullName(g_wszModulePathName);
		strPackageFileFullName = strPackageFileFullName.Left(strPackageFileFullName.ReverseFind(L'\\'));
		
		//package download from web//
		if (nFileCnt > 0 && g_globaldata.general_sh1.output.nPackageTypeMode == 2)
		{
			if (g_pLog)		g_pLog->Write(L"start package downloading.");
			CDownloaderDlg1 dlg;
			
			CString strURL, strPkgName;
			strURL.Format(L"%s", g_globaldata.general_sh1.output.wszDataFileName);
			strPkgName = strURL;

			if (strPkgName.ReverseFind(L'\\') >= 0)
			{
				strPkgName = strPkgName.Right(strPkgName.GetLength() - strPkgName.ReverseFind(L'\\') - 1);
			}

			if (strPkgName.ReverseFind(L'/') >= 0)
			{
				strPkgName = strPkgName.Right(strPkgName.GetLength() - strPkgName.ReverseFind(L'/') - 1);
			}
			
			strPackageFileFullName.Format(L"%s\\%s", strPackageFileFullName, strPkgName);

			dlg.SetURL(strURL.GetBuffer());
			dlg.SetSaveFileFullPathName(strPackageFileFullName, TRUE);
			dlg.DoModal();

			//set err//
			if (!dlg.m_bDownloadSuccess)		g_nErr = SINST_DOWNLOAD_PKG_FAILED;

			if (g_nErr == SINST_DOWNLOAD_PKG_FAILED)
			{
				if (g_pLog)		g_pLog->Write(L"package downloading failed.");
				m_bRun = FALSE;
				return;
			}
		}
		else
		{
			strPackageFileFullName.AppendFormat(L"\\%s", g_globaldata.general_sh1.output.wszDataFileName);
		}

		//open package file//
		bPkgFileOpened = rPackageFile.Open(strPackageFileFullName, CFile::modeRead|CFile::shareDenyNone);
		if (!bPkgFileOpened)
		{
			g_nErr = SINST_PKG_FILE_NOT_OPENED;
			if (g_pLog)		g_pLog->Write(L"package file(%s) can't be read.", strPackageFileFullName);
			m_bRun = FALSE;
			return;
		}
	}


	for (INT i = 0; i < nFileCnt; i++)
	{
		while(m_bPause)			Sleep(2);

		CString strPathName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		//skip temp files//
		if (strPathName.Find(L"#SETUP_TEMP_DIR#") >= 0)	continue;

		//uncompress//
		BOOL bpkg = FALSE;
		if (g_globaldata.general_sh1.output.nPackageTypeMode != 0 && bPkgFileOpened)			bpkg = TRUE;

		strPathName = UnZipFile(strPathName.GetBuffer(), bpkg);
		
		//set file time//
		if (PathFileExists(strPathName))
		{
			HANDLE hFile = CreateFile(strPathName, GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				FILETIME ftCreate, ftAccess, ftWrite;
				ftCreate = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].ftCreate;
				ftAccess = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].ftAccess;
				ftWrite = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].ftWrite;
				BOOL bl = SetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite);
				CloseHandle(hFile);
			}
		}

		if (g_pLog)		g_pLog->Write(L"extracted file and set file times : <%s>.", strPathName);

		//set progress//
		int nPos = 100 * (i + 1) / g_globaldata.install_sh2.filesandfolders.vecInstallationData.size();
		m_ctrlPogExtract.SetPos(nPos);

		Sleep(2);
	}
	m_ctrlPogExtract.SetPos(100);
	if (g_pLog)		g_pLog->Write(L"extracted zip files successfully.");

	if (bPkgFileOpened)//close package file//
	{
		rPackageFile.Close();
	}

	////uninstaller/////
	ExtractUninstaller();

	//updater//
	ExtractUpdater();

	//registering//
	if (g_pLog)		g_pLog->Write(L"registering values.");
	RegisterValues();

	/////////////////////////////////////
	m_bRun = FALSE;	
}


CString CStubDlg::GetSaveFileFullPathName(wchar_t* wszName, CString& strFullDir, BOOL bDir)
{
	CString strFullPathName = L"";
	CString strtmp, strParentDir;
	strtmp.Format(L"%s", wszName);

	INT nPos = 0;
	//get parent dir//
	nPos = strtmp.Find(L'\\');
	if (nPos < 0)
		strParentDir = strtmp;
	else
		strParentDir = strtmp.Left(nPos);

	//****************************//
	if (g_mapDirList.find(strParentDir) != g_mapDirList.end())
	{
		strParentDir = g_mapDirList[strParentDir];
	}
	//****************************//

	if(nPos < 0)
	{
		strFullDir = strParentDir;
	}
	else
	{
		//get full path name and full dir//
		strFullPathName = strParentDir + L"\\" + strtmp.Right(strtmp.GetLength() - nPos - 1);
		if (bDir)
		{
			strFullDir = strFullPathName;
			strFullPathName = L"";
		}
		else
		{
			nPos = strFullPathName.ReverseFind(L'\\');
			strFullDir = strFullPathName.Left(nPos);
		}	
	}

	/////////////////////////
	return strFullPathName;
}

void CStubDlg::CreateShortcuts()
{
	SetStatusText(L"Creating shortcuts...");

	for (INT i = 0; i < g_globaldata.install_sh2.shortcuts.vecDetail.size(); i++)
	{
		CString strLoc, strName, strTarget;
		strLoc.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszLocation);
		strName.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszShortcutName);
		strTarget.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszTargetFile);

		if (g_mapDirList.find(strName) != g_mapDirList.end())
			strName = g_mapDirList[strName];

		CreateLink(strLoc, strName, strTarget);		
	}
}

void CStubDlg::DeleteShortcuts(void)
{
	TRegistry	reg(HKEY_CURRENT_USER);
	CString		dir;
	CString		path;

	if (!reg.OpenKey(REGSTR_SHELLFOLDERS)) {
		return;
	}

	CString strAppName = g_globaldata.install_sh2.filesandfolders.wszMainExecutable;
	if (!strAppName.IsEmpty())
	{
		strAppName = strAppName.Left(strAppName.ReverseFind(L'.'));
		strAppName = strAppName.Right(strAppName.GetLength() - strAppName.ReverseFind(L'\\') - 1);

		dir = reg.GetStr(REGSTR_STARTUP, L"");
		if (dir != L"") {
			path = dir + L"\\" + strAppName + L".lnk";
			DeleteLink(path);

			g_pLog->Write(L"delete shortcut link <%s>.", path);
		}
		dir = reg.GetStr(REGSTR_DESKTOP, L"");
		if (dir != L"") {
			path = dir + L"\\" + strAppName + L".lnk";
			DeleteLink(path);

			g_pLog->Write(L"delete shortcut link <%s>.", path);
		}
		dir = reg.GetStr(REGSTR_PROGRAMS, L"");
		if (dir != L"") {
			path =  dir + L"\\" + strAppName;
			DeleteLinkFolder(path);

			g_pLog->Write(L"delete shortcut linkfolder <%s>.", path);
		}

	}	
}


void CStubDlg::CreateLink(CString strLocation, CString strName, CString strTarget)
{
	CString strPathObj;
	CString strPathLink;

	if (g_mapDirList.find(strLocation) == g_mapDirList.end())	return;

	if (strLocation.Find(L"PROGRAM_GROUP") >= 0)
	{
		CString strDir = g_mapDirList[strLocation] + L"\\" + g_mapDirList[L"#APPLICATION_NAME#"];
		SHCreateDirectory(NULL, strDir);
		strPathLink.Format(L"%s\\%s\\%s.lnk", 
			g_mapDirList[strLocation], 
			g_mapDirList[L"#APPLICATION_NAME#"],
			strName);
	}
	else
	{
		strPathLink = g_mapDirList[strLocation] + L"\\" + strName + L".lnk";
	}
	
	CString strfullDir;
	strPathObj = GetSaveFileFullPathName(strTarget.GetBuffer(), strfullDir);

	ShellLink(strPathObj, strPathLink);
	if (g_pLog)		g_pLog->Write(L"created shortcut. : %s", strPathLink);
}


void CStubDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_INSTALL_CONTINUE)
	{
		if (!m_bRun)
		{
			KillTimer(TIMER_INSTALL_CONTINUE);

			if (g_nErr == SINST_NO_ERROR)
			{
				if (g_pLog)		g_pLog->Write(L"creating shortcuts.");
				CreateShortcuts();
				
				//system page//
				if (g_pLog)		g_pLog->Write(L"system registry.");
				SystemRegistry();
				if (g_pLog)		g_pLog->Write(L"system variables.");
				SystemVariable();
				if (g_pLog)		g_pLog->Write(L"system ini files.");
				SystemIniFiles();
				if (g_pLog)		g_pLog->Write(L"system commands.");
				SystemCommands();

				//register page//
				if (g_pLog)		g_pLog->Write(L"register file associations");
				RegisterFileAssociations();

				//complete//
				SetStatusText(L"Complete!");

				m_bComplete = TRUE;
				if (g_pLog)		g_pLog->Write(L"Complete Setting Up Stub.");

				m_strText.Format(L"%s %s has been installed on your computer.",
					g_globaldata.general_sh1.info.wszProductName,
					g_globaldata.general_sh1.info.wszProductVersion);
				m_strBtn = L"FINISH";
				m_strHeader.Format(L"Installation Completed");
				m_btnInstall.ShowWindow(SW_SHOW);
				m_ctrlPogExtract.ShowWindow(SW_HIDE);
				DrawWindowComponents();

				if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)//no interface//
				{
					m_btnInstall.SetWindowTextW(m_strBtn);
					PostMessage(WM_CLOSE);
				}
			}
			else
			{
				//complete//
				SetStatusText(L"Uncompleted");

				m_bComplete = TRUE;
				if (g_pLog)		g_pLog->Write(L"uncompleted!");

				m_strText.Format(L"%s %s hasn't been installed on your computer.",
					g_globaldata.general_sh1.info.wszProductName,
					g_globaldata.general_sh1.info.wszProductVersion);
				m_strBtn = L"FINISH";
				m_strHeader.Format(L"Installation Uncompleted");
				m_btnInstall.ShowWindow(SW_SHOW);
				m_ctrlPogExtract.ShowWindow(SW_HIDE);
				DrawWindowComponents();

				if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)//no interface//
				{
					m_btnInstall.SetWindowTextW(m_strBtn);
					PostMessage(WM_CLOSE);
				}
			}
			
		}
	}
	else if (nIDEvent == TIMER_REPAIR_CONTINUE)
	{
		if (!m_bRun)
		{
			KillTimer(TIMER_REPAIR_CONTINUE);

			if (g_nErr == SINST_NO_ERROR)
			{
				if (g_pLog)		g_pLog->Write(L"creating shortcuts.");
				CreateShortcuts();


				//system page//
				if (g_pLog)		g_pLog->Write(L"system registry.");
				SystemRegistry();
				if (g_pLog)		g_pLog->Write(L"system variables.");
				SystemVariable();
				if (g_pLog)		g_pLog->Write(L"system ini files.");
				SystemIniFiles();
				if (g_pLog)		g_pLog->Write(L"system commands.");
				SystemCommands();

				//register page//
				if (g_pLog)		g_pLog->Write(L"register file associations");
				RegisterFileAssociations();

				//complete//
				SetStatusText(L"Complete!");

				m_bComplete = TRUE;

				m_strText.Format(L"%s %s has been repaired successfully.",
					g_globaldata.general_sh1.info.wszProductName,
					g_globaldata.general_sh1.info.wszProductVersion);
				m_strBtn = L"FINISH";
				m_strHeader.Format(L"Repairing Completed");
				m_btnInstall.ShowWindow(SW_SHOW);
				m_ctrlPogExtract.ShowWindow(SW_HIDE);
				DrawWindowComponents(FALSE);

				if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)//no interface//
				{
					m_btnInstall.SetWindowTextW(m_strBtn);
					PostMessage(WM_CLOSE);
				}

				if (g_pLog)		g_pLog->Write(L"repaired");
			}
			else
			{
				//complete//
				SetStatusText(L"Uncompleted");

				m_bComplete = TRUE;
				if (g_pLog)		g_pLog->Write(L"uncompleted!");

				m_strText.Format(L"%s %s hasn't been repaired.",
					g_globaldata.general_sh1.info.wszProductName,
					g_globaldata.general_sh1.info.wszProductVersion);
				m_strBtn = L"FINISH";
				m_strHeader.Format(L"Repairing Uncompleted");
				m_btnInstall.ShowWindow(SW_SHOW);
				m_ctrlPogExtract.ShowWindow(SW_HIDE);
				DrawWindowComponents();

				if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)//no interface//
				{
					m_btnInstall.SetWindowTextW(m_strBtn);
					PostMessage(WM_CLOSE);
				}
			}
			
		}
	}
	else if (nIDEvent == TIMER_NOINTERFACE)
	{
		ShowWindow(SW_HIDE);
		KillTimer(TIMER_NOINTERFACE);
	}
	else if (nIDEvent == 10)
	{
		DrawWindowComponents();
		KillTimer(10);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CStubDlg::RegisterValues()
{
	SetStatusText(L"Registering...");

	//register information//
	SetInfoIntoRegistry();

	//register active x control//
	for (INT i = 0; i < g_globaldata.register_sh5.others.vecRegister.size(); i++)
	{
		CString strFullPathName, strfullDir;
		strFullPathName = GetSaveFileFullPathName(g_globaldata.register_sh5.others.vecRegister[i].wszFile, strfullDir);
		if (PathFileExists(strFullPathName))
		{
			CStringA szfile(strFullPathName);
			RegisterOthers(szfile.GetBuffer());

			if(g_pLog)	g_pLog->Write(L"registered control : %s", strFullPathName);
		}
		else
			if(g_pLog)	g_pLog->Write(L"registering control failed : %s", strFullPathName);		
	}
}

void CStubDlg::UnregisterValues()
{
	//unreg uninstaller//
	CString strProductName = g_globaldata.general_sh1.info.wszProductName;

	TRegistry reg(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_UNINSTALL))
	{
		reg.DeleteKey(strProductName);
	}
	reg.CloseKey();

	//delete startup//
	reg.ChangeTopKey(HKEY_LOCAL_MACHINE);
	reg.CreateKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	reg.DeleteValue(strProductName);
	reg.CloseKey();

	//delete information// 
	DeleteInfoFromRegistry();

	//unregister active x control//
	for (INT i = 0; i < g_globaldata.register_sh5.others.vecRegister.size(); i++)
	{
		CString strFullPathName, strfullDir;
		strFullPathName = GetSaveFileFullPathName(g_globaldata.register_sh5.others.vecRegister[i].wszFile, strfullDir);
		if (PathFileExists(strFullPathName))
		{
			CStringA szfile(strFullPathName);
			RegisterOthers(szfile.GetBuffer(), FALSE);
		}
	}
}


CString CStubDlg::UnZipFile(wchar_t* wszFileFullPath, BOOL bPkg)
{
	CFile* fp = NULL;
	if (bPkg)
		fp = &rPackageFile;
	else
		fp = &m_rfile;

	CString strDestFullName;
	UINT realLen = 0;
	UINT compLen = 0;

	//get comp len//
	fp->Read(&compLen, sizeof(compLen));
	if (compLen == 0)//directory
	{
		CString strfullDir;
		GetSaveFileFullPathName(wszFileFullPath, strfullDir, TRUE);
		//write//
		if (!PathFileExists(strfullDir))
			SHCreateDirectory(NULL, strfullDir);

		if (g_pLog)		g_pLog->Write(L"created folder <%s>.", strfullDir);
		strDestFullName = strfullDir;
	}
	else//file
	{
		//get real len//
		fp->Read(&realLen, sizeof(realLen));

		m_CprPtr = new unsigned char[compLen];

		//////UnCompress/////
		fp->Read(m_CprPtr, compLen);

		UINT nLen = 0;
		m_DataPtr = ZlibUncompress(m_CprPtr + 4, compLen, realLen, nLen);

		//get save full path name//
		CString strFullPathName, strfullDir;
		strFullPathName = GetSaveFileFullPathName(wszFileFullPath, strfullDir);
		//write//
		if (!PathFileExists(strfullDir))
			SHCreateDirectory(NULL, strfullDir);

		CFile wfile;
		if (ConfirmDiskFreeSpace(nLen))
		{
			if (wfile.Open(strFullPathName, CFile::modeCreate|CFile::modeWrite))
			{
				CString strSts;
				strSts.Format(L"Extracting %s", strFullPathName);
				SetStatusText(strSts);

				wfile.Write(m_DataPtr, nLen);
				wfile.Close();

				WriteUninstallList(TRUE, strFullPathName);
			}
		}


		free(m_CprPtr);
		free(m_DataPtr);

		m_CprPtr = NULL;
		m_DataPtr = NULL;

		strDestFullName = strFullPathName;
	}	

	return strDestFullName;
}


byte* CStubDlg::UnZipBuffer(BOOL btext)
{
	byte* pBuff;

	UINT realLen = 0;
	UINT compLen = 0;

	//get comp len//
	m_rfile.Read(&compLen, sizeof(compLen));
	if (compLen <= 0)	return NULL;

	//get real len//
	m_rfile.Read(&realLen, sizeof(realLen));

	m_CprPtr = new unsigned char[compLen];

	//////UnCompress/////
	m_rfile.Read(m_CprPtr, compLen);

	UINT nLen = 0;
	m_DataPtr = ZlibUncompress(m_CprPtr + 4, compLen, realLen, nLen);
	if (btext)
	{
		pBuff = new byte[nLen + 1];
		pBuff[nLen] = 0;
	}
	else
		pBuff = new byte[nLen];

	
	memcpy(pBuff, m_DataPtr, nLen);

	free(m_CprPtr);
	free(m_DataPtr);

	m_CprPtr = NULL;
	m_DataPtr = NULL;

	return pBuff;
}


void CStubDlg::GetBitmaps()
{
	if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 2)//modern//
	{
		//get marker//
		UINT64 marker;
		m_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_LOGO_BMP)		return;

		m_pLogoBitmapBuff = UnZipBuffer();

		DrawBackground();
	}
}

void CStubDlg::GetLicAndReadme()
{
	UINT64  marker;

	if (g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement == 1)//license//
	{
		////////get license agreement///////////
		//get marker//
		m_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_LICENSE)
		{
			if (g_pLog)		g_pLog->Write(L"failed license marker.");
			return;
		}

		m_pLicBuff = UnZipBuffer(TRUE);
		if (g_pLog)		g_pLog->Write(L"found license marker.");
	}

	if (g_globaldata.interface_sh3.main.chkDialogs2ShowReadme == 1)//read me//
	{
		////////get readme/////////
		//get marker//
		m_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_README)
		{
			if (g_pLog)		g_pLog->Write(L"failed readme marker.");
			return;
		}

		m_pReadmeBuff = UnZipBuffer();
		if (g_pLog)		g_pLog->Write(L"found readme marker.");
	}
}


void CStubDlg::DrawBackground()
{
	return;
	if (m_pLogoBitmapBuff)
	{
		CDC* pDestDC = GetDlgItem(IDC_STATIC_BACK)->GetDC();
	}
}


void MakePath(WCHAR *dest, const WCHAR *dir, const WCHAR *file)
{
	size_t	len;

	if ((len = wcslen(dir)) == 0)
	{
		wsprintf(dest, L"%s", file);
	}
	else
		wsprintf(dest, L"%s%s%s", dir, dir[len -1] == L'\\' ? L"" : L"\\" , file);
}



void CStubDlg::ExtractUninstaller()
{
	if (!g_globaldata.install_sh2.uninst.chkIncludeUninstaller)		return;

	//get hdr//
	UINT64 nMarker;
	m_rfile.Read(&nMarker, sizeof(UINT64));
	if(nMarker != MARKER_UNINSTALLER)	return;

	//get size//
	DWORD dwSize;
	m_rfile.Read(&dwSize, sizeof(DWORD));

	//get buffer//
	byte* buf = new byte[dwSize];
	m_rfile.Read(buf, dwSize);

	//get option//
	UNINST_OPTION option;
	m_rfile.Read(&option, sizeof(option));

	//write uninstaller//
	if (!PathFileExists(g_strInstDir))	SHCreateDirectory(NULL, g_strInstDir);

	CString strProductName;
	strProductName = g_globaldata.general_sh1.info.wszProductName;
	

	CString strUnistaller = g_globaldata.install_sh2.advanced.wszUninstallerFileName;
	CString strFullPathName = g_strInstDir + L"\\" + strUnistaller;

	CFile wfile;
	if (ConfirmDiskFreeSpace(dwSize))
	{
		if(wfile.Open(strFullPathName, CFile::modeCreate|CFile::modeWrite))
		{
			wfile.Write(buf, dwSize);
			wfile.Write(&option, sizeof(option));

			wfile.Close();
			WriteUninstallList(TRUE, strFullPathName);
		}
	}	

	delete[] buf;

	////////////////////////////
	//Add to the Control Panel//
	////////////////////////////
	
	if (g_globaldata.install_sh2.uninst.chkOptionsAdd2Programs)
	{
		wchar_t buf[MAX_PATH * 3];
		TRegistry reg(HKEY_LOCAL_MACHINE);
		if (reg.OpenKey(REGSTR_PATH_UNINSTALL))
		{
			if (reg.CreateKey(strProductName))
			{
				MakePath(buf, g_strInstDir, strUnistaller);

				reg.SetStr(REGSTR_VAL_UNINSTALLER_DISPLAYNAME, strProductName);
				reg.SetStr(REGSTR_VAL_UNINSTALLER_COMMANDLINE, buf);
			}
			reg.CloseKey();
		}
	}

	if (g_pLog)		g_pLog->Write(L"extracted uninstaller");
}


void CStubDlg::ExtractUpdater()
{
	if (!g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool)		return;

	//get hdr//
	UINT64 nMarker;
	m_rfile.Read(&nMarker, sizeof(UINT64));
	if(nMarker != MARKER_UPDATER)	return;

	//get size//
	DWORD dwSize;
	m_rfile.Read(&dwSize, sizeof(DWORD));

	//get buffer//
	byte* buf = new byte[dwSize];
	m_rfile.Read(buf, dwSize);

	//write updater//
	if (!PathFileExists(g_strInstDir))	SHCreateDirectory(NULL, g_strInstDir);

	CString strFullPathName = g_strInstDir + L"\\" + L"Updater.exe";

	CFile wfile;
	if (ConfirmDiskFreeSpace(dwSize))
	{
		if(wfile.Open(strFullPathName, CFile::modeCreate|CFile::modeWrite))
		{
			wfile.Write(buf, dwSize);
			wfile.Close();

			WriteUninstallList(TRUE, strFullPathName);
		}
	}	

	delete[] buf;
	
	//create updater.ini file//
	CString strUpaderIni = g_strInstDir + L"\\" + CONFIG_INIFILE;
	save_updateinfo_to_ini(strUpaderIni);

	if (g_pLog)		g_pLog->Write(L"extracted updater.exe and create updater_config.ini file.");
}



void CStubDlg::GetRects()
{
	CRect rect;
	GetClientRect(&rect);
	//logo//
	m_reLogo.left = rect.left;		m_reLogo.right = rect.right;
	m_reLogo.top = 0;				m_reLogo.bottom = rect.Height() / 4 + 30;

	//header//
	m_reHdr.left = rect.left;		m_reHdr.right = rect.right;
	m_reHdr.top = rect.Height() / 4 + 40;				m_reHdr.bottom = rect.Height() / 2;

	//text//
	m_reText.left = rect.left;		m_reText.right = rect.right;
	m_reText.top = rect.Height() / 2;		m_reText.bottom = rect.Height() * 2 / 3;

	//install button//
	m_reButton.left = (rect.Width() - g_templateEditor.nButtonWidth) / 2;		m_reButton.right = (rect.Width() + g_templateEditor.nButtonWidth) / 2;
	m_reButton.top = rect.Height() * 2 / 3 + 10;		m_reButton.bottom = rect.Height() * 5 / 6 - 10;
}

HBRUSH CStubDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_STATIC &&	pWnd->GetSafeHwnd() == m_lnkOption.GetSafeHwnd())
	{
// 		pDC->SetTextColor((COLORREF)g_templateEditor.nLicAndCopyrightLinkColor);
// 		pDC->SetBkMode(TRANSPARENT);
// 		SendMessage(WM_ERASEBKGND, (WPARAM)pDC->m_hDC);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CStubDlg::OnBnClickedSyslinkStubLicenseLink()
{
	// TODO: Add your control notification handler code here
}


void CStubDlg::OnBnClickedSyslinkStubOption()
{
	// TODO: Add your control notification handler code here
}


void CStubDlg::OnBnClickedSyslinkStubCopyrightLink()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"open", g_globaldata.general_sh1.info.wszWebSite, NULL, NULL, SW_NORMAL);
}


LRESULT CStubDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class

	if(message == UM_SHOW_STUB_OPTION)
	{
		CStubOptionDlg dlg;
		if(dlg.DoModal() == IDOK)
		{

		}
		return 0L;
	}
	else if (message == UM_SHOW_STUB_LICENSE)
	{
		CStubLicenseDlg dlg;
		dlg.SetViewLicense(m_pLicBuff);
		if(dlg.DoModal() == IDOK)
		{

		}

		return 0L;
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}



void CStubDlg::UninstallApp()
{
	m_bRun = TRUE;
	//delete files//
	DeleteDirectory(g_mapDirList[L"#INSTALL_DIR#"], TRUE);
	g_pLog->Write(L"delete directory <%s>", g_mapDirList[L"#INSTALL_DIR#"]);

	//delete registry keys//
	UnregisterValues();

	//delete shortcut files//
	DeleteShortcuts();	

	m_ctrlPogExtract.SetPos(100);

	m_strText.Format(L"%s %s has been uninstalled successfully.",
		g_globaldata.general_sh1.info.wszProductName,
		g_globaldata.general_sh1.info.wszProductVersion);
	m_strBtn = L"FINISH";
	m_strHeader.Format(L"Uninstalled");
	m_btnInstall.ShowWindow(SW_SHOW);
	m_ctrlPogExtract.ShowWindow(SW_HIDE);
	DrawWindowComponents(FALSE);

	m_bRun = FALSE;
	m_bComplete = TRUE;

	if (g_pLog)		g_pLog->Write(L"uninstalled");
}

CString CStubDlg::RepairFile(wchar_t* wszDestPathName)
{
	UINT realLen = 0;
	UINT compLen = 0;
	CString strDestFullName = L"";
	//get comp len//
	m_rfile.Read(&compLen, sizeof(compLen));
	if (compLen == 0)//directory
	{
		CString strfullDir;
		GetSaveFileFullPathName(wszDestPathName, strfullDir, TRUE);
		//write//
		if (!PathFileExists(strfullDir))
			SHCreateDirectory(NULL, strfullDir);
		strDestFullName = strfullDir;
	}
	else//file
	{
		//get real len//
		m_rfile.Read(&realLen, sizeof(realLen));

		unsigned char *cprPtr = new unsigned char[compLen];

		//////UnCompress/////
		m_rfile.Read(cprPtr, compLen);

		UINT nLen = 0;
		unsigned char *dataPtr = ZlibUncompress(cprPtr + 4, compLen, realLen, nLen);

		//get save full path name//
		CString strFullPathName, strfullDir;
		strFullPathName = GetSaveFileFullPathName(wszDestPathName, strfullDir);
		//write//
		if (!PathFileExists(strfullDir))
			SHCreateDirectory(NULL, strfullDir);

		CMyFile wfile;
		if (wfile.Open(strFullPathName.GetBuffer(), L"wb"))
		{
			if (wfile.GetFileLength() != nLen)
			{
				wfile.Write(dataPtr, nLen);
			}

			wfile.Close();
		}


		free(cprPtr);
		free(dataPtr);

		cprPtr = NULL;
		dataPtr = NULL;
		strDestFullName = strFullPathName;
	}

	return strDestFullName;
}

DWORD WINAPI thRepairing(LPVOID lParam)
{
	CStubDlg* pStubDlg = (CStubDlg*)lParam;
	pStubDlg->RepairApp();
	return 0;
}

void CStubDlg::RepairApp()
{
	m_bRun = TRUE;
	//get file marker//
	UINT64 nZipOption;
	
	int nRead = m_rfile.Read(&nZipOption, sizeof(nZipOption));
	while (nRead > 0 && nZipOption != MARKER_ZIP)
	{
		nRead = m_rfile.Read(&nZipOption, sizeof(nZipOption));
		Sleep(4);
	}
	
	if (nZipOption != MARKER_ZIP)		
	{
		m_ctrlPogExtract.SetPos(100);

		m_strText.Format(L"Repairing %s %s is failed.",
			g_globaldata.general_sh1.info.wszProductName,
			g_globaldata.general_sh1.info.wszProductVersion);
		m_strBtn = L"FINISH";
		m_strHeader.Format(L"Repairing Failed");
		m_btnInstall.ShowWindow(SW_SHOW);
		m_ctrlPogExtract.ShowWindow(SW_HIDE);
		DrawWindowComponents(FALSE);

		m_bRun = FALSE;
		m_bComplete = TRUE;

		return;
	}

	//////////unzip proc/////////
	UnzipProc();	
}

DWORD WINAPI thUninstalling(LPVOID lParam)
{
	CStubDlg* pStubDlg = (CStubDlg*)lParam;
	pStubDlg->UninstallApp();
	return 0;
}


int CStubDlg::CheckUserInfo()
{
	int nRet = 0;

	CString strtxt;
	if (g_globaldata.interface_sh3.userinfo.chkUserNameRequired)
	{
		if (strUserName.IsEmpty())	return -1;
	}

	if (g_globaldata.interface_sh3.userinfo.chkUserEmailRequired)
	{
		strtxt = strUserEmail;
		if (strtxt.IsEmpty())	return -2;

		int k1, k2;
		k1 = strtxt.Find(L"@");
		k2 = strtxt.ReverseFind(L'.');
		if (k1 <= 0 || k2 <= 0 || k1 >= k2)		return -2;
	}

	if (g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired)
	{
		if (strCompany.IsEmpty())	return -3;
	}

	if (g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired)
	{
		if (strFieldText.IsEmpty())	return -4;
	}

	if (g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired)
	{
		CStringA strA(strSN);

		//get hash value//
		CSHA1 sha1;
		sha1.Reset();
		sha1.Update((BYTE *)strA.GetBuffer(), strA.GetLength());
		sha1.Final();
		BYTE hashbuf[HASHLEN];
		sha1.GetHash(hashbuf);

		//compare//
		bool bSNExist = false;
		if (g_globaldata.interface_sh3.userinfo.nSNCount == 0)
		{
			bSNExist = true;
		}
		else
		{
			for (int i = 0; i < g_globaldata.interface_sh3.userinfo.nSNCount; i++)
			{
				if (memcmp((byte*)g_globaldata.interface_sh3.userinfo.wszSNList + i * HASHLEN,
					hashbuf, HASHLEN) == 0)
				{
					bSNExist = true;
					break;
				}
			}
		}

		if (bSNExist)		nRet = 1;
		else				nRet = -5;
	}

	return nRet;
}

void CStubDlg::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: Add your specialized code here and/or call the base class

	ShellExecute(NULL, L"open", g_globaldata.general_sh1.info.wszSupportPage, NULL, NULL, SW_SHOW);

	//CDialogEx::WinHelp(dwData, nCmd);
}



BOOL bDwonloaderRunning;
void download_prerequisites(wchar_t* wszLink)
{
	bDwonloaderRunning = TRUE;
	CDownloaderDlg1 dlg;
	dlg.SetURL(wszLink);
	dlg.DoModal();
	bDwonloaderRunning = FALSE;
}

int32_t CheckPrerequistes(HWND hWnd, CString& strPrerequisiteName)
{
	if (g_globaldata.install_sh2.advanced.chkAllowInstallFailedPrerequisites)		return 1;
	if (bDwonloaderRunning)		return 0;

	//prerequisites//
	//download and exec, wait//
	CString strMsg;
	if (g_globaldata.general_sh1.prerequisites.chkNetFramework)
	{
		strPrerequisiteName.Format(L".Net Framework %s", g_globaldata.general_sh1.prerequisites.wszNetFramework);
		if (!check_net_framework())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires .Net Framework %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszNetFramework);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszNetFrameworkLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strTmp, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);

				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires .Net Framework %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszNetFramework);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	if (g_globaldata.general_sh1.prerequisites.chkInternetExplorer)
	{
		strPrerequisiteName.Format(L"Internet Explorer %s", g_globaldata.general_sh1.prerequisites.wszInternetExplorer);
		if (!check_internet_explorer())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires Internet Explorer %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszInternetExplorer);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszInternetExplorerLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strTmp, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires Internet Explorer %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszInternetExplorer);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	if (g_globaldata.general_sh1.prerequisites.chkAcrobat)
	{
		strPrerequisiteName.Format(L"Acrobat Reader %s", g_globaldata.general_sh1.prerequisites.wszAcrobatReader);
		if (!check_acrobat_reader())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires Acrobat Reader %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszAcrobatReader);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszAcrobatReaderLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, g_globaldata.general_sh1.prerequisites.wszAcrobatReaderLink, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires Acrobat Reader %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszAcrobatReader);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	if (g_globaldata.general_sh1.prerequisites.chkJava)
	{
		strPrerequisiteName.Format(L"Java %s", g_globaldata.general_sh1.prerequisites.wszJava);
		if (!check_java())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires Java %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszJava);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszJavaLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, g_globaldata.general_sh1.prerequisites.wszJavaLink, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);

				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires Java %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszJava);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}

	}

	if (g_globaldata.general_sh1.prerequisites.chkSqlServerCompact)
	{
		strPrerequisiteName.Format(L"SQL Server Compact %s", g_globaldata.general_sh1.prerequisites.wszSQLServerCompact);
		if (!check_sql_server_compact())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires SQL Server Compact %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszSQLServerCompact);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszSQLServerCompactLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, g_globaldata.general_sh1.prerequisites.wszSQLServerCompactLink, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);

				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires SQL Server Compact %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszSQLServerCompact);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	if (g_globaldata.general_sh1.prerequisites.chkSqlServerExpress)
	{
		strPrerequisiteName.Format(L"SQL Server Express %s", g_globaldata.general_sh1.prerequisites.wszSQLServerExpressLink);
		if (!check_sql_server_express())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires SQL Server Express %s.\r\nWould you like to install it now?", g_globaldata.general_sh1.prerequisites.wszSQLServerExpress);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				CString strLink = g_globaldata.general_sh1.prerequisites.wszSQLServerExpressLink;
				strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
					&& strLink.Find(L"http") == 0)
				{
					//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, g_globaldata.general_sh1.prerequisites.wszSQLServerExpressLink, NULL, SW_SHOWNORMAL);
					download_prerequisites(strLink.GetBuffer());
				}
				else
					ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);

				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires SQL Server Express %s.\r\nThis prerequisite has been already installed on your computer.", g_globaldata.general_sh1.prerequisites.wszSQLServerExpressLink);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86 ||
		g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64 ||
		g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64)
	{
		CString strRequire;
		strRequire.Format(L"");
		if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86)
		{
			strRequire.Format(L"%s(x86)", g_globaldata.general_sh1.prerequisites.wszVCRuntimeX86);
		}
		if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64)
		{
			if (!strRequire.IsEmpty())	strRequire.Append(L", ");
			strRequire.AppendFormat(L"%s(x64)", g_globaldata.general_sh1.prerequisites.wszVCRuntimeX64);
		}
		if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64)
		{
			if (!strRequire.IsEmpty())	strRequire.Append(L", ");
			strRequire.AppendFormat(L"%s(ARM64)", g_globaldata.general_sh1.prerequisites.wszVCRuntimeARM64);
		}

		strPrerequisiteName.Format(L"Visual C++ Runtime %s", strRequire);
		if (!check_vc_runtime())
		{
			//////////////////////////////////////////////////////////
			strMsg.Format(L"This installation requires Visual C++ Runtime %s.\r\nWould you like to install it now?", strRequire);
			if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
				return -1;
			else
			{
				if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86)
				{
					CString strLink = g_globaldata.general_sh1.prerequisites.wszVCRuntimeLinkX86;
					strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
					if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
						&& strLink.Find(L"http") == 0)
					{
						//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strLink, NULL, SW_SHOWNORMAL);
						download_prerequisites(strLink.GetBuffer());
					}
					else
						ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
				}

				if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64)
				{
					CString strLink = g_globaldata.general_sh1.prerequisites.wszVCRuntimeLinkX64;
					strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
					if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
						&& strLink.Find(L"http") == 0)
					{
						//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strLink, NULL, SW_SHOWNORMAL);
						download_prerequisites(strLink.GetBuffer());
					}
					else
						ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
				}

				if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64)
				{
					CString strLink = g_globaldata.general_sh1.prerequisites.wszVCRuntimeLinkARM64;
					strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
					if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
						&& strLink.Find(L"http") == 0)
					{
						//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strLink, NULL, SW_SHOWNORMAL);
						download_prerequisites(strLink.GetBuffer());
					}
					else
						ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
				}

				return 0;
			}
		}
		else
		{
			strMsg.Format(L"This installation requires Visual C++ Runtime %s.\r\nThis prerequisite has been already installed on your computer.", strRequire);
			::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_OK | MB_ICONINFORMATION);
		}
	}

	//additional prerequisites//
	for (INT i = 0; i < g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.size(); i++)
	{
		ADDPREREQUISITE val = g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[i];

		//////////////////////////////////////////////////////////
		strPrerequisiteName.Format(L"%s", val.wszName);
		strMsg.Format(L"This installation requires %s. Please install this application\r\nWould you like to install it now?", val.wszName);
		if (::MessageBox(hWnd, strMsg, INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDNO)
			return -1;
		else
		{
			CString strLink = val.wszCommand;

			if (g_globaldata.general_sh1.prerequisites.nUseDownloaderForPrerequisites
				&& strLink.Find(L"http") == 0)
			{
				//ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, strLink, NULL, SW_SHOWNORMAL);
				download_prerequisites(strLink.GetBuffer());
			}
			else
			{
				if (strLink.Find(L"http") != 0)
				{
					strLink = strLink.Right(strLink.GetLength() - strLink.ReverseFind(L'\\') - 1);
					strLink = L"#SETUP_TEMP_DIR#\\" + strLink;
					strLink.Replace(L"#SETUP_TEMP_DIR#", g_mapDirList[L"#SETUP_TEMP_DIR#"]);
				}

				ShellExecute(NULL, L"open", strLink, NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}

	return 1;
}