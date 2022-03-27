#pragma once
#include <stdint.h>

#define  _MAX_INSTALLER_STRING_LEN_		260

struct UNINST_OPTION
{
	UNINST_OPTION()
	{
		chkOptionsSilentMode = FALSE;
		chkOptionsCompleteUninstalltion = FALSE;
		chkRestartCom = FALSE;
		chkCloseMainExecutable = TRUE;
		chkCloseRunningApp = FALSE;
		wsprintf(wszRunningAppFile, L"File.exe");
		wsprintf(wszCloseRunningAppName, L"App");
		chkOptionsPrompt2Remove = FALSE;
		wsprintf(wszPrompt2RemoveCommand, L"");
		chkPrompt2Visit = FALSE;
		wsprintf(wszPrompt2VisitUrl, L"http://www.company.com/uninstall.html");
		wsprintf(wszMainExecutableName, L"");
		wsprintf(wszProductName, L"NewProduct");
		wsprintf(wszCompanyName, L"Company");
		wsprintf(wszProductGUID, L"");
	}

	uint8_t				chkOptionsSilentMode;
	uint8_t				chkOptionsCompleteUninstalltion;
	uint8_t				chkRestartCom;
	uint8_t				chkCloseMainExecutable;
	uint8_t				chkCloseRunningApp;
	wchar_t				wszRunningAppFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszCloseRunningAppName[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				chkOptionsPrompt2Remove;
	wchar_t				wszPrompt2RemoveCommand[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				chkPrompt2Visit;
	wchar_t				wszPrompt2VisitUrl[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszMainExecutableName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszProductName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszCompanyName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszProductGUID[_MAX_INSTALLER_STRING_LEN_];
};

extern UNINST_OPTION g_UninstOption;

extern wchar_t g_wszModulePathName[_MAX_INSTALLER_STRING_LEN_];
extern CString g_strDeleteDir; 
void GetModuleFullPathName();

void CloseMainExecutable();

int ProcessFind(TCHAR * strProcessName, DWORD & dwPid);
BOOL IsModuleInProcess(DWORD dwPID, TCHAR * sProcessName);

void DeleteDirectory(CString szDir, BOOL bDeleteSelf, int *pCnt=NULL);

void DeleteShortcut(void);
BOOL DeleteLink(LPCTSTR path);
BOOL DeleteLinkFolder(LPCTSTR path);
void GetParentDir(const wchar_t *srcfile, wchar_t* buf);
CString GetTempDir();

void UnregUninstaller();

/////////registry/////////////
#define  PRODUCT_NAME				L"PRODUCT_NAME"
#define  PRODUCT_VERSION			L"PRODUCT_VERSION"
#define  PRODUCT_DESCRIPTION		L"PRODUCT_DESCRIPTION"
#define  INSTALL_PATH				L"INSTALL_PATH"

extern CString g_strProductName;
extern CString g_strProductVersion;
extern CString g_strInstallPath;

void DelStartup();
void GetInfoFromRegistry();
void DeleteInfoFromRegistry();