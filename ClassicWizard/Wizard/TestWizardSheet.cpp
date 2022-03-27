
#include "stdafx.h"
#include "TestWizardSheet.h"


///////////////////////////////////
#include "../../common/other.h"
#include "../../common/zlib/Process.h"
#include "../../common/TRegistry.h"
#include "../../common/regstr.h"
///////////////////////////////////

#include "../CDownloaderDlg.h"
int32_t CheckPrerequistes(HWND hWnd, CString& strPrerequisiteName);

INT nAlreadyInstalled = FALSE;

CTestWizardSheet::CTestWizardSheet(CTestWizardInfo* pTestWizardInfo, UINT uStartPage, HWND hWndParent) :
	baseClass(_T("Test Wizard"), IDB_WIZ97_HEADER, IDB_WIZ97_WATERMARK, uStartPage, hWndParent),
	infoRefClass(pTestWizardInfo)
{
	//downloader test//
	/*CString strCmd;
	strCmd = L"https://download.microsoft.com/download/5/E/9/5E9B18CC-8FD5-467E-B5BF-BADE39C51F73/SQLServer2017-SSEI-Expr.exe";
	//strCmd = L"https://download.visualstudio.microsoft.com/download/pr/63764a19-21d6-4e7b-b0b1-dddc215143ee/0e7fddbacf4fd9c42e8dd7718e88cd4c/dotnet-sdk-5.0.301-win-x86.zip";
	//strCmd = L"https://javadl.oracle.com/webapps/download/AutoDL?BundleId=245060_d3c52aa6bfa54d3ca74e617f18309292";
	//strCmd = L"https://sdlc-esd.oracle.com/ESD6/JSCDL/jdk/8u301-b09/d3c52aa6bfa54d3ca74e617f18309292/jre-8u301-windows-x64.exe?GroupName=JSC&amp;FilePath=/ESD6/JSCDL/jdk/8u301-b09/d3c52aa6bfa54d3ca74e617f18309292/jre-8u301-windows-x64.exe&amp;BHost=javadl.sun.com&amp;File=jre-8u301-windows-x64.exe&amp;AuthParam=1627901448_4da6466fd5ebf6b6b68f9e0e5e976c58&amp;ext=.exe";

	CDownloaderDlg dlg;
	dlg.SetURL(strCmd.GetBuffer());
	dlg.DoModal();*/

	InitFunction();

	m_pageWelcome.SetTestWizardInfo(pTestWizardInfo);
	m_pageLicAgree.SetTestWizardInfo(pTestWizardInfo);
	m_pageAddInfo.SetTestWizardInfo(pTestWizardInfo);
	m_pageAddTask.SetTestWizardInfo(pTestWizardInfo);
	m_pageAppFolder.SetTestWizardInfo(pTestWizardInfo);
	m_pageReadme.SetTestWizardInfo(pTestWizardInfo);
	m_pageReadyToInst.SetTestWizardInfo(pTestWizardInfo);
	m_pageCompletion.SetTestWizardInfo(pTestWizardInfo);
	m_pageAlreadyInstalled.SetTestWizardInfo(pTestWizardInfo);	
	
	if (g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome)
		this->AddPage(m_pageWelcome);
	if (nAlreadyInstalled == 1)
	{
		this->AddPage(m_pageAlreadyInstalled);
		m_pageAlreadyInstalled.m_pInstaPage = &m_pageReadyToInst;
	}
	else
	{
		if (g_pLog)		g_pLog->Write(L"classic wizard mode");
		if (g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement && g_pLicBuff)
			this->AddPage(m_pageLicAgree);
		if (g_globaldata.interface_sh3.main.chkDialogs2ShowReadme && g_pReadmeBuff)
			this->AddPage(m_pageReadme);
		if (g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation)
			this->AddPage(m_pageAddInfo);
		if (g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder)
			this->AddPage(m_pageAppFolder);
		if (g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks)
			this->AddPage(m_pageAddTask);
		if (1/*g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install*/)
			this->AddPage(m_pageReadyToInst);
	}
	
	if (1/*g_globaldata.interface_sh3.main.chkDialogs2ShowFinish*/)
		this->AddPage(m_pageCompletion);
}

CTestWizardSheet::~CTestWizardSheet()
{
	CloseFunction();
}

/*void CTestWizardSheet::NextPage(BOOL bFinish)
{
	WPARAM wParam;
	if (bFinish)
		wParam = 0x00003025;
	else
		wParam = 0x00003024;

 	PostMessage(WM_COMMAND, wParam, (LPARAM)(GetParent().m_hWnd));
}*/


LRESULT CTestWizardSheet::OnHelp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	// We get here when the user hits F1 while on a page,
	// or uses the "What's This" button then clicks on a control.
	// We can also handle WM_HELP on the page for the cases
	// when a control on the dialog has focus.  If the page doesn't handle WM_HELP,
	// then the sheet is given a chance to handle it (and we end up here).

	ShellExecute(NULL, L"open", g_globaldata.general_sh1.info.wszSupportPage, NULL, NULL, SW_SHOW);

	return 0;
}


LRESULT CTestWizardSheet::OnCancel(WORD, WORD, HWND, BOOL&)
{
	if (m_pageReadyToInst.m_bRun)
		m_pageReadyToInst.m_bPause = TRUE;
	
	CString strMsg;
	strMsg = L"Are you sure you want to cancel the Installation?";
	if(::MessageBox(NULL, strMsg, L"Exit Setup", MB_OKCANCEL) == IDOK)
	{
		g_pLog->Write(L"setup is aborted.");

		if (nAlreadyInstalled == 1)
		{
			RemoveApp();
		}		

		PostQuitMessage(0);
	}

	m_pageReadyToInst.m_bPause = FALSE;

	return 0L;
}

LRESULT CTestWizardSheet::OnBnHelp(WORD, WORD, HWND, BOOL&)
{
	ShellExecute(NULL, L"open", g_globaldata.general_sh1.info.wszSupportPage, NULL, NULL, SW_SHOW);
	return 0L;
}

LRESULT CTestWizardSheet::OnSetWindowTitle(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CString strTitle;
	strTitle.Format(L"SInstaller - %s %s", g_globaldata.general_sh1.info.wszProductName, g_globaldata.general_sh1.info.wszProductVersion);
	SetWindowText(strTitle);
	return 0L;
}


LRESULT CTestWizardSheet::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0L;
}



void CTestWizardSheet::InitFunction()
{
	m_CprPtr = NULL;
	m_DataPtr = NULL;

	m_pWizardBuff = NULL;
	m_pHdrBuff = NULL;
	g_pLicBuff = NULL;
	g_pReadmeBuff = NULL;

	GetModuleFullPathName();
	init_log();

	//read file//
	g_rfile.Open(g_wszModulePathName, _T("rb"));
	
	//get stub size//
	if (g_pLog)		g_pLog->Write(L"finding header.");
	UINT64 nStubSize = 0;
	UINT64 nFileLen = g_rfile.GetFileLength();
	g_rfile.Seek(nFileLen - sizeof(UINT64), SEEK_SET);

	g_rfile.Read(&nStubSize, sizeof(UINT64));

	//seek to global option param//
	g_rfile.Seek(nStubSize, SEEK_SET);

	//get global option param//
	if (g_pLog)		g_pLog->Write(L"finding option marker.");
	GetGlobalOptionParam();

	//if (g_pLog)		g_pLog->Write(L"finding logo bitmaps marker.");
	GetBitmaps();

	if (g_pLog)		g_pLog->Write(L"finding readme and license marker.");
	GetLicAndReadme();

	GetDirNamesAndVariables();

	//extract setup prerequisites exe//
	//ExtractSetupPrerequisites();

	if (g_pLog)		g_pLog->Write(L"classic wizard install mode.");
	
	//extract temp files//
	for (INT i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
	{
		CString strPathName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		if (strPathName.Find(L"#SETUP_TEMP_DIR#") >= 0)
		{
			m_pageReadyToInst.UnZipFile(strPathName.GetBuffer(), FALSE);
			if (g_pLog)		g_pLog->Write(L"extracted temp file : <%s>.", strPathName);
		}			
	}

	//
	//check already installed//
	CString strAlreadyProName, strAlreadyProVer;
	nAlreadyInstalled = CheckAlreadyInstalled(strAlreadyProName, strAlreadyProVer);
	

	//lower version installed//
	if (nAlreadyInstalled == -1)
	{
		CString strMsg;
		strMsg.Format(L"%s %s was already installed on your computer. But this version is %s.\nDo you want to install this low version?",
			strAlreadyProName, strAlreadyProVer, g_globaldata.general_sh1.info.wszProductVersion);
		if (::MessageBox(NULL, strMsg, L"warning", MB_YESNO) == IDNO)
		{
			PostQuitMessage(0);
			return;
		}

		strMsg = L"";
	}

	if (nAlreadyInstalled == 0)//not installed
	{
		BOOL bClose = FALSE;
		int nRet = 0;
		while (1)
		{
			CString strPrereauisiteName;
			nRet = CheckPrerequistes(this->m_hWnd, strPrereauisiteName);

			CString strMsg = L"Please wait for installation of prerequisites.\n\nIf prerequisites are installed successfully, please click Retry to continue.\nOr if you don't want to install this application, please click Cancel to exit.";
			strMsg = L"Please wait for installation of prerequisites.\n\n";
			strMsg.Append(L"If you press Continue, the installer continues without waiting for the pre-requisites to be installed.\n");
			strMsg.Append(L"Or if you press Cancel, it cancels installation of main Installer.");

			if (nRet > 0 || nRet < 0)		break;

			CString strTitle;
			strTitle.Format(L"Installer \"%s\"", strPrereauisiteName);
			UINT nMsgRet = ::MessageBox(this->m_hWnd, strMsg, strTitle, MB_CANCELTRYCONTINUE | MB_ICONWARNING);
			if (nMsgRet == IDCANCEL)
			{
				bClose = TRUE;
				break;
			}
			else if (nMsgRet == IDCONTINUE)
			{
				break;
			}
		}

		if (bClose) 	PostQuitMessage(0);
	}
	
}

void CTestWizardSheet::CloseFunction()
{
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

	if (m_pWizardBuff)
	{
		delete[] m_pWizardBuff;
		m_pWizardBuff = NULL;
	}

	if (m_pHdrBuff)
	{
		delete[] m_pHdrBuff;
		m_pHdrBuff = NULL;
	}

	if (g_pLicBuff)
	{
		delete[] g_pLicBuff;
		g_pLicBuff = NULL;
	}

	if (g_pReadmeBuff)
	{
		delete[] g_pReadmeBuff;
		g_pReadmeBuff = NULL;
	}
}


BOOL CTestWizardSheet::GetGlobalOptionParam()
{
	int k = sizeof(vector<byte>);

	//get MARKER_OPTION
	UINT64 nMarkerOption;
	g_rfile.Read(&nMarkerOption, sizeof(nMarkerOption));
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
		g_rfile.Read(&g_globaldata.general_sh1.info, sizeof(g_globaldata.general_sh1.info));
		//load sh1-pg2
		g_rfile.Read(&g_globaldata.general_sh1.params, sizeof(g_globaldata.general_sh1.params));
		//load sh1-pg3
		g_rfile.Read(&g_globaldata.general_sh1.requires, sizeof(g_globaldata.general_sh1.requires));
		//load sh1-pg4
		g_rfile.Read(&g_globaldata.general_sh1.prerequisites, sizeof(g_globaldata.general_sh1.prerequisites) - VECSIZE);
		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDPREREQUISITE value;
			g_rfile.Read(&value, sizeof(ADDPREREQUISITE));
			g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.push_back(value);
		}
		//load sh1-pg5
		g_rfile.Read(&g_globaldata.general_sh1.output, sizeof(g_globaldata.general_sh1.output));

		/************sheet2*************/
		//load sh2-pg1
		g_rfile.Read(&g_globaldata.install_sh2.filesandfolders, sizeof(g_globaldata.install_sh2.filesandfolders) - VECSIZE);
		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDINSTALLATIONFILES value;
			g_rfile.Read(&value, sizeof(ADDINSTALLATIONFILES));
			g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(value);
		}

		//load sh2-pg2
		g_rfile.Read(&g_globaldata.install_sh2.shortcuts, sizeof(g_globaldata.install_sh2.shortcuts) - VECSIZE);
		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDSHORTCUTDETAIL value;
			g_rfile.Read(&value, sizeof(ADDSHORTCUTDETAIL));
			g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
		}
		//load sh2-pg3
		g_rfile.Read(&g_globaldata.install_sh2.uninst, sizeof(g_globaldata.install_sh2.uninst));
		//load sh2-pg4
		g_rfile.Read(&g_globaldata.install_sh2.updater, sizeof(g_globaldata.install_sh2.updater));
		//load sh2-pg5
		g_rfile.Read(&g_globaldata.install_sh2.advanced, sizeof(g_globaldata.install_sh2.advanced));

		/************sheet3*************/
		//load sh3-pg1
		g_rfile.Read(&g_globaldata.interface_sh3.main, sizeof(g_globaldata.interface_sh3.main));
		//load sh3-pg2
		g_rfile.Read(&g_globaldata.interface_sh3.lang, sizeof(g_globaldata.interface_sh3.lang));
		//load sh3-pg5
		g_rfile.Read(&g_globaldata.interface_sh3.userinfo, sizeof(g_globaldata.interface_sh3.userinfo));
		//load sh3-pg6
		g_rfile.Read(&g_globaldata.interface_sh3.custom, sizeof(g_globaldata.interface_sh3.custom));
		//load sh3-pg7
		g_rfile.Read(&g_globaldata.interface_sh3.finish, sizeof(g_globaldata.interface_sh3.finish));

		/************sheet4*************/
		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDREGISTRYVALUE value;
			g_rfile.Read(&value, sizeof(ADDREGISTRYVALUE));
			g_globaldata.sys_sh4.registry.vecAddRegvalue.push_back(value);
		}


		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDINIVALUE value;
			g_rfile.Read(&value, sizeof(ADDINIVALUE));
			g_globaldata.sys_sh4.inifiles.vecAddInivalue.push_back(value);
		}

		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDVARIABLE value;
			g_rfile.Read(&value, sizeof(ADDVARIABLE));
			g_globaldata.sys_sh4.variables.vecAddVariable.push_back(value);
		}

		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDCOMMAND value;
			g_rfile.Read(&value, sizeof(ADDCOMMAND));
			g_globaldata.sys_sh4.commands.vecAddCommand.push_back(value);
		}

		/************sheet5*************/
		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDEXTENSION value;
			g_rfile.Read(&value, sizeof(ADDEXTENSION));
			g_globaldata.register_sh5.fileassoc.vecAddExtension.push_back(value);
		}

		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			ADDENVIRONMENTVARIABLE value;
			g_rfile.Read(&value, sizeof(ADDENVIRONMENTVARIABLE));
			g_globaldata.register_sh5.envvar.vecAddEnvironmentVariable.push_back(value);
		}

		g_rfile.Read(&nCnt, sizeof(nCnt));
		for (INT i = 0; i < nCnt; i++)
		{
			_REGISTER value;
			g_rfile.Read(&value, sizeof(_REGISTER));
			g_globaldata.register_sh5.others.vecRegister.push_back(value);
		}

		if (g_pLog)		g_pLog->Write(L"found option marker.");
		return TRUE;
	}
}


byte* CTestWizardSheet::UnZipBuffer(bool btext)
{
	byte* pBuff = NULL;

	UINT realLen = 0;
	UINT compLen = 0;

	//get comp len//
	g_rfile.Read(&compLen, sizeof(compLen));
	if (compLen <= 0)	return NULL;

	//get real len//
	g_rfile.Read(&realLen, sizeof(realLen));

	m_CprPtr = new unsigned char[compLen];

	//////UnCompress/////
	g_rfile.Read(m_CprPtr, compLen);

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

void CTestWizardSheet::GetBitmaps()
{
	if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 1)//classic//
	{
		//get wizard marker//
		UINT64 marker;
		g_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_WIZARD_BMP)		return;

		m_pWizardBuff = UnZipBuffer();

		//get hdr marker//
		g_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_HEADER_BMP)		return;

		m_pHdrBuff = UnZipBuffer();

		DrawBackground();
	}
}

void CTestWizardSheet::GetLicAndReadme()
{
	UINT64  marker;

	if (g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement == 1)//license//
	{
		////////get license agreement///////////
		//get marker//
		g_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_LICENSE)
		{
			if (g_pLog)		g_pLog->Write(L"failed license marker.");
			return;
		}

		g_pLicBuff = UnZipBuffer(true);
		if (g_pLog)		g_pLog->Write(L"found license marker.");
	}

	if (g_globaldata.interface_sh3.main.chkDialogs2ShowReadme == 1)//read me//
	{
		////////get readme/////////
		//get marker//
		g_rfile.Read(&marker, sizeof(marker));
		if (marker != MARKER_README)
		{
			if (g_pLog)		g_pLog->Write(L"failed readme marker.");
			return;
		}

		g_pReadmeBuff = UnZipBuffer(true);
		if (g_pLog)		g_pLog->Write(L"found readme marker.");
	}
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

void CTestWizardSheet::DrawBackground()
{
	HBITMAP hbmp;
	if (m_pWizardBuff)
	{
		hbmp = GetHbitmap(m_pWizardBuff);
		this->SetWatermark(hbmp);
	}
	
	if (m_pHdrBuff)
	{
		hbmp = GetHbitmap(m_pHdrBuff);
		this->SetHeader(hbmp);
	}
}


void CTestWizardSheet::RemoveApp()
{
	//delete half-copied files//
	DeleteDirectory(g_mapDirList[L"#INSTALL_DIR#"], TRUE);
	g_pLog->Write(L"delete directory <%s>", g_mapDirList[L"#INSTALL_DIR#"]);

	//delete registry keys//
	m_pageReadyToInst.UnregisterValues();

	g_pLog->Write(L"Unregist all values.");
	//delete shortcut files//
	m_pageReadyToInst.DeleteShortcuts();
	g_pLog->Write(L"Delete all Shortcuts.");
}


BOOL bDwonloaderRunning;
void download_prerequisites(wchar_t* wszLink)
{
	bDwonloaderRunning = TRUE;
	CDownloaderDlg dlg;
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