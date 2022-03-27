
#include "stdafx.h"
#include "CReadyToInstPage.h"
#include "../../common/other.h"
#include "../../common/common.h"
#include "../../common/zlib/Process.h"
#include "../../common/TRegistry.h"
#include "../../common/regstr.h"

#include "../CDownloaderDlg.h"

CReadyToInstPage* pPage = NULL;

#define  TIMER_NOINTERFACE	2

LRESULT CReadyToInstPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();

	if (g_globaldata.interface_sh3.main.chkOptionsHideFileNames)
	{
		m_editSummary.ShowWindow(SW_HIDE);
	}

	this->InitializeValues();

	return 1;
}

LRESULT CReadyToInstPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;

	//this->UninitializeControls();
	return 0;
}

void CReadyToInstPage::InitializeFont(void)
{
	// Set the font
	CLogFont logFont;
	CClientDC dc(m_hWnd);
	logFont.SetHeight(8, dc);
	::lstrcpy(logFont.lfFaceName, _T("Courier New"));

	m_fontSummary.Attach(logFont.CreateFontIndirect());
	m_editSummary.SetFont(m_fontSummary);

	// Set the tab stops to 4 characters.
	// Tab stops are in dialog units.
	TEXTMETRIC tm = { 0 };
	CFontHandle oldFont = dc.SelectFont(m_fontSummary);
	dc.GetTextMetrics(&tm);
	dc.SelectFont(oldFont);

	int dialogUnitsX = ::MulDiv(4, tm.tmAveCharWidth, LOWORD(GetDialogBaseUnits()));
	int tabStops = 4 * dialogUnitsX;

	m_editSummary.SetTabStops(tabStops);
}

void CReadyToInstPage::InitializeControls(void)
{
	m_editSummary = this->GetDlgItem(IDC_EDIT_WIZ97_INSALL_DETAIL);
	this->InitializeFont();
}

void CReadyToInstPage::UninitializeControls(void)
{
}

void CReadyToInstPage::InitializeValues(void)
{
	
	CString text;
	text.Format(L"Destination Folder:\r\n\t%s\r\n", g_strInstDir);
	m_editSummary.SetWindowText(text);

	m_progWind = this->GetDlgItem(IDC_PROG_INSTALL);
	SendMessage(m_progWind.m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(m_progWind.m_hWnd, PBM_SETPOS, 0, 0);

	m_bRun = FALSE;
	m_bComplete = FALSE;

	m_CprPtr = NULL;
	m_DataPtr = NULL;

	if (!g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install)
		OnWizardNext();

	pPage = this;

	m_bPause = FALSE;
}

bool CReadyToInstPage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CReadyToInstPage::OnSetActive()
{
	if (!m_bRun)
	{
		this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
	else
	{
		this->SetWizardButtons(0);
	}
	

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CReadyToInstPage::OnWizardNext()
{
	if (m_bComplete)		return m_pTestWizardInfo->FindNextPage(IDD);
	
	//main proc//
	if (!m_bRun)
	{
		m_bRun = TRUE;
		m_bComplete = FALSE;

		OnSetActive();

		/*if (!General_Prerequisites())
		{
			m_bRun = FALSE;
			m_bComplete = TRUE;
			return m_pTestWizardInfo->FindNextPage(IDD);
		}
		::SetTimer(this->m_hWnd, TIMER_PREREQUISITES, 300, TimerCheckPrerequisites);*/
		//uninst list param init//
		init_uninstlst();

		StartMainInstall();

		return -1;		
	}
	else
	{
		//pause proc
		return -1;
	}
}

int CReadyToInstPage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CReadyToInstPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}



//***************************//
//***************************//
//*******MAIN PROC*********//
//***************************//
//***************************//
BOOL CReadyToInstPage::ExtractZipFiles()
{
	if (g_pLog)		g_pLog->Write(L"finding zip marker.");
	//get zip marker//
	UINT64 nZipOption;
	g_rfile.Read(&nZipOption, sizeof(nZipOption));

	if (nZipOption != MARKER_ZIP)		
	{
		m_bRun = FALSE;
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
	CReadyToInstPage* pDlg = (CReadyToInstPage*)lParam;
	pDlg->UnzipProc();
	return 0;
}

CMyFile rPackageFile;
void CReadyToInstPage::UnzipProc()
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
			CDownloaderDlg dlg;

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
		bPkgFileOpened = rPackageFile.Open(strPackageFileFullName.GetBuffer(), _T("rb"));		
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
		while(m_bPause)		Sleep(2);

		CString strDestFullName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		if (strDestFullName.Find(L"#SETUP_TEMP_DIR#") >= 0)	continue;
		
		//uncompress//
		BOOL bpkg = FALSE;
		if (g_globaldata.general_sh1.output.nPackageTypeMode != 0 && bPkgFileOpened)			bpkg = TRUE;
		
		strDestFullName = UnZipFile(strDestFullName.GetBuffer(), TRUE, bpkg);
		
		//set file time//
		if (PathFileExists(strDestFullName))
		{
			HANDLE hFile = CreateFile(strDestFullName, GENERIC_ALL, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
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

		if (g_pLog)		g_pLog->Write(L"extracted file and set file times : <%s>.", strDestFullName);

		//progress//
		int nPos = 100 * (i + 1) / g_globaldata.install_sh2.filesandfolders.vecInstallationData.size();
		SendMessage(m_progWind.m_hWnd, PBM_SETPOS, nPos, 0);

		Sleep(2);
	}
	SendMessage(m_progWind.m_hWnd, PBM_SETPOS, 100, 0);
	if (g_pLog)		g_pLog->Write(L"extracted zip files successfully.");
	
	if (bPkgFileOpened)//close package file//
	{
		rPackageFile.Close();
	}

	////uninstaller/////
	ExtractUninstaller();

	//updater//
	ExtractUpdater();

	//regist values//
	if (g_pLog)		g_pLog->Write(L"registering values.");
	RegisterValues();

	m_bRun = FALSE;
}



CString CReadyToInstPage::GetSaveFileFullPathName(wchar_t* wszName, CString& strFullDir, BOOL bDir)
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

	if (nPos < 0)
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


void CReadyToInstPage::CreateShortcuts()
{
	m_editSummary.AppendText(L"Creating shortcuts...\r\n");

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

void CReadyToInstPage::DeleteShortcuts(void)
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


void CReadyToInstPage::CreateLink(CString strLocation, CString strName, CString strTarget)
{
	CString strPathObj;
	CString strPathLink;

	if (g_mapDirList.find(strLocation) == g_mapDirList.end())	return;

	if (strLocation.Find(L"#PROGRAM_GROUP#") >= 0 && !bStartMenu)
	{
		return;
	}
	else if (strLocation.Find(L"#PROGRAM_GROUP#") >= 0 && bStartMenu)
	{
		strPathLink = g_mapDirList[strLocation] + L"\\" + strStartMenuDir + L"\\" + strName + L".lnk";
		if (!PathFileExists(g_mapDirList[strLocation] + L"\\" + strStartMenuDir))
		{
			SHCreateDirectory(NULL, g_mapDirList[strLocation] + L"\\" + strStartMenuDir);
		}
	}
	else if (strLocation.Find(L"#DESKTOP#") >= 0 && !bDesktop)
	{
		return;
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


void CReadyToInstPage::RegisterValues(BOOL bRepair)
{
	if (!bRepair)
	{
		m_editSummary.AppendText(L"Registering...\r\n");
	}	

	//regist general info//
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

void CReadyToInstPage::UnregisterValues()
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

void CALLBACK TimerCallbackFunction(HWND hwnd, UINT, UINT, DWORD)
{
	pPage->OtherProc();	
}

void CReadyToInstPage::OtherProc()
{
	if (!m_bRun)
	{
		KillTimer(0);

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
			m_bComplete = TRUE;
			m_editSummary.AppendText(L"Completed!\r\n");

			if (g_pLog)		g_pLog->Write(L"Installation Completed");

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
			if (g_pLog)		g_pLog->Write(L"Complete Setting Up Wizard Stub.");

			OnSetActive();
		}
		else
		{
			m_bComplete = TRUE;
			if (g_nErr == SINST_DOWNLOAD_PKG_FAILED)
			{
				m_editSummary.AppendText(L"Package downloading failed. Uncompleted!\r\n");
			}
			else if (g_nErr == SINST_PKG_FILE_NOT_OPENED)
			{
				m_editSummary.AppendText(L"Package File can't be read. Uncompleted!\r\n");
			}

			if (g_pLog)		g_pLog->Write(L"uncompleted");

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

			OnSetActive();
		}
	}

	if (!g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install)
	{
		::PostMessage(GetParent().m_hWnd, WM_COMMAND, 0x00003024, (LPARAM)(::GetParent(GetParent().m_hWnd)));
		//if(g_pLog)	g_pLog->Write(L"auto next page.");
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
		wsprintf(dest, L"%s%s%s", dir, dir[len - 1] == L'\\' ? L"" : L"\\", file);
}

void CReadyToInstPage::ExtractUninstaller()
{
	if (!g_globaldata.install_sh2.uninst.chkIncludeUninstaller)		return;

	//get hdr//
	UINT64 nMarker;
	g_rfile.Read(&nMarker, sizeof(UINT64));
	if (nMarker != MARKER_UNINSTALLER)	return;

	//get size//
	DWORD dwSize;
	g_rfile.Read(&dwSize, sizeof(DWORD));

	//get buffer//
	byte* buf = new byte[dwSize];
	g_rfile.Read(buf, dwSize);

	//get option//
	UNINST_OPTION option;
	g_rfile.Read(&option, sizeof(option));

	//write uninstaller//
	if (!PathFileExists(g_strInstDir))	SHCreateDirectory(NULL, g_strInstDir);

	CString strAppName;
	strAppName = g_globaldata.general_sh1.info.wszProductName;


	CString strUnistaller = g_globaldata.install_sh2.advanced.wszUninstallerFileName;
	CString strFullPathName = g_strInstDir + L"\\" + strUnistaller;

	CMyFile wfile;
	if (ConfirmDiskFreeSpace(dwSize))
	{
		if (wfile.Open(strFullPathName.GetBuffer(), L"wb"))
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
			if (reg.CreateKey(strAppName))
			{
				MakePath(buf, g_strInstDir, strUnistaller);

				reg.SetStr(REGSTR_VAL_UNINSTALLER_DISPLAYNAME, strAppName);
				reg.SetStr(REGSTR_VAL_UNINSTALLER_COMMANDLINE, buf);
			}
			reg.CloseKey();
		}
	}
	if (g_pLog)		g_pLog->Write(L"extracted uninstaller");
}

void CReadyToInstPage::ExtractUpdater()
{
	if (!g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool)		return;

	//get hdr//
	UINT64 nMarker;
	g_rfile.Read(&nMarker, sizeof(UINT64));
	if (nMarker != MARKER_UPDATER)	return;

	//get size//
	DWORD dwSize;
	g_rfile.Read(&dwSize, sizeof(DWORD));

	//get buffer//
	byte* buf = new byte[dwSize];
	g_rfile.Read(buf, dwSize);

	//write updater//
	if (!PathFileExists(g_strInstDir))	SHCreateDirectory(NULL, g_strInstDir);

	CString strFullPathName = g_strInstDir + L"\\" + L"Updater.exe";

	CMyFile wfile;
	if (ConfirmDiskFreeSpace(dwSize))
	{
		if (wfile.Open(strFullPathName.GetBuffer(), L"wb"))
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


void CALLBACK TimerCheckPrerequisites(HWND hwnd, UINT, UINT, DWORD)
{
	/*if(CheckPrerequistes())
	{
		KillTimer(hwnd, TIMER_PREREQUISITES);
		pPage->StartMainInstall();
	}*/
}

void CReadyToInstPage::StartMainInstall()
{
	////////condition////////
	General_Requirements();

	//set install dir//
	if (g_mapDirList.find(L"#INSTALL_DIR#") != g_mapDirList.end())
		g_mapDirList[L"#INSTALL_DIR#"] = g_strInstDir;

	m_editSummary.AppendText(L"Extracting zip files...\r\n");
	ExtractZipFiles();
	SetTimer(0, 100, TimerCallbackFunction);
}



CString CReadyToInstPage::UnZipFile(wchar_t* wszDestPathName, BOOL bSetSts, BOOL bPkg)
{
	CMyFile* fp = NULL;
	if (bPkg)
		fp = &rPackageFile;
	else
		fp = &g_rfile;

	UINT realLen = 0;
	UINT compLen = 0;
	CString strDestFullName = L"";
	//get comp len//
	fp->Read(&compLen, sizeof(compLen));
	if (compLen == 0)//directory
	{
		CString strfullDir;
		GetSaveFileFullPathName(wszDestPathName, strfullDir, TRUE);
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
		strFullPathName = GetSaveFileFullPathName(wszDestPathName, strfullDir);
		//write//
		if (!PathFileExists(strfullDir))
			SHCreateDirectory(NULL, strfullDir);

		CMyFile wfile;
		if (wfile.Open(strFullPathName.GetBuffer(), L"wb"))
		{
			if (bSetSts)
			{
				CString strSts;
				strSts.Format(L"%s", strFullPathName);
				m_editSummary.AppendText(L"\t");
				m_editSummary.AppendText(strFullPathName);
				m_editSummary.AppendText(L"\r\n");
			}			

			wfile.Write(m_DataPtr, nLen);
			wfile.Close();

			WriteUninstallList(TRUE, strFullPathName);
		}


		free(m_CprPtr);
		free(m_DataPtr);

		m_CprPtr = NULL;
		m_DataPtr = NULL;
		//if (g_pLog)		g_pLog->Write(L"extracted file <%s>.", strFullPathName);
		strDestFullName = strFullPathName;
	}

	return strDestFullName;
}