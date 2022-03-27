
#include "stdafx.h"
#include "CAlreadyInstalledPage.h"
#include "CReadyToInstPage.h"
#include "../../common/common.h"
#include "../../common/zlib/Process.h"
#include "../../common/other.h"

#include "../CDownloaderDlg.h"

BOOL bRepair = TRUE;

LRESULT CAlreadyInstalledPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();
	this->InitializeValues();
	
	return 1;
}

LRESULT CAlreadyInstalledPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;
	//this->UninitializeControls();
	return 0;
}


void CAlreadyInstalledPage::InitializeControls(void)
{
	CheckDlgButton(IDC_RADIO_ALREADY_INSTALLED_REPAIR, BST_CHECKED);
	
	m_progBar = this->GetDlgItem(IDC_PROGRESS_ALREADY_INSTALLED);
	SendMessage(m_progBar.m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
	SendMessage(m_progBar.m_hWnd, PBM_SETPOS, 0, 0);
}

void CAlreadyInstalledPage::UninitializeControls(void)
{
}

void CAlreadyInstalledPage::InitializeValues(void)
{
	m_bWorking = FALSE;
	m_bStart = FALSE;

	CString strNotice;
	strNotice.Format(L"%s is already installed.", g_globaldata.general_sh1.info.wszProductName);
	strNotice += L"\r\nIf you want to remove this application, please select uninstall mode.";
	GetDlgItem(IDC_STATIC_ALREADY_INSTALLED_NOTICE).SetWindowText(strNotice);
}

bool CAlreadyInstalledPage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CAlreadyInstalledPage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);	

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
		
	return 0;
}

CString GetSaveFileFullPathName(wchar_t* wszName, CString& strFullDir, BOOL bDir = FALSE)
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

CMyFile rPackageFile1;
CString RepairFile(wchar_t* wszDestPathName, BOOL bSetSts = TRUE, BOOL bPkg = FALSE)
{
	CMyFile* fp = NULL;
	if (bPkg)
		fp = &rPackageFile1;
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

		unsigned char *cprPtr = new unsigned char[compLen];

		//////UnCompress/////
		fp->Read(cprPtr, compLen);

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
			wfile.Write(dataPtr, nLen);
			wfile.Close();

			WriteUninstallList(TRUE, strFullPathName);
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
	CAlreadyInstalledPage* pPage = (CAlreadyInstalledPage*)lParam;

	//get file marker//
	UINT64 nZipOption;
	int nRead = g_rfile.Read(&nZipOption, sizeof(nZipOption));
	while (nRead > 0 && nZipOption != MARKER_ZIP)
	{
		nRead = g_rfile.Read(&nZipOption, sizeof(nZipOption));
		Sleep(4);
	}
	

	if (nZipOption != MARKER_ZIP)		
	{
		SendMessage(pPage->m_progBar.m_hWnd, PBM_SETPOS, 100, 0);

		pPage->SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Completed.");
		pPage->m_bWorking = FALSE;

		return 0;
	}

	/////////////////////////////////
	///////////unzip proc////////////
	/////////////////////////////////

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
				pPage->m_bWorking = FALSE;
				pPage->SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Package downloading failed.");
				return 0;
			}
		}
		else
		{
			strPackageFileFullName.AppendFormat(L"\\%s", g_globaldata.general_sh1.output.wszDataFileName);
		}

		//open package file//
		bPkgFileOpened = rPackageFile1.Open(strPackageFileFullName.GetBuffer(), _T("rb"));
		if (!bPkgFileOpened)
		{
			g_nErr = SINST_PKG_FILE_NOT_OPENED;
			if (g_pLog)		g_pLog->Write(L"package file(%s) can't be read.", strPackageFileFullName);
			pPage->m_bWorking = FALSE;
			return 0;
		}
	}


	for (INT i = 0; i < nFileCnt; i++)
	{
		Sleep(2);

		CString strDestFullName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		if (strDestFullName.Find(L"#SETUP_TEMP_DIR#") >= 0)	continue;

		//uncompress//
		BOOL bpkg = FALSE;
		if (g_globaldata.general_sh1.output.nPackageTypeMode != 0 && bPkgFileOpened)			bpkg = TRUE;

		strDestFullName = RepairFile(strDestFullName.GetBuffer(), TRUE, bpkg);

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
		SendMessage(pPage->m_progBar.m_hWnd, PBM_SETPOS, nPos, 0);

		Sleep(2);
	}
	SendMessage(pPage->m_progBar.m_hWnd, PBM_SETPOS, 100, 0);
	if (g_pLog)		g_pLog->Write(L"extracted zip files successfully.");

	if (bPkgFileOpened)//close package file//
	{
		rPackageFile1.Close();
	}

	////uninstaller/////
	pPage->m_pInstaPage->ExtractUninstaller();

	//updater//
	pPage->m_pInstaPage->ExtractUpdater();

	//regist values//
	if (g_pLog)		g_pLog->Write(L"registering values.");
	pPage->m_pInstaPage->RegisterValues(TRUE);


	/////////////////////////////////
	///////////other proc////////////
	/////////////////////////////////
	//create shortcuts//
	if (g_pLog)		g_pLog->Write(L"creating shortcuts.");
	for (INT i = 0; i < g_globaldata.install_sh2.shortcuts.vecDetail.size(); i++)
	{
		CString strLocation, strName, strTarget;
		strLocation.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszLocation);
		strName.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszShortcutName);
		strTarget.Format(L"%s", g_globaldata.install_sh2.shortcuts.vecDetail[i].wszTargetFile);

		if (g_mapDirList.find(strName) != g_mapDirList.end())
			strName = g_mapDirList[strName];


		CString strPathObj;
		CString strPathLink;

		if (g_mapDirList.find(strLocation) == g_mapDirList.end())	continue;

		if (strLocation.Find(L"#PROGRAM_GROUP#") >= 0 && !bStartMenu)
		{
			continue;
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
			continue;
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
	

	pPage->SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Completed.");
	pPage->m_bWorking = FALSE;

	if (g_pLog)		g_pLog->Write(L"repaired");

	return 0;
}

DWORD WINAPI thUninstalling(LPVOID lParam)
{
	CAlreadyInstalledPage* pPage = (CAlreadyInstalledPage*)lParam;

	//delete files//
	DeleteDirectory(g_mapDirList[L"#INSTALL_DIR#"], TRUE);
	g_pLog->Write(L"delete directory <%s>", g_mapDirList[L"#INSTALL_DIR#"]);

	//delete registry keys//
	pPage->m_pInstaPage->UnregisterValues();

	//delete shortcut files//
	pPage->m_pInstaPage->DeleteShortcuts();	
	
	SendMessage(pPage->m_progBar.m_hWnd, PBM_SETPOS, 100, 0);

	pPage->SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Completed.");
	pPage->m_bWorking = FALSE;

	if (g_pLog)		g_pLog->Write(L"uninstalled");

	return 0;
}

int CAlreadyInstalledPage::OnWizardNext()
{
	if (!m_bStart)
	{
		m_bStart = TRUE;
		m_bWorking = TRUE;
		DWORD dwThid;

		int k = IsDlgButtonChecked(IDC_RADIO_ALREADY_INSTALLED_REPAIR);
		if (k > 0)
		{
			init_uninstlst();
			CreateThread(NULL, 0, thRepairing, this, 0, &dwThid);
			SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Repairing...");
			bRepair = TRUE;
		}
		else
		{
			CString strMsg;
			strMsg = L"Are you sure you want to uninstall this application?";
			if(::MessageBox(NULL, strMsg, L"Uninstall", MB_OKCANCEL) == IDOK)
			{
				CreateThread(NULL, 0, thUninstalling, this, 0, &dwThid);
				SetDlgItemText(IDC_STATIC_ALREADY_INSTALLED_STS, L"Uninstalling...");
				bRepair = FALSE;
			}			
		}	

		return - 1;
	}

	if (m_bWorking)
	{
		return -1;
	}

	return m_pTestWizardInfo->FindNextPage(IDD);
}

int CAlreadyInstalledPage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CAlreadyInstalledPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}
