#pragma once
#include <stdint.h>
#include <map>
using namespace std;

#define		STR_DEFAULT_DIR		L"NewStubProduct"

extern map<CString, CString> g_mapDirList;
extern CString g_strInstDir;

BOOL TIsWow64();
BOOL TIsUserAnAdmin();
BOOL TIsEnableUAC();

CString GetProgramFiles(BOOL bX86 = TRUE);
CString GetDefaultInstallDir();
CString GetSetupTempDir();
CString GetTempDir();
CString GetMyDocuments();
CString GetMyPictures();
CString GetMyMusic();
CString GetAppData();
CString GetLocalAppData();
CString GetUserProfile();
CString GetTemplates();
CString GetFavorites();
CString GetSentTo();
CString GetDesktop();
CString GetStartMenu();
CString GetProgramsMenu();
CString GetStartUp();
CString GetInstallDirectory();

void GetDirNamesAndVariables();
void DeleteDirectory(CString szDir, BOOL bDeleteSelf, int *pCnt=NULL);

BOOL ShellLink(LPCTSTR wsrc, LPCTSTR wdest, LPCTSTR warg = L"", LPCTSTR app_id = NULL);


/////////registry/////////////
#define		PRODUCT_NAME				L"PRODUCT_NAME"
#define		PRODUCT_VERSION				L"PRODUCT_VERSION"
#define		PRODUCT_DESCRIPTION			L"PRODUCT_DESCRIPTION"
#define		PRODUCT_SUPPORT_PAGE		L"SUPPORT_PAGE"
#define		PRODUCT_COMPANY_NAME		L"COMPANY_NAME"
#define		PRODUCT_WEBSITE				L"COMPANY_WEBSITE"
#define		INSTALL_PATH				L"INSTALL_PATH"

INT CheckAlreadyInstalled(CString& strProName, CString& strProVer);

void SetInfoIntoRegistry();
void DeleteInfoFromRegistry();

BOOL DeleteLink(LPCTSTR path);
BOOL DeleteLinkFolder(LPCTSTR path);

void RegisterOthers(char* szfile, BOOL bReg = TRUE);

void SystemCommands();
void SystemIniFiles();

void SetStartup();
void SetRegKeyValue(int valuetype, CString strKey, int nVal, CString strValue);//value type 0 : int, 1 : str

////////////////missing features//////////////////////
