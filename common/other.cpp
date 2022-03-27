
#include "stdafx.h"
#include "../common/other.h"
#include "../common/TRegistry.h"
#include "../common/regstr.h"
#include "../common/common.h"

#include <propvarutil.h>
#include <propkey.h>
#pragma comment(lib, "propsys.lib")
#include <netfw.h>

map<CString, CString> g_mapDirList;
CString g_strInstDir = L"";

BOOL TIsUserAnAdmin()
{
	static BOOL	once = FALSE;
	static BOOL(WINAPI *pIsUserAnAdmin)(void);
	static BOOL	ret = FALSE;

	if (!once)
	{
		pIsUserAnAdmin = (BOOL(WINAPI *)(void))
			GetProcAddress(::GetModuleHandle(L"shell32"), "IsUserAnAdmin");
		if (pIsUserAnAdmin)
		{
			ret = pIsUserAnAdmin();
		}
		once = TRUE;
	}
	return	ret;
}

BOOL TIsEnableUAC()
{
	static BOOL once = FALSE;
	static BOOL ret = FALSE;

	if (!once) {
		once = TRUE;
	}
	return	ret;
}

BOOL TIsWow64()
{
	static BOOL	once = FALSE;
	static BOOL	ret = FALSE;

	if (!once) {
		BOOL(WINAPI *pIsWow64Process)(HANDLE, BOOL *) = (BOOL(WINAPI *)(HANDLE, BOOL *))
			GetProcAddress(::GetModuleHandle(L"kernel32"), "IsWow64Process");
		if (pIsWow64Process) {
			pIsWow64Process(::GetCurrentProcess(), &ret);
		}
		once = TRUE;
	}
	return ret;
}


CString GetProgramFiles(BOOL bX86)
{
	CString strRes = L"";
	BOOL bWow64 = TIsWow64();

	TRegistry reg(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_SETUP))
	{
		CString strProgramFiles;

		if (!bWow64)
		{
			strProgramFiles.Format(L"%s", REGSTR_PROGRAMFILES);
		}
		else
		{
			if (bX86)
			{
				strProgramFiles.Format(L"%s", REGSTR_PROGRAMFILESX86);
			}
			else
			{
				strProgramFiles.Format(L"%s", REGSTR_PROGRAMW6432DIR);
			}
		}		

		strRes = reg.GetStr(strProgramFiles, L"");

		reg.CloseKey();
	}

	return strRes;
}


CString GetDefaultInstallDir()
{
	CString strRes = L"";
	BOOL bWow64 = TIsWow64();

	TRegistry reg(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_SETUP))
	{
		CString strProgramFiles;

		if (bWow64)
			strProgramFiles.Format(L"%s", REGSTR_PROGRAMFILESX86);
		else
			strProgramFiles.Format(L"%s", REGSTR_PROGRAMFILES);

		CString strBuf = reg.GetStr(strProgramFiles, L"");
		strRes.Format(L"%s\\%s", strBuf, g_globaldata.general_sh1.info.wszProductName);

		reg.CloseKey();
	}

	return strRes;
}

CString GetTempDir()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Local AppData");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s\\%s", strBuf, L"Temp");
	}

	return strRes;
}

CString GetSetupTempDir()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Local AppData");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s\\%s", strBuf, L"Temp\\StubTMP");

		/*INT i = 0;
		while(PathFileExists(strRes))
		{
		i++;
		strRes.Format(L"%s(%d)", strRes, i);
		}*/

		reg.CloseKey();
	}

	return strRes;
}

CString GetMyDocuments()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Personal");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetMyPictures()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"My Pictures");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetMyMusic()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"My Music");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetAppData()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"AppData");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetLocalAppData()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Local AppData");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetUserProfile()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Personal");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);

		INT k = strRes.ReverseFind(L'\\');
		strRes = strRes.Left(k);
	}

	return strRes;
}

CString GetTemplates()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Templates");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetFavorites()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Favorites");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetSentTo()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"SendTo");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetDesktop()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Desktop");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetStartMenu()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Start Menu");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}


CString GetProgramsMenu()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Programs");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

CString GetStartUp()
{
	CString strRes = L"";
	TRegistry reg(HKEY_CURRENT_USER);
	if (reg.OpenKey(REGSTR_SHELLFOLDERS))
	{
		CString strFile;

		strFile.Format(L"%s", L"Startup");

		CString strBuf = reg.GetStr(strFile, L"");
		strRes.Format(L"%s", strBuf);
	}

	return strRes;
}

void DeleteDirectory(CString szDir, BOOL bDeleteSelf, int *pCnt)
{
	szDir += "\\";
	WIN32_FIND_DATA ds; 
	CString szAim = szDir;
	szAim += "*";
	HANDLE hDir = FindFirstFile(szAim,&ds);
	if(hDir==INVALID_HANDLE_VALUE)
	{
		FindClose(hDir);
		return;
	}

	CString strPath;
	do 
	{
		if(ds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(((CString)ds.cFileName != (CString)L".") && ((CString)ds.cFileName != (CString)L".."))
			{

				CString szSubDir = szDir;
				szSubDir += ds.cFileName;
				if (pCnt)
					(*pCnt)++;
				else
					DeleteDirectory(szSubDir, TRUE, pCnt);

			}
		}
		else
		{
			strPath = szDir + ds.cFileName;

			if (pCnt)
				(*pCnt)++;
			else
				DeleteFile(strPath);
		}

	} while(FindNextFile(hDir,&ds)==TRUE);


	FindClose(hDir);


	if (bDeleteSelf == TRUE)
	{
		if (pCnt)
			(*pCnt)++;
		else
			RemoveDirectory(szDir);
	}

	return;
}


void GetDirNamesAndVariables()
{
	typedef pair <CString, CString> Map_DirList_Pair;
	CString strListDir, strDir;	

	//SetupTempDir//
	strListDir = L"#SETUP_TEMP_DIR#";
	strDir = GetSetupTempDir();
	strDir.AppendFormat(L"\\TMP_XXX");
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//ProgramFiles//
	strListDir = L"#PROGRAM_FILES#";
	strDir = GetProgramFiles(FALSE);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//program files x86//
	strListDir = L"#PROGRAM_FILES_X86#";
	strDir = GetProgramFiles(TRUE);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//WindowsDir//
	strListDir = L"#SYSTEM_DIR#";
	strDir = L"C:\\Windows";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//SystemDir//
	strListDir = L"#WINDOWS_DIR#";
	if (TIsWow64())
		strDir = L"C:\\Windows\\Syswow64";
	else
		strDir = L"C:\\Windows\\System32";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//TempDir//
	strListDir = L"#TEMP_DIR#";
	strDir = GetTempDir();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//FontsDir//
	strListDir = L"#FONTS_DIR#";
	strDir = L"C:\\Windows\\Fonts";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//MyDocuments//
	strListDir = L"#MY_DOCUMENTS#";
	strDir = GetMyDocuments();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//SystemDrive//
	strListDir = L"#SYSTEM_DRIVE#";
	strDir = L"C:\\";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CurrentDir//
	strListDir = L"#CURRENT_DIR#";
	strDir = GetDefaultInstallDir();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//AllUserProfile//
	strListDir = L"#ALL_USER_PROFILE#";
	strDir = L"C:\\Users\\Public";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//LocalAppData//
	strListDir = L"#LOCAL_APP_DATA#";
	strDir = GetLocalAppData();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//UserProfile//
	strListDir = L"#USER_PROFILE#";
	strDir = GetUserProfile();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//AppData//
	strListDir = L"#APP_DATA#";
	strDir = GetAppData();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//LocalSettings//
	strListDir = L"#LOCAL_SETTINGS#";
	strDir = GetLocalAppData();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonFiles//
	strListDir = L"#COMMON_FILES#";
	strDir = L"C:\\Program Files\\Common Files";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//Templates//
	strListDir = L"#TEMPLATES#";
	strDir = GetTemplates();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//Favorites//
	strListDir = L"#FAVORITES#";
	strDir = GetFavorites();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonAppData//
	strListDir = L"#COMMON_APP_DATA#";
	strDir = L"C:\\ProgramData";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonMyDocuments//
	strListDir = L"#COMMON_MY_DOCUMENTS#";
	strDir = L"C:\\Users\\Public\\Documents";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonTemplates//
	strListDir = L"#COMMON_TEMPLATES#";
	strDir = L"C:\\ProgramData\\Microsoft\\Windows\\Templates";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonFavorites//
	strListDir = L"#COMMON_FAVORITES#";
	strDir = GetFavorites();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//SentTo//
	strListDir = L"#SENT_TO#";
	strDir = GetSentTo();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//ProgramGroup//
	strListDir = L"#PROGRAM_GROUP#";
	strDir = L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//Desktop//
	strListDir = L"#DESKTOP#";
	strDir = GetDesktop();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//StartMenu//
	strListDir = L"#START_MENU#";
	strDir = GetStartMenu();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//ProgramsMenu//
	strListDir = L"#PROGRAMS_MENU#";
	strDir = GetProgramsMenu();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//StartUp//
	strListDir = L"#START_UP#";
	strDir = GetStartUp();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//QuickLaunch//
	strListDir = L"#QUICK_LAUNCH#";
	strDir = GetUserProfile() + L"\\AppData\\Roaming\\Microsoft\\Internet Explorer\\Quick Launch";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonDesktop//
	strListDir = L"#COMMON_DESKTOP#";
	strDir = L"C:\\Users\\Public\\Desktop";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonStartMenu//
	strListDir = L"#COMMON_START_MENU#";
	strDir = L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonProgramsMenu//
	strListDir = L"#COMMON_PROGRAMS_MENU#";
	strDir = L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CommonStartUp//
	strListDir = L"#COMMON_START_UP#";
	strDir = L"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup";
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//AppName//
	strListDir = L"#APPLICATION_NAME#";
	strDir = g_globaldata.general_sh1.info.wszProductName;
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//AppVersion//
	strListDir = L"#APPLICATION_VERSION#";
	strDir = g_globaldata.general_sh1.info.wszProductVersion;
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//AppNameVersion//
	strListDir = L"#APPLICATION_NAME_VERSION#";
	strDir.Format(L"%s %s", g_globaldata.general_sh1.info.wszProductName, g_globaldata.general_sh1.info.wszProductVersion);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//GUID//
	strListDir = L"#GUID#";
	strDir.Format(L"%s", g_globaldata.general_sh1.params.wszProductGUID);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//CompanyName//
	strListDir = L"#COMPANY_NAME#";
	strDir.Format(L"%s", g_globaldata.general_sh1.info.wszCompanyName);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//WebSite//
	strListDir = L"#WEB_SITE#";
	strDir.Format(L"%s", g_globaldata.general_sh1.info.wszWebSite);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//SupportLink//
	strListDir = L"#SUPPORT_LINK#";
	strDir.Format(L"%s", g_globaldata.general_sh1.info.wszSupportPage);
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));

	//get install dir//
	strListDir = L"#INSTALL_DIR#";
	strDir = GetInstallDirectory();
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));
	g_strInstDir = strDir;

	//get installer default dir//
	strListDir = L"#INSTALLER_DEFAULT_DIR#";
	strDir = g_wszModulePathName;
	strDir = strDir.Left(strDir.ReverseFind(L'\\'));
	g_mapDirList.insert(Map_DirList_Pair(strListDir, strDir));
}

CString GetInstallDirectory()
{
	CString strInstallDir = g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory;
	INT k1 = 0, k2 = 1;

	while(1)
	{
		k1 = strInstallDir.Find(L"#");
		k2 = strInstallDir.Find(L"#", k1 + 1);
		if (k1 >= k2)	break;
		CString strtmp = strInstallDir.Mid(k1, k2 - k1 + 1);
		strInstallDir.Replace(strtmp, g_mapDirList[strtmp]);
	}

	return strInstallDir;
}


BOOL ShellLink(LPCTSTR wsrc, LPCTSTR wdest, LPCTSTR warg, LPCTSTR app_id)
{
	IShellLink		*shellLink;
	IPersistFile	*persistFile;
	BOOL			ret = FALSE;

	// failed in thread
	if (SUCCEEDED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void **)&shellLink))) {
		shellLink->SetPath(wsrc);
		shellLink->SetArguments(warg);

		shellLink->SetWorkingDirectory(wsrc);

		if (app_id && *app_id) {
			IPropertyStore *pps;
			HRESULT hr = shellLink->QueryInterface(IID_PPV_ARGS(&pps));

			if (SUCCEEDED(hr)) {
				PROPVARIANT pv;
				hr = InitPropVariantFromString(app_id, &pv);
				if (SUCCEEDED(hr)) {
					pps->SetValue(PKEY_AppUserModel_ID, pv);
					pps->Commit();
					PropVariantClear(&pv);
				}
			}
			if (pps)
				pps->Release();
		}


		if (SUCCEEDED(shellLink->QueryInterface(IID_IPersistFile, (void **)&persistFile))) {
			if (SUCCEEDED(persistFile->Save(wdest, TRUE))) {
				ret = TRUE;
				::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATHW | SHCNF_FLUSH, wdest, NULL);
			}
			persistFile->Release();
		}
		shellLink->Release();
	}
	return	ret;
}


void SetStartup()
{
	CString strMainExe;
	strMainExe = g_globaldata.install_sh2.filesandfolders.wszMainExecutable;
	if (strMainExe.IsEmpty())	return;

	strMainExe.Right(strMainExe.GetLength() - strMainExe.ReverseFind(L'\\') - 1);
	strMainExe = g_strInstDir + L"\\" + strMainExe;

	TRegistry reg(HKEY_LOCAL_MACHINE);
	reg.CreateKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	reg.SetStr(g_globaldata.general_sh1.info.wszProductName, strMainExe);
	reg.CloseKey();
}


void SetInfoIntoRegistry()
{
	//reg general information//
	CString strCompany, strKeyName;
	strCompany = g_globaldata.general_sh1.info.wszCompanyName;
	strKeyName.Format(L"%s %s", g_globaldata.general_sh1.info.wszProductName, g_globaldata.general_sh1.info.wszProductVersion);

	TRegistry reg(strCompany, strKeyName);
	
	reg.SetStr(PRODUCT_NAME, g_globaldata.general_sh1.info.wszProductName);
	reg.SetStr(PRODUCT_VERSION, g_globaldata.general_sh1.info.wszProductVersion);
	reg.SetStr(PRODUCT_DESCRIPTION, g_globaldata.general_sh1.info.wszProductDescription);
	reg.SetStr(PRODUCT_SUPPORT_PAGE, g_globaldata.general_sh1.info.wszSupportPage);
	reg.SetStr(PRODUCT_COMPANY_NAME, g_globaldata.general_sh1.info.wszCompanyName);
	reg.SetStr(PRODUCT_WEBSITE, g_globaldata.general_sh1.info.wszWebSite);
	reg.SetStr(INSTALL_PATH, g_strInstDir);
	reg.CloseKey();
	
	if (g_pLog)		g_pLog->Write(L"set main registry infomation into HKEY_CURRENT_USER\\SOFTWARE\\%s\\%s", strCompany, strKeyName);

	//regist product GUID//
	reg.ChangeTopKey(HKEY_CLASSES_ROOT);
	reg.CreateKey(g_globaldata.general_sh1.params.wszProductGUID);
	reg.SetStr(L"ProductName", g_globaldata.general_sh1.info.wszProductName);
	reg.SetStr(L"ProductVer", g_globaldata.general_sh1.info.wszProductVersion);
	reg.SetStr(L"CompanyName", g_globaldata.general_sh1.info.wszCompanyName);
	reg.SetStr(L"WebSite", g_globaldata.general_sh1.info.wszWebSite);
	reg.CloseKey();
	if (g_pLog)		g_pLog->Write(L"set product guid : HKEY_CLASSES_ROOT\\%s", g_globaldata.general_sh1.params.wszProductGUID);
}

void DeleteInfoFromRegistry()
{
	//delete general information//
	CString strCompany;
	strCompany = g_globaldata.general_sh1.info.wszCompanyName;

	TRegistry reg(HKEY_CURRENT_USER);
	reg.OpenKey(L"SOFTWARE\\" + strCompany);

	CString strKey;
	strKey.Format(L"%s %s", g_globaldata.general_sh1.info.wszProductName, g_globaldata.general_sh1.info.wszProductVersion);
	reg.DeleteKey(strKey);
	reg.CloseKey();

	//delete product GUID//
	reg.ChangeTopKey(HKEY_CLASSES_ROOT);
	if (reg.OpenKey(g_globaldata.general_sh1.params.wszProductGUID))
	{
		reg.DeleteValue(L"ProductName");
		reg.DeleteValue(L"ProductVer");
		reg.DeleteValue(L"CompanyName");
		reg.DeleteValue(L"WebSite");
	}
	reg.CloseKey();
}


void RegisterOthers(char* szfile, BOOL bReg)
{
	CStringA strCmd;

	if (bReg)
		strCmd.Format("regsvr32 -s \"%s\"", szfile);
	else
		strCmd.Format("regsvr32 -u \"%s\"", szfile);

	/*strCmd.Format("%s", szfile);
	strCmd = strCmd.Right(4);
	strCmd.MakeUpper();
	if(strCmd.Find(".DLL") >= 0)
	{
		//dll//
		if (bReg)
			strCmd.Format("regsvr32 \"%s\"", szfile);
		else
			strCmd.Format("regsvr32 -u \"%s\"", szfile);		
	}
	else
	{
		//ocx//
				
	}*/

	system(strCmd);
}

void SystemCommands()
{
	for (INT i = 0; i < g_globaldata.sys_sh4.commands.vecAddCommand.size(); i++)
	{
		CString strFile(g_globaldata.sys_sh4.commands.vecAddCommand[i].wszfile);
		CString strDir = strFile.Left(strFile.Find(L'\\'));
		strFile.Replace(strDir, g_mapDirList[strDir]);

		ShellExecute(NULL, L"open",
			strFile, 
			g_globaldata.sys_sh4.commands.vecAddCommand[i].wszParameters,
			NULL, SW_SHOWNORMAL);
	}
}

void SystemIniFiles()
{
	for (INT i = 0; i < g_globaldata.sys_sh4.inifiles.vecAddInivalue.size(); i++)
	{
		ADDINIVALUE val;
		val = g_globaldata.sys_sh4.inifiles.vecAddInivalue[i];

		CString strFile(val.wszInifile);
		CString strDir = strFile.Left(strFile.Find(L'\\'));
		strFile.Replace(strDir, g_mapDirList[strDir]);

		if(PathFileExists(strFile))
		{
			WritePrivateProfileString(val.wszSection, 
				val.wszKey, 
				val.wszValue, 
				strFile);
		}	
		else
		{
			strFile.Format(L"%s\\%s", g_mapDirList[L"#INSTALL_DIR#"], strFile);
			HANDLE hFile = CreateFile(strFile, GENERIC_READ, 0, 0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
			CloseHandle(hFile);

			WritePrivateProfileString(val.wszSection, 
				val.wszKey, 
				val.wszValue, 
				strFile);
		}

		if (g_pLog)		g_pLog->Write(L"set ini value : file name = %s, section = %s, key = %s, value = %s",
			strFile, val.wszSection, val.wszKey, val.wszValue);
	}
}


void SetRegKeyValue(int valuetype, CString strKey, int nValue, CString strValue)
{
	CString strCompany;
	strCompany = g_globaldata.general_sh1.info.wszCompanyName;

	TRegistry reg(strCompany, g_globaldata.general_sh1.info.wszProductName);
	if (valuetype == 0)//int
	{
		reg.SetInt(strKey, nValue);
	}
	else if (valuetype == 1)//string
	{
		reg.SetStr(strKey, strValue);
	}

	reg.CloseKey();
}


void GetParentDir(const wchar_t *srcfile, wchar_t* buf)
{
	CString str;
	str.Format(L"%s", srcfile);

	int k = str.ReverseFind(L'\\');
	str.Delete(k, str.GetLength() - k);

	wcscpy(buf, str.GetBuffer());
}

BOOL DeleteLink(LPCTSTR path)
{
	wchar_t	dir[MAX_PATH * 3];

	if (!DeleteFile(path))
		return	FALSE;

	GetParentDir(path, dir);
	::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH|SHCNF_FLUSH, dir, NULL);

	return	TRUE;
}

CString GetFolder(LPCTSTR wszPath)
{
	CString strPath(wszPath);

	int pos = strPath.ReverseFind(L'\\');
	return strPath.Left(pos);
}

BOOL DeleteLinkFolder(LPCTSTR path)
{
	WIN32_FIND_DATAW	fdat;

	HANDLE	fh = FindFirstFileW(CString(path) + L"\\*.*", &fdat);
	if (fh == INVALID_HANDLE_VALUE) {
		return	FALSE;
	}
	do {
		if (_wcsicmp(L".", fdat.cFileName) == 0 || _wcsicmp(L"..", fdat.cFileName) == 0) {
			continue;
		}
		CString buf;
		buf = CString(path) + L"\\" + fdat.cFileName;
		DeleteFile(buf);
	} while (FindNextFile(fh, &fdat));

	::FindClose(fh);

	RemoveDirectory(path);

	::SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_PATH|SHCNF_FLUSH, GetFolder(path), NULL);

	return	TRUE;
}



INT CheckAlreadyInstalled(CString& strProName, CString& strProVer)
{
	//return 0 : not installed, 1 : installed, -1 : lower version
	INT nInstalled = 0;
	if (!g_globaldata.general_sh1.params.chkOptionsUninstallPrevious)
	{
		nInstalled = 0;
		return nInstalled;
	}

	//check product guid//
	TRegistry reg(HKEY_CLASSES_ROOT);

	BOOL bl = reg.OpenKey(g_globaldata.general_sh1.params.wszProductGUID);
	if (bl)
	{
		strProName = reg.GetStr(L"ProductName", L"");
		strProVer = reg.GetStr(L"ProductVer", L"");
		if (wcscmp(strProName.GetBuffer(), g_globaldata.general_sh1.info.wszProductName) == 0)
		{
			if (wcscmp(strProVer.GetBuffer(), g_globaldata.general_sh1.info.wszProductVersion) > 0)
			{
				nInstalled = -1;
			}
			else if (wcscmp(strProVer.GetBuffer(), g_globaldata.general_sh1.info.wszProductVersion) == 0)
			{
				nInstalled = 1;
			}			
		}
	}
	reg.CloseKey();

	return nInstalled;
}