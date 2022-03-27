#include "stdafx.h"
#include "uninst.h"
#include "../common/TRegistry.h"
#include "../common/regstr.h"

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


UNINST_OPTION g_UninstOption;

wchar_t g_wszModulePathName[_MAX_INSTALLER_STRING_LEN_];

CString g_strDeleteDir = L"";
CString g_strProductName = L"";
CString g_strProductVersion = L"";
CString g_strInstallPath = L"";

void GetModuleFullPathName()
{
	GetModuleFileName(NULL, g_wszModulePathName, _MAX_INSTALLER_STRING_LEN_);
}

void CloseMainExecutable()
{
	DWORD dwPid;
	int nCnt = ProcessFind(g_UninstOption.wszMainExecutableName, dwPid);
	if (nCnt > 0)
	{
		HANDLE	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
		BOOL bRet = TerminateProcess(hProc, 0);
	}
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

void DeleteShortcut(void)
{
	TRegistry	reg(HKEY_CURRENT_USER);
	CString		dir;
	CString		path;

	if (!reg.OpenKey(REGSTR_SHELLFOLDERS)) {
		return;
	}

	CString strAppName = g_UninstOption.wszMainExecutableName;
	if (!strAppName.IsEmpty())
	{
		strAppName = strAppName.Left(strAppName.ReverseFind(L'.'));

		dir = reg.GetStr(REGSTR_STARTUP, L"");
		if (dir != L"") {
			path = dir + L"\\" + strAppName + L".lnk";
			DeleteLink(path);
		}
		dir = reg.GetStr(REGSTR_DESKTOP, L"");
		if (dir != L"") {
			path = dir + L"\\" + strAppName + L".lnk";
			DeleteLink(path);
		}
		dir = reg.GetStr(REGSTR_PROGRAMS, L"");
		if (dir != L"") {
			path =  dir + L"\\" + strAppName;
			DeleteLinkFolder(path);
		}
	}
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


void GetParentDir(const wchar_t *srcfile, wchar_t* buf)
{
	CString str;
	str.Format(L"%s", srcfile);

	int k = str.ReverseFind(L'\\');
	str.Delete(k, str.GetLength() - k);

	wcscpy(buf, str.GetBuffer());
}

void UnregUninstaller()
{
	CString strProductName = g_UninstOption.wszProductName;

	TRegistry reg(HKEY_LOCAL_MACHINE);
	if (reg.OpenKey(REGSTR_PATH_UNINSTALL))
	{
		reg.DeleteKey(strProductName);
		reg.CloseKey();
	}
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


void DelStartup()
{
	TRegistry reg(HKEY_LOCAL_MACHINE);
	reg.CreateKey(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");	
	reg.DeleteValue( g_UninstOption.wszProductName);
	reg.CloseKey();
}


void GetInfoFromRegistry()
{
	TRegistry reg(g_UninstOption.wszCompanyName, g_UninstOption.wszProductName);
	g_strProductName = reg.GetStr(PRODUCT_NAME, L"");
	g_strProductVersion = reg.GetStr(PRODUCT_VERSION, L"");
	g_strInstallPath = reg.GetStr(INSTALL_PATH, L"");
	reg.CloseKey();
}

void DeleteInfoFromRegistry()
{
	//delete general information//
	CString strCompany;
	strCompany = g_UninstOption.wszCompanyName;

	TRegistry reg(HKEY_CURRENT_USER);
	reg.OpenKey(L"SOFTWARE\\" + strCompany);
	reg.DeleteKey(g_strProductName);	
	reg.CloseKey();

	//delete product GUID//
	reg.ChangeTopKey(HKEY_CLASSES_ROOT);
	if (reg.OpenKey(g_UninstOption.wszProductGUID))
	{
		reg.DeleteValue(L"ProductName");
		reg.DeleteValue(L"ProductVer");
		reg.DeleteValue(L"CompanyName");
		reg.DeleteValue(L"WebSite");
	}
	reg.CloseKey();
}
