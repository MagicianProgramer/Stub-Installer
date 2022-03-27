#include "stdafx.h"
#include "../common/common.h"
#include "../common/other.h"
#include "TRegistry.h"
#include <math.h>
#include <Lm.h>
#pragma comment(lib, "netapi32.lib")

/*#include <WinReg.h>
#pragma comment(lib, "advapi32.lib")*/

CMyFile g_rfile;

byte* g_pLicBuff = NULL;
byte* g_pReadmeBuff = NULL;
CLog* g_pLog = NULL;
CLog* g_pUninstLst = NULL;
INT g_nErr = SINST_NO_ERROR;

#define  LOG_FILE		L"log.txt";
void init_log()
{
	if (g_pLog)	return;

	CString strLogFile(g_wszModulePathName);
	strLogFile = strLogFile.Left(strLogFile.ReverseFind(L'\\'));
	strLogFile = strLogFile + L"\\" + LOG_FILE;
	g_pLog = new CLog(strLogFile, true, 1024 * 1024 * 200);
	g_pLog->SetEnabled(TRUE);
	g_pLog->Write(_T("Starting up %s %s Session"), INSTALLER_NAME, INSTALLER_VER);	
}


void close_log()
{
	if (g_pLog)
	{
		delete g_pLog;
		g_pLog = NULL;
	}
}

#include <tlhelp32.h>
/////////////////////Process Find///////////
BOOL IsModuleInProcess(DWORD dwPID, TCHAR * sProcessName)
{
	HANDLE        hModuleSnap = NULL;
	MODULEENTRY32 me32        = {0};
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);

	if ( hModuleSnap == (HANDLE)-1 ) return NULL;

	me32.dwSize = sizeof(MODULEENTRY32);

	if ( Module32First(hModuleSnap, &me32) )
	{
		do
		{
			if ( _wcsicmp(me32.szModule, sProcessName) == 0 ) 
			{
				CloseHandle(hModuleSnap);
				return TRUE;
			}
		}
		while ( Module32Next(hModuleSnap, &me32) );
	}

	CloseHandle(hModuleSnap);
	return FALSE;
}

int ProcessFind(TCHAR * strProcessName, DWORD & dwPid)
{
	HANDLE         hProcessSnap = NULL;
	BOOL           bRet      = FALSE;
	PROCESSENTRY32 pe32      = {0};
	int nRet = 0;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if ( hProcessSnap == (HANDLE)-1 ) return 0;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if ( Process32First(hProcessSnap, &pe32) )
	{
		BOOL          bCurrent = FALSE;
		MODULEENTRY32 me32     = {0};

		do
		{
			if ( IsModuleInProcess(pe32.th32ProcessID, strProcessName) )
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				if ( hProcess )
				{
					CloseHandle(hProcess);
					dwPid = pe32.th32ProcessID;
					nRet++;
				}
			}
		}
		while (Process32Next(hProcessSnap, &pe32));
	}
	CloseHandle (hProcessSnap);

	return nRet;
}


STD_GLOBAL_DATA g_globaldata;
GENERAL_OPTIONS g_generalOptions;
PREREQUISITES_LINKS g_preLinks;
TEMPLATE_EDITOR g_templateEditor;

void SaveProjectOptionsIntoRegistry()
{
	TRegistry reg(INSTALLER_COMPANY, INSTALLER_NAME);
	
	//general options//
	reg.SetInt(L"GOptions_UsingWizard", g_generalOptions.chkUsingWizard);
	reg.SetInt(L"GOptions_OnStartup", g_generalOptions.nOnStartupMode);
	reg.SetInt(L"GOptions_CheckForUpdates", g_generalOptions.chkCheckForUpdates);

	reg.SetInt(L"GOptions_CommonOptions", g_generalOptions.chkCommonOption);
	reg.SetInt(L"GOptions_UpdateFileVersionInfo", g_generalOptions.chkUpdateFileVersionInfo);
	reg.SetInt(L"GOptions_CompressEngineFile", g_generalOptions.chkCompressEngineFile);
	reg.SetInt(L"GOptions_EncryptSensitiveData", g_generalOptions.chkEncryptSensitiveData);
	reg.SetInt(L"GOptions_GetProductVersonFromMainExe", g_generalOptions.chkGetProductVersionFromMainExecutable);

	//file options//
	reg.SetInt(L"FOptions_DoNotAddHiddenFilesAndFolders", g_fileOptions.chkDoNotAddHiddenFilesAndFolders);
	reg.SetInt(L"FOptions_DoNotAddEmptyFolders", g_fileOptions.chkDoNotAddEmptyFolders);
	reg.SetStr(L"FOptions_DoNotAddExtensions", g_fileOptions.wszDoNotAddFilesWithExtensions);
	reg.SetInt(L"FOptions_DoNotShowFilesTreeView", g_fileOptions.chkDoNotShowFilesTreeView);
	reg.SetInt(L"FOptions_UseRelativePaths", g_fileOptions.chkUseRelativePaths);

	reg.SetInt(L"FOptions_SignSetup", g_fileOptions.chkSignSetupFileAfterBuilding);
	reg.SetStr(L"FOptions_SignCommand", g_fileOptions.wszCommand);
	reg.SetInt(L"FOptions_SignUninstallExeEtc", g_fileOptions.chkUninstallUpdaterDownloader);

	//prereauisites link//
	reg.SetStr(L"POptions_DotNet", g_preLinks.wszNetLink);
	reg.SetStr(L"POptions_AcrobatReader", g_preLinks.wszAcrobatLink);
	reg.SetStr(L"POptions_JRE", g_preLinks.wszJAVALink);
	reg.SetStr(L"POptions_SQLCompact", g_preLinks.wszSQLCompactLink);
	reg.SetStr(L"POptions_SQLExpress", g_preLinks.wszSQLExpressLink);
	reg.SetStr(L"POptions_VC", g_preLinks.wszVCRuntimeForX86Link);
	reg.SetInt(L"POptions_DownloadMethod", g_preLinks.nUseDownloader);


	reg.CloseKey();
}


void GetProjectOptionsFromRegistry()
{
	TRegistry reg(INSTALLER_COMPANY, INSTALLER_NAME);

	g_generalOptions.chkUsingWizard = reg.GetInt(L"GOptions_UsingWizard", 1);
	g_generalOptions.chkCommonOption = reg.GetInt(L"GOptions_CommonOptions", 0);
	if (!g_generalOptions.chkCommonOption)
	{
		reg.CloseKey();
		return;
	}

	CString str;

	//general options//
	g_generalOptions.nOnStartupMode = reg.GetInt(L"GOptions_OnStartup", 0);
	g_generalOptions.chkCheckForUpdates = reg.GetInt(L"GOptions_CheckForUpdates", 0);

	
	g_generalOptions.chkUpdateFileVersionInfo = reg.GetInt(L"GOptions_UpdateFileVersionInfo", 0);
	g_generalOptions.chkCompressEngineFile = reg.GetInt(L"GOptions_CompressEngineFile", 0);
	g_generalOptions.chkEncryptSensitiveData = reg.GetInt(L"GOptions_EncryptSensitiveData", 0);
	g_generalOptions.chkGetProductVersionFromMainExecutable = reg.GetInt(L"GOptions_GetProductVersonFromMainExe", 0);

	//file options//
	g_fileOptions.chkDoNotAddHiddenFilesAndFolders = reg.GetInt(L"FOptions_DoNotAddHiddenFilesAndFolders", 0);
	g_fileOptions.chkDoNotAddEmptyFolders = reg.GetInt(L"FOptions_DoNotAddEmptyFolders", 0);
	str = reg.GetStr(L"FOptions_DoNotAddExtensions", L"");
	wsprintf(g_fileOptions.wszDoNotAddFilesWithExtensions, str.GetBuffer());
	g_fileOptions.chkDoNotShowFilesTreeView = reg.GetInt(L"FOptions_DoNotShowFilesTreeView", 0);
	g_fileOptions.chkUseRelativePaths = reg.GetInt(L"FOptions_UseRelativePaths", 0);

	g_fileOptions.chkSignSetupFileAfterBuilding = reg.GetInt(L"FOptions_SignSetup", 0);
	str = reg.GetStr(L"FOptions_SignCommand", L"");
	wsprintf(g_fileOptions.wszCommand, str.GetBuffer());
	g_fileOptions.chkUninstallUpdaterDownloader = reg.GetInt(L"FOptions_SignUninstallExeEtc", 0);

	//prereauisites link//
	str = reg.GetStr(L"POptions_DotNet", L"");
	wsprintf(g_preLinks.wszNetLink, str.GetBuffer());
	str = reg.GetStr(L"POptions_AcrobatReader", L"");
	wsprintf(g_preLinks.wszAcrobatLink, str.GetBuffer());
	str = reg.GetStr(L"POptions_JRE", L"");
	wsprintf(g_preLinks.wszJAVALink, str.GetBuffer());
	str = reg.GetStr(L"POptions_SQLCompact", L"");
	wsprintf(g_preLinks.wszSQLCompactLink, str.GetBuffer());
	str = reg.GetStr(L"POptions_SQLExpress", L"");
	wsprintf(g_preLinks.wszSQLExpressLink, str.GetBuffer());
	str = reg.GetStr(L"POptions_VC", L"");
	wsprintf(g_preLinks.wszVCRuntimeForX86Link, str.GetBuffer());
	g_preLinks.nUseDownloader = reg.GetInt(L"POptions_DownloadMethod", 0);

	reg.CloseKey();
}

FILE_OPTIONS g_fileOptions;
wchar_t g_wszModulePathName[_MAX_INSTALLER_STRING_LEN_];

void GetModuleFullPathName()
{
	GetModuleFileName(NULL, g_wszModulePathName, _MAX_INSTALLER_STRING_LEN_);
}

int8_t  CompareGlobalData(STD_GLOBAL_DATA* pData1, STD_GLOBAL_DATA* pData2)
{
	int8_t ret = 0;
	int32_t		cnt1, cnt2;
	/************sheet1*************/
	//save sh1-pg1
	ret = memcmp(&pData1->general_sh1.info, &pData2->general_sh1.info, sizeof(GENERAL_INFORMATION));
	if(ret)		return 1;

	//save sh1-pg2
	ret = memcmp(&pData1->general_sh1.params, &pData2->general_sh1.params, sizeof(GENERAL_PARAMETERS));
	if(ret)		return 1;

	//save sh1-pg3
	ret = memcmp(&pData1->general_sh1.requires, &pData2->general_sh1.requires, sizeof(GENERAL_REQUIREMENTS));
	if(ret)		return 1;

	//save sh1-pg4
	ret = memcmp(&pData1->general_sh1.prerequisites, &pData2->general_sh1.prerequisites, sizeof(GENERAL_PREREQUISTES) - VECSIZE);
	if(ret)		return 1;
	
	//vector compare//
	cnt1 = pData1->general_sh1.prerequisites.vecAddPrerequisites.size();
	cnt2 = pData2->general_sh1.prerequisites.vecAddPrerequisites.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->general_sh1.prerequisites.vecAddPrerequisites[0],
			&pData2->general_sh1.prerequisites.vecAddPrerequisites[0], cnt1 * sizeof(ADDPREREQUISITE));
		if(ret)		return 1;
	}
	//save sh1-pg5
	ret = memcmp(&pData1->general_sh1.output, &pData2->general_sh1.output, sizeof(GENERAL_OUTPUT));
	if(ret)		return 1;


	/************sheet2*************/
	//save sh2-pg1
	ret = memcmp(&pData1->install_sh2.filesandfolders, &pData2->install_sh2.filesandfolders, sizeof(INSTALLATION_FILESANDFOLDERS) - VECSIZE);
	if(ret)		return 1;
	
	cnt1 = pData1->install_sh2.filesandfolders.vecInstallationData.size();
	cnt2 = pData2->install_sh2.filesandfolders.vecInstallationData.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->install_sh2.filesandfolders.vecInstallationData[0],
			&pData2->install_sh2.filesandfolders.vecInstallationData[0], cnt1 * sizeof(ADDINSTALLATIONFILES));
		if(ret)		return 1;
	}

	//save sh2-pg2
	ret = memcmp(&pData1->install_sh2.shortcuts, &pData2->install_sh2.shortcuts, sizeof(INSTALLATION_SHORTCUTS) - VECSIZE);
	if(ret)		return 1;
	
	cnt1 = pData1->install_sh2.shortcuts.vecDetail.size();
	cnt2 = pData2->install_sh2.shortcuts.vecDetail.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->install_sh2.shortcuts.vecDetail[0],
			&pData2->install_sh2.shortcuts.vecDetail[0], cnt1 * sizeof(ADDSHORTCUTDETAIL));
		if(ret)		return 1;
	}

	//save sh2-pg3
	ret = memcmp(&pData1->install_sh2.uninst, &pData2->install_sh2.uninst, sizeof(INSTALLATION_UNINSTALLATION));
	if(ret)		return 1;

	//save sh2-pg4
	ret = memcmp(&pData1->install_sh2.updater, &pData2->install_sh2.updater, sizeof(INSTALLATION_UPDATER));
	if(ret)		return 1;

	//save sh2-pg5
	ret = memcmp(&pData1->install_sh2.advanced, &pData2->install_sh2.advanced, sizeof(INSTALLATION_ADVANCED));
	if(ret)		return 1;


	/************sheet3*************/
	//save sh3-pg1
	ret = memcmp(&pData1->interface_sh3, &pData2->interface_sh3, sizeof(STD_INTERFACE));
	if(ret)		return 1;


	/************sheet4*************/
	//sh4-pg1
	cnt1 = pData1->sys_sh4.registry.vecAddRegvalue.size();
	cnt2 = pData2->sys_sh4.registry.vecAddRegvalue.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->sys_sh4.registry.vecAddRegvalue[0],
			&pData2->sys_sh4.registry.vecAddRegvalue[0], cnt1 * sizeof(ADDREGISTRYVALUE));
		if(ret)		return 1;
	}

	//sh4-pg2
	cnt1 = pData1->sys_sh4.inifiles.vecAddInivalue.size();
	cnt2 = pData2->sys_sh4.inifiles.vecAddInivalue.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->sys_sh4.inifiles.vecAddInivalue[0],
			&pData2->sys_sh4.inifiles.vecAddInivalue[0], cnt1 * sizeof(ADDINIVALUE));
		if(ret)		return 1;
	}

	//sh4-pg3
	cnt1 = pData1->sys_sh4.variables.vecAddVariable.size();
	cnt2 = pData2->sys_sh4.variables.vecAddVariable.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->sys_sh4.variables.vecAddVariable[0],
			&pData2->sys_sh4.variables.vecAddVariable[0], cnt1 * sizeof(ADDVARIABLE));
		if(ret)		return 1;
	}

	//sh4-pg4
	cnt1 = pData1->sys_sh4.commands.vecAddCommand.size();
	cnt2 = pData2->sys_sh4.commands.vecAddCommand.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->sys_sh4.commands.vecAddCommand[0],
			&pData2->sys_sh4.commands.vecAddCommand[0], cnt1 * sizeof(ADDCOMMAND));
		if(ret)		return 1;
	}


	/************sheet5*************/
	//sh5-pg1
	cnt1 = pData1->register_sh5.fileassoc.vecAddExtension.size();
	cnt2 = pData2->register_sh5.fileassoc.vecAddExtension.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->register_sh5.fileassoc.vecAddExtension[0],
			&pData2->register_sh5.fileassoc.vecAddExtension[0], cnt1 * sizeof(ADDEXTENSION));
		if(ret)		return 1;
	}


	//sh5-pg2
	cnt1 = pData1->register_sh5.envvar.vecAddEnvironmentVariable.size();
	cnt2 = pData2->register_sh5.envvar.vecAddEnvironmentVariable.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->register_sh5.envvar.vecAddEnvironmentVariable[0],
			&pData2->register_sh5.envvar.vecAddEnvironmentVariable[0], cnt1 * sizeof(ADDENVIRONMENTVARIABLE));
		if(ret)		return 1;
	}

	//sh5-pg3
	cnt1 = pData1->register_sh5.others.vecRegister.size();
	cnt2 = pData2->register_sh5.others.vecRegister.size();
	if (cnt1 != cnt2)		return 1;
	else if (cnt1 != 0)
	{
		ret = memcmp(&pData1->register_sh5.others.vecRegister[0],
			&pData2->register_sh5.others.vecRegister[0], cnt1 * sizeof(_REGISTER));
		if(ret)		return 1;
	}

	
	return 0;
}

void CopyGlobalData(STD_GLOBAL_DATA* pDataDst, STD_GLOBAL_DATA* pDataSrc)
{
	/************sheet1*************/
	//save sh1-pg1
	memcpy(&pDataDst->general_sh1.info, &pDataSrc->general_sh1.info, sizeof(GENERAL_INFORMATION));
	//save sh1-pg2
	memcpy(&pDataDst->general_sh1.params, &pDataSrc->general_sh1.params, sizeof(GENERAL_PARAMETERS));
	//save sh1-pg3
	memcpy(&pDataDst->general_sh1.requires, &pDataSrc->general_sh1.requires, sizeof(GENERAL_REQUIREMENTS));
	//save sh1-pg4
	memcpy(&pDataDst->general_sh1.prerequisites, &pDataSrc->general_sh1.prerequisites, sizeof(GENERAL_PREREQUISTES) - VECSIZE);
	pDataDst->general_sh1.prerequisites.vecAddPrerequisites.clear();
	for (int32_t i = 0; i < pDataSrc->general_sh1.prerequisites.vecAddPrerequisites.size(); i++)
	{
		pDataDst->general_sh1.prerequisites.vecAddPrerequisites.push_back(
			pDataSrc->general_sh1.prerequisites.vecAddPrerequisites[i]);
	}
	//save sh1-pg5
	memcpy(&pDataDst->general_sh1.output, &pDataSrc->general_sh1.output, sizeof(GENERAL_OUTPUT));


	/************sheet2*************/
	//save sh2-pg1
	memcpy(&pDataDst->install_sh2.filesandfolders, &pDataSrc->install_sh2.filesandfolders, sizeof(INSTALLATION_FILESANDFOLDERS) - VECSIZE);
	pDataDst->install_sh2.filesandfolders.vecInstallationData.clear();
	for (int32_t i = 0; i < pDataSrc->install_sh2.filesandfolders.vecInstallationData.size(); i++)
	{
		pDataDst->install_sh2.filesandfolders.vecInstallationData.push_back(
			pDataSrc->install_sh2.filesandfolders.vecInstallationData[i]);
	}
	//save sh2-pg2
	memcpy(&pDataDst->install_sh2.shortcuts, &pDataSrc->install_sh2.shortcuts, sizeof(INSTALLATION_SHORTCUTS) - VECSIZE);
	pDataDst->install_sh2.shortcuts.vecDetail.clear();
	for (int32_t i = 0; i < pDataSrc->install_sh2.shortcuts.vecDetail.size(); i++)
	{
		pDataDst->install_sh2.shortcuts.vecDetail.push_back(
			pDataSrc->install_sh2.shortcuts.vecDetail[i]);
	}
	//save sh2-pg3
	memcpy(&pDataDst->install_sh2.uninst, &pDataSrc->install_sh2.uninst, sizeof(INSTALLATION_UNINSTALLATION));
	//save sh2-pg4
	memcpy(&pDataDst->install_sh2.updater, &pDataSrc->install_sh2.updater, sizeof(INSTALLATION_UPDATER));
	//save sh2-pg5
	memcpy(&pDataDst->install_sh2.advanced, &pDataSrc->install_sh2.advanced, sizeof(INSTALLATION_ADVANCED));


	/************sheet3*************/
	//save sh3-pg1
	memcpy(&pDataDst->interface_sh3, &pDataSrc->interface_sh3, sizeof(STD_INTERFACE));


	/************sheet4*************/
	pDataDst->sys_sh4.registry.vecAddRegvalue.clear();
	for (int32_t i = 0; i < pDataSrc->sys_sh4.registry.vecAddRegvalue.size(); i++)
	{
		pDataDst->sys_sh4.registry.vecAddRegvalue.push_back(
			pDataSrc->sys_sh4.registry.vecAddRegvalue[i]);
	}

	pDataDst->sys_sh4.inifiles.vecAddInivalue.clear();
	for (int32_t i = 0; i < pDataSrc->sys_sh4.inifiles.vecAddInivalue.size(); i++)
	{
		pDataDst->sys_sh4.inifiles.vecAddInivalue.push_back(
			pDataSrc->sys_sh4.inifiles.vecAddInivalue[i]);
	}

	pDataDst->sys_sh4.variables.vecAddVariable.clear();
	for (int32_t i = 0; i < pDataSrc->sys_sh4.variables.vecAddVariable.size(); i++)
	{
		pDataDst->sys_sh4.variables.vecAddVariable.push_back(
			pDataSrc->sys_sh4.variables.vecAddVariable[i]);
	}

	pDataDst->sys_sh4.commands.vecAddCommand.clear();
	for (int32_t i = 0; i < pDataSrc->sys_sh4.commands.vecAddCommand.size(); i++)
	{
		pDataDst->sys_sh4.commands.vecAddCommand.push_back(
			pDataSrc->sys_sh4.commands.vecAddCommand[i]);
	}

	/************sheet5*************/
	pDataDst->register_sh5.fileassoc.vecAddExtension.clear();
	for (int32_t i = 0; i < pDataSrc->register_sh5.fileassoc.vecAddExtension.size(); i++)
	{
		pDataDst->register_sh5.fileassoc.vecAddExtension.push_back(
			pDataSrc->register_sh5.fileassoc.vecAddExtension[i]);
	}

	pDataDst->register_sh5.envvar.vecAddEnvironmentVariable.clear();
	for (int32_t i = 0; i < pDataSrc->register_sh5.envvar.vecAddEnvironmentVariable.size(); i++)
	{
		pDataDst->register_sh5.envvar.vecAddEnvironmentVariable.push_back(
			pDataSrc->register_sh5.envvar.vecAddEnvironmentVariable[i]);
	}

	pDataDst->register_sh5.others.vecRegister.clear();
	for (int32_t i = 0; i < pDataSrc->register_sh5.others.vecRegister.size(); i++)
	{
		pDataDst->register_sh5.others.vecRegister.push_back(
			pDataSrc->register_sh5.others.vecRegister[i]);
	}
}

static inline OSVERSIONINFOEX winOsVersion()
{
	OSVERSIONINFOEX result = { sizeof(OSVERSIONINFOEX), 0, 0, 0, 0,{ '\0' }, 0, 0, 0, 0, 0 };

	//typedef NTSTATUS (NTAPI *RtlGetVersionFunction)(LPOSVERSIONINFO);
	
	
	/*HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
	if (ntdll ) {
		auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunction>(GetProcAddress(ntdll, "RtlGetVersion"));

		if (pRtlGetVersion) {
			pRtlGetVersion(reinterpret_cast<LPOSVERSIONINFO>(&result));
		}
	}*/

	return result;
}

//////////sheet 1///////////
void General_Requirements()
{
	//requirements//
	CString strOS;
	if (!CheckOS(strOS))
	{
		CString strRequiredOS = L"Required operating systems are ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_XP)
			strRequiredOS += L"Windows XP, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_VISTA)
			strRequiredOS += L"Windows Vista, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_7)
			strRequiredOS += L"Windows 7, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_8)
			strRequiredOS += L"Windows 8, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_8_1)
			strRequiredOS += L"Windows 8.1, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_10)
			strRequiredOS += L"Windows 10, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2003)
			strRequiredOS += L"Windows Server 2003, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2008)
			strRequiredOS += L"Windows Server 2008, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2008_R2)
			strRequiredOS += L"Windows Server 2008 R2, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2012)
			strRequiredOS += L"Windows Server 2012, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2012_R2)
			strRequiredOS += L"Windows Server 2012 R2, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2016)
			strRequiredOS += L"Windows Server 2016, ";
		if (g_globaldata.general_sh1.requires.nOperatingSystemChk & WINDOWS_SERVER_2019)
			strRequiredOS += L"Windows Server 2019, ";

		strRequiredOS = strRequiredOS.Left(strRequiredOS.ReverseFind(L','));

		CString strMsg;
		strMsg.Format(L"%s\n%s.\nSo you can't run this application.", strOS, strRequiredOS);

		::MessageBox(::GetActiveWindow(), strMsg, _T("Error"), MB_OK | MB_ICONERROR);

		if (g_pLog)		g_pLog->Write(strMsg);
		//not operating system//
		ExitProcess(0);
	}

	if (!CheckOSBit(strOS))
	{
		CString strMsg;
		strMsg.Format(L"%s\n\nSo you can't run this application.", strOS);

		if (g_pLog)		g_pLog->Write(strMsg);
		::MessageBox(::GetActiveWindow(), strMsg, _T("Error"), MB_OK | MB_ICONERROR);

		//not operating system//
		ExitProcess(0);
	}

	if (g_pLog)		g_pLog->Write(L"checked os successfully.");

	if (g_globaldata.general_sh1.requires.chkAdminPrivieges && !IsProcessElevated())//administrator privileges
	{
		CString strMsg;
		strMsg.Format(L"This application requires Administrator Privilege. Do you continue to install this application?");

		if (g_pLog)		g_pLog->Write(L"This application requires Administrator Privilege.");
		if (::MessageBox(::GetActiveWindow(), strMsg, _T("Warning"), MB_YESNO | MB_ICONWARNING) != IDYES)
		{
			ExitProcess(0);
		}		
	}

	//if (g_pLog)		g_pLog->Write(L"checked administrator privileges successfully.");

	if (g_globaldata.general_sh1.requires.chkInternetConnection && !InternetConnected())//internet connection
	{
		CString strMsg;
		strMsg.Format(L"This application requires Internet Connection. Do you continue to install this application?");

		if (g_pLog)		g_pLog->Write(L"This application requires Internet Connection.");
		if (::MessageBox(::GetActiveWindow(), strMsg, _T("Warning"), MB_YESNO | MB_ICONWARNING) != IDYES)
		{
			ExitProcess(0);
		}
	}


	CloseRunningApp1();
	CloseRunningApp2();
}

CString g_strSetupPrerequisitesExePathName = L"";
BOOL General_Prerequisites()
{
	//prerequisites//
	//download and exec, wait//
	if (g_globaldata.general_sh1.prerequisites.chkNetFramework)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszNetLink))
			//if (::MessageBox(NULL, L"You didn't download .Net Framwork. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	if (g_globaldata.general_sh1.prerequisites.chkAcrobat)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszAcrobatLink))
			//if (::MessageBox(NULL, L"You didn't download Adobe Acrobat Reader. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	if (g_globaldata.general_sh1.prerequisites.chkJava)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszJAVALink))
			//if (::MessageBox(NULL, L"You didn't download JAVA. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	if (g_globaldata.general_sh1.prerequisites.chkSqlServerCompact)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszSQLCompactLink))
			//if (::MessageBox(NULL, L"You didn't download SQL Server Compact. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	if (g_globaldata.general_sh1.prerequisites.chkSqlServerExpress)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszSQLExpressLink))
			//if (::MessageBox(NULL, L"You didn't download SQL Server Express. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	if (g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86)
	{
		if (!DowndloadPrerequisites(g_preLinks.wszVCRuntimeForX86Link))
			//if (::MessageBox(NULL, L"You didn't download Visual C++ Runtime. Do you want to continue?", L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
				return FALSE;
	}

	return TRUE;
}

BOOL check_net_framework()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	CString strKey;
	CString strRequireVer;
	strRequireVer.Format(L"%s", g_globaldata.general_sh1.prerequisites.wszNetFramework);
	//.Net Framework Setp//
	strKey = L"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full";
	if (reg.OpenKey(strKey, FALSE))
	{
		//get version//
		CString strVer;
		strVer = reg.GetStr(L"Version", L"");
		if (strVer.Find(strRequireVer) >= 0)	{reg.CloseKey(); return TRUE;}
		if(strRequireVer.Find(L"NET") >= 0)		{reg.CloseKey(); return FALSE;}
		
		//compare//
		strVer.Replace(L".", L"");
		strRequireVer.Replace(L".", L"");
		float fver = _wtoi(strVer.GetBuffer()) / pow(10.0, strVer.GetLength());
		float reqver = _wtoi(strRequireVer.GetBuffer()) / pow(10.0, strRequireVer.GetLength());
		if (fver > reqver)		{reg.CloseKey(); return TRUE;}
	}
	else
	{
		strKey = L"SOFTWARE\\Wow6432Node\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full";
		if (reg.OpenKey(strKey, FALSE))
		{
			//get version//
			CString strVer;
			strVer = reg.GetStr(L"Version", L"");
			if (strVer.Find(strRequireVer) >= 0)	{reg.CloseKey(); return TRUE;}

			//compare//
			strVer.Replace(L".", L"");
			strRequireVer.Replace(L".", L"");
			float fver = _wtoi(strVer.GetBuffer()) / pow(10.0, strVer.GetLength());
			float reqver = _wtoi(strRequireVer.GetBuffer()) / pow(10.0, strRequireVer.GetLength());
			if (fver > reqver)		{reg.CloseKey(); return TRUE;}
		}
	}

	reg.CloseKey();
	
	return FALSE;
}

BOOL check_internet_explorer()
{
	return TRUE;
}

BOOL check_acrobat_reader()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	CString strKey;
	CString strRequireVer;
	strRequireVer.Format(L"%s", g_globaldata.general_sh1.prerequisites.wszAcrobatReader);
	//.Net Framework Setp//
	strKey = L"SOFTWARE\\Adobe\\Adobe Acrobat\\" + strRequireVer;
	if (reg.OpenKey(strKey, FALSE))
	{
		reg.CloseKey();
		return TRUE;
	}
	else
	{
		strKey = L"SOFTWARE\\WOW6432Node\\Adobe\\Adobe Acrobat\\" + strRequireVer;
		if (reg.OpenKey(strKey, FALSE))
		{
			reg.CloseKey();
			return TRUE;
		}
	}
	reg.CloseKey();

	return FALSE;
}

BOOL check_java()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	CString strKey;
	CString strRequireVer;
	strRequireVer.Format(L"%s", g_globaldata.general_sh1.prerequisites.wszJava);
	//.Net Framework Setp//
	strKey = L"SOFTWARE\\JavaSoft\\Java Runtime Environment";
	if (reg.OpenKey(strKey, FALSE))
	{
		CString strVer;
		strVer = reg.GetStr(L"CurrentVersion", L"");
		if (strVer.Find(strRequireVer) >= 0)
		{
			reg.CloseKey();
			return TRUE;
		}		
	}
	else
	{
		strKey = L"SOFTWARE\\WOW6432Node\\JavaSoft\\Java Runtime Environment";
		if (reg.OpenKey(strKey, FALSE))
		{
			CString strVer;
			strVer = reg.GetStr(L"CurrentVersion", L"");
			if (strVer.Find(strRequireVer) >= 0)
			{
				reg.CloseKey();
				return TRUE;
			}
		}
	}
	reg.CloseKey();

	return FALSE;
}

BOOL check_sql_server_compact()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	CString strKey;
	CString strRequireVer;
	strRequireVer.Format(L"%s", g_globaldata.general_sh1.prerequisites.wszSQLServerCompact);

	CString strtmp;
	float f = _wtof(strRequireVer);
	strtmp.Format(L"v%.1f", f);
	//.Net Framework Setp//
	strKey = L"SOFTWARE\\Microsoft\\Microsoft SQL Server Compact Edition\\" + strtmp;
	if (reg.OpenKey(strKey, FALSE))
	{
		reg.CloseKey();
		return TRUE;
	}

	reg.CloseKey();

	return FALSE;
}

BOOL check_sql_server_express()
{
	return FALSE;

	CString str = ExecCmd("wmic product get name,version");
	CString strFind;
	strFind.Format(L"SQL Server %s", g_globaldata.general_sh1.prerequisites.wszSQLServerExpress);
	if (str.Find(strFind) > 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL check_vc_runtime()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	CString strKey;
	CString strRequireVer;
	strRequireVer.Format(L"%s", g_globaldata.general_sh1.prerequisites.wszVCRuntimeX86);
	DWORD dwBId = 0;
	if (strRequireVer.Find(L"2019") >= 0)		dwBId = 27820;
	else if (strRequireVer.Find(L"2017") >= 0)		dwBId = 23026;
	else if (strRequireVer.Find(L"2015") >= 0)		dwBId = 26020;

	//.Net Framework Setp//
	strKey = L"SOFTWARE\\Microsoft\\VisualStudio\\14.0\\VC\\Runtimes";
	if (reg.OpenKey(strKey, FALSE))
	{
		reg.CloseKey();
		return TRUE;
	}

	reg.CloseKey();

	return FALSE;
}




BOOL DowndloadPrerequisites(wchar_t* wszURL)
{
	CString strFile = wszURL;
	//strFile = strFile.Right(strFile.GetLength() - strFile.ReverseFind(L'/'));
	CString strMsg = L"This installation requires " + strFile + L".\r\nWould you like to install it now?";
	if(::MessageBox(NULL, strMsg, L"Warnning", MB_YESNO | MB_ICONWARNING) == IDNO)
	{
		return FALSE;
	}

	ShellExecute(NULL, L"open", g_strSetupPrerequisitesExePathName, wszURL, NULL, SW_SHOWNORMAL);

	return TRUE;
}

BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor)
{
	static DWORD dwMajorCache = 0, dwMinorCache = 0;
	if (0 != dwMajorCache)
	{
		dwMajor = dwMajorCache;
		dwMinor = dwMinorCache;
		return TRUE;
	}

	LPWKSTA_INFO_100 pBuf = NULL;
	if (NERR_Success != NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf))
		return FALSE;

	dwMajor = dwMajorCache = pBuf->wki100_ver_major;
	dwMinor = dwMinorCache = pBuf->wki100_ver_minor;
	NetApiBufferFree(pBuf);

	return TRUE;
}

BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor, DWORD& dwServicePack)
{
	if (!GetWindowsVersion(dwMajor, dwMinor))
		return FALSE;

	static DWORD dwServicePackCache = ULONG_MAX;
	if (ULONG_MAX != dwServicePackCache)
	{
		dwServicePack = dwServicePackCache;
		return TRUE;
	}

	const int nServicePackMax = 10;
	OSVERSIONINFOEX osvi;
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, VER_EQUAL);

	for (int i = 0; i < nServicePackMax; ++i)
	{
		osvi.wServicePackMajor = i;
		if (VerifyVersionInfo(&osvi, VER_SERVICEPACKMAJOR, dwlConditionMask))
		{
			dwServicePack = dwServicePackCache = i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CheckOS(CString& strOS)
{
	//check operating system version//
	DWORD dwMajorVersion = 0, dwMinorVersion = 0;
	GetWindowsVersion(dwMajorVersion, dwMinorVersion);

	/////////////////////////
	//OSVERSIONINFOEX OSversion;
	//OSversion = winOsVersion();
	int os;
	strOS = L"Your operating system is ";
	if (dwMajorVersion == 10)//win10
	{
		os = WINDOWS_10 | WINDOWS_SERVER_2016 | WINDOWS_SERVER_2019;
		strOS += L"Windows 10.";
	}
	else if (dwMajorVersion == 6)
	{
		if (dwMinorVersion == 3)
		{
			os = WINDOWS_8_1 | WINDOWS_SERVER_2012_R2;
			strOS += L"Windows 8.1.";
		}
		else if (dwMinorVersion == 2)
		{
			os = WINDOWS_8 | WINDOWS_SERVER_2012;
			strOS += L"Windows 8.";
		}
		else if (dwMinorVersion == 1)
		{
			os = WINDOWS_7 | WINDOWS_SERVER_2008_R2;
			strOS += L"Windows 7.";
		}
		else if (dwMinorVersion == 0)
		{
			os = WINDOWS_VISTA | WINDOWS_SERVER_2008;
			strOS += L"Windows Vista.";
		}
	}
	else if (dwMajorVersion < 6)
	{
		if (dwMajorVersion == 5 && dwMinorVersion == 2)
		{
			os = WINDOWS_SERVER_2003;
			strOS += L"Windows Server 2003.";
		}
		else if (dwMajorVersion == 5 && dwMinorVersion == 1)
		{
			os = WINDOWS_XP;
			strOS += L"Windows XP.";
		}
	}

	if (!(os & g_globaldata.general_sh1.requires.nOperatingSystemChk))
		return FALSE;


	return TRUE;
}

BOOL CheckOSBit(CString& strBit)
{
	strBit = L"Your operating system is ";
	//check wow64//
	if (g_globaldata.general_sh1.requires.nOperatingSystemMode == 0)//32 & 64 bit
	{
		strBit += L"";
		return TRUE;
	}
	else if (g_globaldata.general_sh1.requires.nOperatingSystemMode == 1)//64 bit
	{
		strBit += L"32-bit.\nBut this application requires 64-bit operating system.";
		if (TIsWow64())		return TRUE;
		else				return FALSE;
	}
	else if (g_globaldata.general_sh1.requires.nOperatingSystemMode == 2)//32 bit
	{
		strBit += L"64-bit.\nBut this application requires 64-bit operating system.";
		if (TIsWow64())		return FALSE;
		else				return TRUE;
	}

	return TRUE;
}

void CloseRunningApp1()
{
	DWORD dwPid;
	int nCnt = ProcessFind(g_globaldata.general_sh1.requires.wszCloseRunnigApp1_File, dwPid);
	if (nCnt > 0)
	{
		HANDLE	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		BOOL bRet = TerminateProcess(hProc, 0);
		if (g_pLog)		g_pLog->Write(L"closed running application : %s", g_globaldata.general_sh1.requires.wszCloseRunnigApp1_File);
	}
}

void CloseRunningApp2()
{
	DWORD dwPid;
	int nCnt = ProcessFind(g_globaldata.general_sh1.requires.wszCloseRunnigApp2_File, dwPid);
	if (nCnt > 0)
	{
		HANDLE	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		TerminateProcess(hProc, 0);
		if (g_pLog)		g_pLog->Write(L"closed running application : %s", g_globaldata.general_sh1.requires.wszCloseRunnigApp2_File);
	}
}


BOOL ConfirmDiskFreeSpace(UINT64 len)
{
	if (g_globaldata.install_sh2.advanced.chkAllowInstallNoFreeSpace)
		return TRUE;

	if (g_pLog)		g_pLog->Write(L"checking for free space.");

	DWORD dwSecsPerCluster = 0;
	DWORD dwBytesPerSector = 0;
	DWORD dwFreeClusters = 0;
	DWORD dwTotalClusters = 0;

	CString strDrive = g_strInstDir.Left(g_strInstDir.Find(L"\\"));
	if(GetDiskFreeSpace(strDrive, &dwSecsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwTotalClusters))
	{
		UINT64 freespace = 0;
		freespace = (UINT64)dwSecsPerCluster * (UINT64)dwBytesPerSector * (UINT64)dwFreeClusters;

		if (freespace > len)			return TRUE;
	}
	else
	{
		CString str;
		DWORD dw = GetLastError();
		str.Format(L"Get Disk Free Space Failed : error = %d", dw);
		g_pLog->Write(str);
	}

	return FALSE;
}

int save_updateinfo_to_ini(CString strFile)
{
	CString strTmp;
	//product details//
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_NAME, g_globaldata.general_sh1.info.wszProductName, strFile);
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_MAIN_EXE, g_globaldata.install_sh2.filesandfolders.wszMainExecutable, strFile);
	strTmp.Format(L"%d", 2);
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_GET_CURRENT_VER_METHOD, strTmp, strFile);
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_GET_CURRENT_VER_TEXT,  g_globaldata.general_sh1.info.wszProductVersion, strFile);
	strTmp.Format(L"%d", 1);
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_APP_INSTALLED_FOR_ALL_USERS, strTmp, strFile);
	strTmp.Format(L"%d", 0);
	WritePrivateProfileString(CONFIG_SECTION, KEY_PRODUCT_DETAILS_APP_IS_64BIT, strTmp, strFile);

	//advanced options//
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkProductNameInUpdaterTitle);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_PRODUCT_NAME_IN_TITLE, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkAllowUsersToChangeTheUpdaterOptions);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_ALLOW_USERS_TO_CHANGE_OPTION, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkUseDownloader);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_USE_DOWNLOADER, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkAutomaticallyCheckForUpdates);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_AUTO_CHECK_FOR_UPDATE, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nAutoCheckMethod);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_AUTO_CHECK_METHOD, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkInstallUpdatesSilently);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_INSTALL_UPDATES_SILETLY, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nChkUseSmartNotification);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_SMART_NOTIFICATION_METHOD, strTmp, strFile);
	strTmp.Format(L"%d", g_globaldata.install_sh2.updater.nSmartNotificationMethod);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_SMART_NOTIFICATION_METHOD, strTmp, strFile);

	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_ADDITIONAL_PARAMS, g_globaldata.install_sh2.updater.wszAdditionalParams, strFile);
	WritePrivateProfileString(CONFIG_SECTION, KEY_ADVANCED_OPTION_PASS_PARAMS_TO_NEW_VERSION_URL, g_globaldata.install_sh2.updater.wszPassParamsToNewVersionURL, strFile);


	//update file information//
	WritePrivateProfileString(CONFIG_SECTION, KEY_UPDATE_INFORMATION_FILE_UPDATE_TEXT_URL, g_globaldata.install_sh2.updater.wszUpdateInformationFileURL, strFile);


	return 1;
}


CString ExecCmd(const char* cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	} catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);

	CString strResult(result.c_str());
	return strResult;
}


BOOL IsProcessElevated()
{
	BOOL fIsElevated = FALSE;
	HANDLE hToken = NULL;
	TOKEN_ELEVATION elevation;
	DWORD dwSize;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
		printf("\n Failed to get Process Token :%d.", GetLastError());
		
		if (hToken)
		{
			CloseHandle(hToken);
			hToken = NULL;
		}

		return FALSE;
	}
	

	if (!GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
	{
		printf("\nFailed to get Token Information :%d.", GetLastError());
		
		if (hToken)
		{
			CloseHandle(hToken);
			hToken = NULL;
		}

		return FALSE;
	}

	fIsElevated = elevation.TokenIsElevated;

	if (hToken)
	{
		CloseHandle(hToken);
		hToken = NULL;
	}
	return fIsElevated;
}


BOOL InternetConnected()
{
	 BOOL bRet = InternetCheckConnection(L"https://www.google.com", FLAG_ICC_FORCE_CONNECTION, 0);
	 return bRet;
}

CString GetRealPathName(wchar_t* wszPathName)
{
	CString strRet = L"";
	strRet.Format(L"%s", wszPathName);
	map<CString, CString>::iterator iter;
	for (iter = g_mapDirList.begin(); iter != g_mapDirList.end(); iter++)
		strRet.Replace(iter->first, iter->second);

	return strRet;
}


void FinishTab()
{
	//launch application//
	if (g_globaldata.interface_sh3.finish.chkLaunchApp)
	{
		CString strOpen = L"open";
		if (g_globaldata.interface_sh3.finish.chkLaunchAppRunAsAdmin)		strOpen = L"runas";
		
		CString strExec;
		strExec = GetRealPathName(g_globaldata.interface_sh3.finish.wszLaunchAppFile);
		strExec.Replace(L"#", L"");
		ShellExecute(NULL, strOpen, strExec, NULL, NULL, SW_SHOW);
	}

	//launch custom app//
	if (g_globaldata.interface_sh3.finish.chkCustomAction)
	{
		CString strOpen = L"open";
		if (g_globaldata.interface_sh3.finish.chkCustomActionRunAsAdmin)		strOpen = L"runas";

		CString strExec;
		strExec = GetRealPathName(g_globaldata.interface_sh3.finish.wszCustomActionCommand);
		strExec.Replace(L"#", L"");
		ShellExecute(NULL, strOpen, strExec, NULL, NULL, SW_SHOW);
	}

	//restart computer//
	if (g_globaldata.interface_sh3.finish.chkRestartComputer)
	{
		if(::MessageBox(::GetActiveWindow(), L"You have to restart your PC, before you use this application.", L"Warning", MB_YESNO) == IDYES)
			system("shutdown /r /t 0");
	}
	
}

void RegisterValue(CString strKey, CString strValue, CString strData, CString strBitness, CString strType, int nOverwrite = 1, int nRomveOnUninstall = 1)
{
	//replace key and data//
	map<CString, CString>::iterator iter;
	for (iter = g_mapDirList.begin(); iter != g_mapDirList.end(); iter++)
	{
		strKey.Replace(iter->first, iter->second);
		strValue.Replace(iter->first, iter->second);
		strData.Replace(iter->first, iter->second);
	}


	//get key//
	HKEY hKey;
	CString strHKEY = strKey.Left(strKey.Find(L'\\'));
	if (strHKEY == L"HKEY_CLASSES_ROOT")
		hKey = HKEY_CLASSES_ROOT;
	else if (strHKEY == L"HKEY_CURRENT_USER")
		hKey = HKEY_CURRENT_USER;
	else if (strHKEY == L"HKEY_LOCAL_MACHINE")
		hKey = HKEY_LOCAL_MACHINE;
	else if (strHKEY == L"HKEY_USERS")
		hKey = HKEY_USERS;
	else if (strHKEY == L"HKEY_PERFORMANCE_DATA")
		hKey = HKEY_PERFORMANCE_DATA;
	else if (strHKEY == L"HKEY_PERFORMANCE_TEXT")
		hKey = HKEY_PERFORMANCE_TEXT;
	else if (strHKEY == L"HKEY_PERFORMANCE_NLSTEXT")
		hKey = HKEY_PERFORMANCE_NLSTEXT;
	else if (strHKEY == L"HKEY_CURRENT_CONFIG")
		hKey = HKEY_CURRENT_CONFIG;
	else if (strHKEY == L"HKEY_DYN_DATA")
		hKey = HKEY_DYN_DATA;
	else if (strHKEY == L"HKEY_CURRENT_USER_LOCAL_SETTINGS")
		hKey = HKEY_CURRENT_USER_LOCAL_SETTINGS;
	else
		return;

	TRegistry reg(hKey);

	//get strOpenKey//
	CString strOpenKey;
	strOpenKey = strKey;
	strOpenKey.Replace(strHKEY + L"\\", L"");

	BOOL bl = reg.OpenKey(strOpenKey, TRUE);
	if (bl)
	{
		//set value//
		strData.Replace(L"#", L"");
		bl = reg.SetStr(strValue, strData);
	}	

	if (bl)
	{
		if (g_pLog)		g_pLog->Write(L"Registered : %s - %s - %s", strKey, strValue, strData);
	}
	else
	{
		if (g_pLog)		g_pLog->Write(L"Register Failed : %s - %s - %s", strKey, strValue, strData);
	}


	bl = reg.CloseKey();
}


void SystemRegistry()
{
	for (INT i = 0; i < g_globaldata.sys_sh4.registry.vecAddRegvalue.size(); i++)
	{
		ADDREGISTRYVALUE val = g_globaldata.sys_sh4.registry.vecAddRegvalue[i];
		RegisterValue(val.wszRegistry_key,
			val.wszRegistry_value,
			val.wszValue_data,
			val.wszBitness,
			val.wszValuetype,
			val.chkOverwrite,
			val.chkRemove);		
	}
}


void SystemVariable()
{
	for (INT i = 0; i < g_globaldata.sys_sh4.variables.vecAddVariable.size(); i++)
	{
		ADDVARIABLE val = g_globaldata.sys_sh4.variables.vecAddVariable[i];

		//create variable//
		CString strRegKey;
		strRegKey.Format(L"%s\\%s", val.wszRegistryKey, val.wszVariable);
		strRegKey.Replace(L"<", L"");
		strRegKey.Replace(L">", L"");

		RegisterValue(strRegKey, val.wszRegistryValue, val.wszDefault_data, val.wszBitness, val.wszValuetype);
	}
}



void RegisterFileAssociations()
{
	for (INT i = 0; i < g_globaldata.register_sh5.fileassoc.vecAddExtension.size(); i++)
	{
		ADDEXTENSION val = g_globaldata.register_sh5.fileassoc.vecAddExtension[i];

		CString strExt(val.wszExtension);
		CString strValue;
		CString strKey;
		CString strtmp;

		//CLASSES_ROOT//
		CString strExtFile;
		strExtFile.Format(L"%sfile", strExt);
		//regist extension//
		::RegSetKeyValue(HKEY_CLASSES_ROOT, L"." + strExt, 0, REG_SZ, (const BYTE *)strExtFile.GetBuffer(), (wcslen(strExtFile.GetBuffer()) + 1) * 2);

		//create ext file//
		strValue.Format(L"%s", val.wszDescription);
		map<CString, CString>::iterator iter;
		for (iter = g_mapDirList.begin(); iter != g_mapDirList.end(); iter++)
			strValue.Replace(iter->first, iter->second);
		
		//ext
		strKey = strExtFile;
		::RegSetKeyValue(HKEY_CLASSES_ROOT, strKey, 0, REG_SZ, (const BYTE *)strValue.GetBuffer(), (wcslen(strValue.GetBuffer()) + 1) * 2);

		//icon
		strKey = strExtFile + L"\\DefaultIcon";
		strValue.Format(L"%s", val.wszIconFile);
		for (iter = g_mapDirList.begin(); iter != g_mapDirList.end(); iter++)
			strValue.Replace(iter->first, iter->second);
		strValue.AppendFormat(L",%d", val.nIconIndex);
		::RegSetKeyValue(HKEY_CLASSES_ROOT, strKey, 0, REG_SZ, (const BYTE *)strValue.GetBuffer(), (wcslen(strValue.GetBuffer()) + 1) * 2);

		//shell
		strKey = strExtFile + L"\\shell\\open\\command";
		strValue.Format(L"%s", val.wszAssociatedFiles);
		for (iter = g_mapDirList.begin(); iter != g_mapDirList.end(); iter++)
			strValue.Replace(iter->first, iter->second);
		strValue.AppendFormat(L" \"%%1\"");
		::RegSetKeyValue(HKEY_CLASSES_ROOT, strKey, 0, REG_SZ, (const BYTE *)strValue.GetBuffer(), (wcslen(strValue.GetBuffer()) + 1) * 2);

		if (g_pLog) g_pLog->Write(L"registered file association : ext = %s, app = %s", val.wszExtension, strValue);
	}
}



void init_uninstlst()
{
	if (g_pUninstLst)	return;

	CString strLogFile(g_wszModulePathName);
	if (!PathFileExists(g_mapDirList[L"#INSTALL_DIR#"]))
	{
		SHCreateDirectory(NULL, g_mapDirList[L"#INSTALL_DIR#"]);
	}

	strLogFile = g_mapDirList[L"#INSTALL_DIR#"] + L"\\" + UNINSTLST;
	g_pUninstLst = new CLog(strLogFile, false, 1024 * 1024 * 200);
	g_pUninstLst->SetEnabled(TRUE);
	g_pUninstLst->SetUnsinstLst(TRUE);
}


void close_uninstlst()
{
	if (g_pUninstLst)
	{
		delete g_pUninstLst;
		g_pUninstLst = NULL;
	}
}

void WriteUninstallList(BOOL bFile, CString strFullPath)
{
	if (!g_pUninstLst)		return;

	if (bFile) // file
	{
		g_pUninstLst->Write(L"%s", strFullPath);
	}
	else // registry
	{
		g_pUninstLst->Write(L"r:%s", strFullPath);
	}
}