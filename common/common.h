#pragma once
#include <stdint.h>
#include <vector>
using namespace std;

#include <winnt.h>
#include "CMyFile.h"
#include "log/Log.h"

#define		INSTALLER_COMPANY		L"SEBY"
#define		INSTALLER_NAME			L"SInstaller"
#define		INSTALLER_VER			L"v1.0"
#define		PROJECT_EXT				L".sinst"
#define		INSTALLERPKG_NAME		L"InstallerPackage.sizip"

#include <Wininet.h>
#include <iostream>
#include <string.h>
#pragma comment(lib, "wininet.lib")

#define  VECSIZE	16// all vector size is 16, 12 : vs2017, 20, 16 : vs2010 (debug, release)

#define  HASHLEN	20

#define  MARKER_OPTION						0xA0B0C0D0A1B2C3D4
#define  MARKER_ZIP							0xA1B1C1D1A5B6C7D8
#define  MARKER_WIZARD_BMP					0xA2B2C2D212345670
#define  MARKER_HEADER_BMP					0xA2B2C2D212345671
#define  MARKER_LOGO_BMP					0xA2B2C2D212345672
#define  MARKER_LICENSE						0xA3B3C3D312345670
#define  MARKER_README						0xA3B3C3D312345671
#define  MARKER_UNINSTALLER					0xA4B4C4D412344321
#define  MARKER_UPDATER						0xA4B4C4D412344322
#define  MARKER_PACKAGETOOL					0xA4B4C4D412344323
#define  MARKER_SETUP_PREREQUISITES			0xA4B4C4D412344323

#define  STR_BITMAPS	L"Bitmaps"

enum{
	UM_INSTALLATION_CREATE_SHORTCUT = WM_USER + 0x80,
	UM_INTERFACE_FINISH_REFRESH,
	UM_INSTALLATION_MAIN_SHORTCUT,
	UM_INSTALLATION_SHORTCUT_REFRESH,
	UM_INSTALLATION_UNINSTALLATION_REFRESH,
	UM_INSTALLATION_FILESANDFOLDERS_REFRESH,
	UM_INTERFACE_USERINFO_REFRESH,
	UM_INSTALLATION_UPDATER_REFRESH,
	UM_SET_TITLE,
};

#define  _MAX_INSTALLER_STRING_LEN_		260

struct GENERAL_INFORMATION//sheet1-page1
{
	GENERAL_INFORMATION()
	{
		wsprintf(wszProductName, L"NewProduct");
		wsprintf(wszProductVersion, L"1.0");
		wsprintf(wszProductDescription, L"");
		wsprintf(wszCompanyName, L"Company");
		wsprintf(wszWebSite, L"http://www.company.com");
		wsprintf(wszSupportPage, L"http://www.company.com/support");
	}

	wchar_t wszProductName[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	wchar_t wszProductVersion[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	wchar_t wszProductDescription[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	wchar_t wszCompanyName[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	wchar_t wszWebSite[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	wchar_t wszSupportPage[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
};

struct GENERAL_PARAMETERS//sheet1-page2
{
	GENERAL_PARAMETERS()
	{
		nInstallMode = 0;
		nInstallProduct4 = 1;
		wsprintf(wszProductGUID, L"{xxx-xxx-xxx-xxx}");
		chkOptionsUninstallPrevious = TRUE;
		chkOptionsUninstallSilently = FALSE;
		chkOptionsAllowMutiple = FALSE;
	}

	int32_t				nInstallMode;
	int32_t				nInstallProduct4;
	wchar_t				wszProductGUID[ _MAX_INSTALLER_STRING_LEN_];  // the edit box
	uint8_t				chkOptionsUninstallPrevious;
	uint8_t				chkOptionsUninstallSilently;
	uint8_t				chkOptionsAllowMutiple;
};

enum OPERATIONGSYSTEM{
	WINDOWS_XP = 0x01,
	WINDOWS_VISTA = 0x02,
	WINDOWS_7 = 0x04,
	WINDOWS_8 = 0x08,
	WINDOWS_8_1 = 0x10,
	WINDOWS_10 = 0x20,
	WINDOWS_SERVER_2003 = 0x40,
	WINDOWS_SERVER_2008 = 0x80,
	WINDOWS_SERVER_2008_R2 = 0x100,
	WINDOWS_SERVER_2012 = 0x200,
	WINDOWS_SERVER_2012_R2 = 0x400,
	WINDOWS_SERVER_2016 = 0x800,
	WINDOWS_SERVER_2019 = 0x1000,
};

struct GENERAL_REQUIREMENTS//sheet1-page3
{
	GENERAL_REQUIREMENTS()
	{
		nOperatingSystemChk = WINDOWS_XP | WINDOWS_VISTA | WINDOWS_7 | WINDOWS_8 | WINDOWS_8_1 | WINDOWS_10;
		nOperatingSystemMode = 0;
		chkAdminPrivieges = TRUE;
		chkInternetConnection = FALSE;

		chkCloseRunnigApp1 = FALSE;
		wsprintf(wszCloseRunnigApp1_File, L"iexplore.exe");
		wsprintf(wszCloseRunnigApp1_Name, L"Internet Explorer");
		chkCloseRunnigApp2 = FALSE;
		wsprintf(wszCloseRunnigApp2_File, L"");
		wsprintf(wszCloseRunnigApp2_Name, L"");
	}

	uint64_t			nOperatingSystemChk;
	int32_t				nOperatingSystemMode;
	uint8_t				chkAdminPrivieges;
	uint8_t				chkInternetConnection;

	uint8_t				chkCloseRunnigApp1;
	wchar_t				wszCloseRunnigApp1_File[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszCloseRunnigApp1_Name[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				chkCloseRunnigApp2;
	wchar_t				wszCloseRunnigApp2_File[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszCloseRunnigApp2_Name[_MAX_INSTALLER_STRING_LEN_];

};

struct ADDPREREQUISITE{
	ADDPREREQUISITE()
	{
		memset(wszName, 0, _MAX_INSTALLER_STRING_LEN_);
		memset(wszCommand, 0, _MAX_INSTALLER_STRING_LEN_);
		chkSkipFailed = 0;
	}
	wchar_t wszName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszCommand[_MAX_INSTALLER_STRING_LEN_];
	uint8_t chkSkipFailed;
};

struct GENERAL_PREREQUISTES//sheet1-page4
{
	GENERAL_PREREQUISTES()
	{
		chkNetFramework = FALSE;
		nNetFrameworkMode = 0;
		chkInternetExplorer = FALSE;
		nInternetExplorerMode = 0;
		chkAcrobat = FALSE;
		nAcrobatMode = 0;
		chkJava = FALSE;
		nJavaMode = 0;
		chkSqlServerCompact = FALSE;
		nSqlServerCompactMode = 0;
		chkSqlServerExpress = FALSE;
		nSqlServerExpressMode = 0;
		chkVCRuntimeX86 = FALSE;
		nVCRuntimeModeX86 = 0;
		chkVCRuntimeX64 = 0;
		nVCRuntimeModeX64 = 0;
		chkVCRuntimeARM64 = 0;
		nVCRuntimeModeARM64 = 0;
		vecAddPrerequisites.clear();

		wsprintf(wszNetFramework, L"1.0");
		wsprintf(wszInternetExplorer, L"10.0");
		wsprintf(wszAcrobatReader, L"5.0");
		wsprintf(wszJava, L"8");
		wsprintf(wszSQLServerCompact, L"3.5");
		wsprintf(wszSQLServerExpress, L"2005");
		wsprintf(wszVCRuntimeX86, L"2010");
		wsprintf(wszVCRuntimeX64, L"2010");
		wsprintf(wszVCRuntimeARM64, L"2010");

		wsprintf(wszNetFrameworkLink, L"");
		wsprintf(wszInternetExplorerLink, L"");
		wsprintf(wszAcrobatReaderLink, L"");
		wsprintf(wszJavaLink, L"");
		wsprintf(wszSQLServerCompactLink, L"");
		wsprintf(wszSQLServerExpressLink, L"");
		wsprintf(wszVCRuntimeLinkX86, L"");
		wsprintf(wszVCRuntimeLinkX64, L"");
		wsprintf(wszVCRuntimeLinkARM64, L"");

		nUseDownloaderForPrerequisites = 0;
	}

	uint8_t				chkNetFramework;
	int32_t				nNetFrameworkMode;
	wchar_t				wszNetFramework[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszNetFrameworkLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkInternetExplorer;
	int32_t				nInternetExplorerMode;
	wchar_t				wszInternetExplorer[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszInternetExplorerLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkAcrobat;
	int32_t				nAcrobatMode;
	wchar_t				wszAcrobatReader[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszAcrobatReaderLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkJava;
	int32_t				nJavaMode;
	wchar_t				wszJava[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszJavaLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkSqlServerCompact;
	int32_t				nSqlServerCompactMode;
	wchar_t				wszSQLServerCompact[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszSQLServerCompactLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkSqlServerExpress;
	int32_t				nSqlServerExpressMode;
	wchar_t				wszSQLServerExpress[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszSQLServerExpressLink[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkVCRuntimeX86;
	int32_t				nVCRuntimeModeX86;
	wchar_t				wszVCRuntimeX86[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszVCRuntimeLinkX86[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkVCRuntimeX64;
	int32_t				nVCRuntimeModeX64;
	wchar_t				wszVCRuntimeX64[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszVCRuntimeLinkX64[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				chkVCRuntimeARM64;
	int32_t				nVCRuntimeModeARM64;
	wchar_t				wszVCRuntimeARM64[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszVCRuntimeLinkARM64[_MAX_INSTALLER_STRING_LEN_];

	uint8_t				nUseDownloaderForPrerequisites;

	vector<ADDPREREQUISITE>		vecAddPrerequisites;
#ifndef _DEBUG
	int32_t				tmp;
#endif // _DEBUG
};

struct GENERAL_OUTPUT//sheet1-page5
{
	GENERAL_OUTPUT()
	{
		nPackageTypeMode = 0;
		wsprintf(wszDataFileName, L"");
		wsprintf(wszFolder, L"C:\\Users\\Administrator\\Documents\\SInstaller\\#APPLICATION_NAME#");
		wsprintf(wszFileName, L"Setup");
		wsprintf(wszIconFile, L"Default");
	}

	int32_t				nPackageTypeMode;
	wchar_t				wszDataFileName[ _MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszFolder[ _MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszFileName[ _MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszIconFile[ _MAX_INSTALLER_STRING_LEN_];
};


struct ADDINSTALLATIONFILES{
	ADDINSTALLATIONFILES()
	{
		nChkDir = 0;
		wsprintf(wszSrcFullName, L"");
		wsprintf(wszDestName, L"");
		wsprintf(wszDestFullName, L"");
		nFileExist = 1;
		nChkRemoveOnInstall = 1;
		nChkIncludeLastFolderName = 0;
		nChkMainExcutable = 0;
	}

	int8_t	nChkDir;
	wchar_t wszSrcFullName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszDestName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszDestFullName[_MAX_INSTALLER_STRING_LEN_];
	int32_t	nFileExist;
	int8_t	nChkRemoveOnInstall;
	int8_t	nChkIncludeLastFolderName;
	int8_t	nChkMainExcutable;
	FILETIME ftCreate;
	FILETIME ftAccess;
	FILETIME ftWrite;
};

struct INSTALLATION_FILESANDFOLDERS//sheet2-page1
{
	INSTALLATION_FILESANDFOLDERS()
	{
		wsprintf(wszSourceDirectory, L"");
		wsprintf(wszInstallationDirectory, L"#PROGRAM_FILES_X86#\\#APPLICATION_NAME#");
		nChk64BitApplication = 0;
		wsprintf(wszMainExecutable, L"");
		vecInstallationData.clear();
	}

	wchar_t				wszSourceDirectory[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszInstallationDirectory[_MAX_INSTALLER_STRING_LEN_];
	int8_t				nChk64BitApplication;
	wchar_t				wszMainExecutable[_MAX_INSTALLER_STRING_LEN_];	
	vector<ADDINSTALLATIONFILES> vecInstallationData;
#ifndef _DEBUG
	int32_t				tmp;
#endif // _DEBUG
};

struct ADDSHORTCUTDETAIL
{
	ADDSHORTCUTDETAIL()
	{
		wsprintf(wszLocation, L"");
		wsprintf(wszShortcutName, L"");
		wsprintf(wszTargetFile, L"");
		wsprintf(wszDescription, L"");
		wsprintf(wszParameters, L"");
		wsprintf(wszWorkingDir, L"");
		wsprintf(wszIconFile, L"");
		wsprintf(wszShow, L"");
		nIconIdx = 0;
		nMainExe = 0;
	}
	wchar_t wszLocation[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszShortcutName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszTargetFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszDescription[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszParameters[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszWorkingDir[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszIconFile[_MAX_INSTALLER_STRING_LEN_];
	int8_t  nIconIdx;
	wchar_t wszShow[_MAX_INSTALLER_STRING_LEN_];
	int8_t	nMainExe;
};

struct INSTALLATION_SHORTCUTS//sheet2-page2
{
	INSTALLATION_SHORTCUTS()
	{
		wsprintf(wszProgramGroupName, L"#APPLICATION_NAME#");
		vecDetail.clear();
		nChkDesktopShortcutForMainExe = 1;
		nChkStartMenuShortcutForMainExe = 1;
	}

	wchar_t		wszProgramGroupName[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				nChkDesktopShortcutForMainExe;
	uint8_t				nChkStartMenuShortcutForMainExe;
	vector<ADDSHORTCUTDETAIL>  vecDetail;
#ifndef _DEBUG
	int32_t				tmp;
#endif // _DEBUG
};

struct INSTALLATION_UNINSTALLATION//sheet2-page3
{
	INSTALLATION_UNINSTALLATION()
	{
		chkIncludeUninstaller = FALSE;
		chkOptionsAdd2Programs = FALSE;
		chkOptionsSilentMode = FALSE;
		chkOptionsCompleteUninstalltion = FALSE;
		chkOptionsRestartCom = FALSE;
		chkOptionsCloseMainExecutable = TRUE;
		chkOptionsCloseRunningApp = FALSE;
		wsprintf(wszOptionsCloseRunningAppFile, L"File.exe");
		wsprintf(wszOptionsCloseRunningAppName, L"App");
		chkOptionsPrompt2Remove = FALSE;
		wsprintf(wszOptionsPrompt2RemoveCommand, L"");
		chkOptionsPrompt2Visit = FALSE;
		wsprintf(wszOptionsPrompt2VisitUrl, L"http://www.company.com/uninstall.html");
	}

	uint8_t				chkIncludeUninstaller;
	uint8_t				chkOptionsAdd2Programs;
	uint8_t				chkOptionsSilentMode;
	uint8_t				chkOptionsCompleteUninstalltion;
	uint8_t				chkOptionsRestartCom;
	uint8_t				chkOptionsCloseMainExecutable;
	uint8_t				chkOptionsCloseRunningApp;
	wchar_t				wszOptionsCloseRunningAppFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t				wszOptionsCloseRunningAppName[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				chkOptionsPrompt2Remove;
	wchar_t				wszOptionsPrompt2RemoveCommand[_MAX_INSTALLER_STRING_LEN_];
	uint8_t				chkOptionsPrompt2Visit;
	wchar_t				wszOptionsPrompt2VisitUrl[_MAX_INSTALLER_STRING_LEN_];
};

enum AUTO_CHECK_METHOD{
	ON_WINOWS_START_UP = 0,
	EVERY_DAY,
	EVERY_WEEK,
	EVERY_MONTH,
};

enum SMART_NOTIFICATION_METHOD{
	_1 = 0,
	_2,
	_3,
	_4,
};

struct INSTALLATION_UPDATER//sheet2-page4
{
	INSTALLATION_UPDATER()
	{
		nChkIncludeUpdaterTool = 0;
		wsprintf(wszUpdateInformationFileURL, L"http://www.company.com/update_on_server.txt");
		wsprintf(wszAdditionalParams, L"");
		wsprintf(wszPassParamsToNewVersionURL, L"");
		nChkProductNameInUpdaterTitle = 0;
		nChkAllowUsersToChangeTheUpdaterOptions = 0;
		nChkUseDownloader = 0;
		nChkAutomaticallyCheckForUpdates = 0;
		nAutoCheckMethod = EVERY_DAY;
		nChkInstallUpdatesSilently = 0;
		nChkUseSmartNotification = 0;
		nSmartNotificationMethod = _1;
		wsprintf(wszAdditionalParams, L"");
		wsprintf(wszPassParamsToNewVersionURL, L"");
	}

	uint8_t nChkIncludeUpdaterTool;
	wchar_t wszUpdateInformationFileURL[_MAX_INSTALLER_STRING_LEN_];
	uint8_t nChkProductNameInUpdaterTitle;
	uint8_t nChkAllowUsersToChangeTheUpdaterOptions;
	uint8_t nChkUseDownloader;
	uint8_t nChkAutomaticallyCheckForUpdates;
	AUTO_CHECK_METHOD nAutoCheckMethod;
	uint8_t nChkInstallUpdatesSilently;
	uint8_t nChkUseSmartNotification;
	SMART_NOTIFICATION_METHOD nSmartNotificationMethod;
	wchar_t wszAdditionalParams[ _MAX_INSTALLER_STRING_LEN_];
	wchar_t wszPassParamsToNewVersionURL[ _MAX_INSTALLER_STRING_LEN_];
};

struct INSTALLATION_ADVANCED//sheet2-page5
{
	INSTALLATION_ADVANCED()
	{
		chkAllowInstallNoFreeSpace = FALSE;
		chkAllowInstallFailedPrerequisites = FALSE;
		wsprintf(wszUninstallerFileName, L"Uninstaller.exe");
		nRequiredSpace = 0;
	}

	wchar_t		wszUninstallerFileName[_MAX_INSTALLER_STRING_LEN_];
	uint8_t		chkAllowInstallNoFreeSpace;
	uint8_t		chkAllowInstallFailedPrerequisites;
	uint64_t	nRequiredSpace;
};

struct TEMPLATE_EDITOR{
	TEMPLATE_EDITOR()
	{
		nChkUseCustomSettings = 0;

		nLogoSettingAutoSize = 0;
		nLogoSettingTransparent = 0;
		nLogoSettingStretch = 0;
		nLogoSettingProportioal = 0;

		nWindowSizeWidth = 491;
		nWindowSizeHeight = 293;
		nChkWindowSizeBorderless = 0;
		nWindowColor = 16777215;

		nChkFontAddFontFile = 0;
		wszFontAddFontFile[0] = 0;
		wsprintf(wszFontName, L"Courier New");

		nHeaderSize = 24;
		nHeaderColor = 8421504;
		nHeaderStyle = 0;
		nHeaderOffset = 0;

		nTextSize = 15;
		nTextColor = 8421504;
		nTextStyle = 0;
		nTextOffset = 0;

		nButtonWidth = 120;
		nButtonFontSize = 18;
		nButtonOffset = 0;

		nLicAndCopyrightSize = 15;
		nLicAndCopyrightTextColor = 8421504;
		nLicAndCopyrightLinkColor = 16744448;
		nLicAndCopyrightPosition = 0;
	}
	
	uint8_t nChkUseCustomSettings;

	//logo setting//
	uint8_t nLogoSettingAutoSize;
	uint8_t nLogoSettingTransparent;
	uint8_t nLogoSettingStretch;
	uint8_t nLogoSettingProportioal;

	//window size//
	int32_t nWindowSizeWidth;
	int32_t nWindowSizeHeight;
	uint8_t nChkWindowSizeBorderless;

	//window color//
	uint32_t	nWindowColor;

	//font//
	uint8_t nChkFontAddFontFile;
	wchar_t wszFontAddFontFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t wszFontName[_MAX_INSTALLER_STRING_LEN_];

	//header//
	int32_t nHeaderSize;
	uint32_t nHeaderColor;
	uint8_t nHeaderStyle;
	int32_t nHeaderOffset;

	//text//
	int32_t nTextSize;
	uint32_t nTextColor;
	uint8_t nTextStyle;
	int32_t nTextOffset;

	//button//
	int32_t nButtonWidth;
	int32_t nButtonFontSize;
	int32_t nButtonOffset;

	//license/copyright text//
	int32_t nLicAndCopyrightSize;
	int32_t nLicAndCopyrightTextColor;
	int32_t nLicAndCopyrightLinkColor;
	uint8_t nLicAndCopyrightPosition;
};

extern TEMPLATE_EDITOR g_templateEditor;

struct INTERFACE_MAIN//sheet3-page1
{
	INTERFACE_MAIN()
	{
		nGraphicInterfaceMode = 2;//modern
		wsprintf(wszGraphicLogoBitmap, L"#INSTALLER_DEFAULT_DIR#\\Bitmps\\Logo\\01.bmp");

		chkDialogs2ShowWelcome = TRUE;
		chkDialogs2ShowLicenseAgreement = FALSE;
		chkDialogs2ShowReadme = FALSE;
		chkDialogs2ShowUserInformation = FALSE;
		chkDialogs2ShowDestinationFolder = TRUE;
		chkDialogs2ShowAdditionalTasks = TRUE;
		chkDialogs2ShowReady2Install = FALSE;
		chkDialogs2ShowFinish = TRUE;

		chkOptionsShowCopyright = TRUE;
		chkOptionsHideFileNames = TRUE;
		chkOptionsBeepOnFinish = TRUE;
		chkOptionsRunAsAdmin = FALSE;

		wsprintf(wszOptionsReadmeFile, L"");
		wsprintf(wszOptionsLicenseFile, L"");
		chkOptionsShowAgreement = FALSE;

		wsprintf(wszGraphicWizardBmp, L"#INSTALLER_DEFAULT_DIR#\\Bitmaps\\Wizard\\01.bmp");
		wsprintf(wszGraphicHdrBmp, L"#INSTALLER_DEFAULT_DIR#\\Bitmaps\\Header\\01.bmp");
	}

	int32_t					nGraphicInterfaceMode;
	wchar_t					wszGraphicLogoBitmap[_MAX_INSTALLER_STRING_LEN_];

	uint8_t					chkDialogs2ShowWelcome;
	uint8_t					chkDialogs2ShowLicenseAgreement;
	uint8_t					chkDialogs2ShowReadme;
	uint8_t					chkDialogs2ShowUserInformation;
	uint8_t					chkDialogs2ShowDestinationFolder;
	uint8_t					chkDialogs2ShowAdditionalTasks;
	uint8_t					chkDialogs2ShowReady2Install;
	uint8_t					chkDialogs2ShowFinish;

	uint8_t					chkOptionsShowCopyright;
	uint8_t					chkOptionsHideFileNames;
	uint8_t					chkOptionsBeepOnFinish;
	uint8_t					chkOptionsRunAsAdmin;
	wchar_t					wszOptionsLicenseFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t					wszOptionsReadmeFile[_MAX_INSTALLER_STRING_LEN_];
	uint8_t					chkOptionsShowAgreement;

	wchar_t					wszGraphicWizardBmp[_MAX_INSTALLER_STRING_LEN_];
	wchar_t					wszGraphicHdrBmp[_MAX_INSTALLER_STRING_LEN_];
};

struct INTERFACE_LANGUAGES//sheet3-page2
{
	INTERFACE_LANGUAGES()
	{

	}
	//No description in project guide//
};



struct INTERFACE_USERINFORMATION//sheet3-page5
{
	INTERFACE_USERINFORMATION()
	{
		chkUserName = FALSE;
		chkUserNameRequired = FALSE;
		chkUserEmail = FALSE;
		chkUserEmailRequired = FALSE;
		chkCompanyName = FALSE;
		chkCompanyNameRequired = FALSE;
		chkCustomFiled = FALSE;
		chkCustomFiledRequired = FALSE;
		wsprintf(wszFieldText, L"");
		nFieldTypeMode = 0;
		chkFieldTypeFileFolder = FALSE;
		chkSerialNumber = FALSE;
		chkSerialNumberRequired = FALSE;
		wsprintf(wszSNList, L"");
		nSNCount = 0;
	}
	uint8_t			chkUserName;
	uint8_t			chkUserNameRequired;
	uint8_t			chkUserEmail;
	uint8_t			chkUserEmailRequired;
	uint8_t			chkCompanyName;
	uint8_t			chkCompanyNameRequired;
	uint8_t			chkCustomFiled;
	uint8_t			chkCustomFiledRequired;
	wchar_t			wszFieldText[_MAX_INSTALLER_STRING_LEN_];
	int32_t			nFieldTypeMode;
	uint8_t			chkFieldTypeFileFolder;
	uint8_t			chkSerialNumber;
	uint8_t			chkSerialNumberRequired;
	int32_t			nSNCount;
	wchar_t			wszSNList[_MAX_INSTALLER_STRING_LEN_ * 100];
};

struct INTERFACE_CUSTOM//sheet3-page6
{
	INTERFACE_CUSTOM()
	{
		chkCheckBox = FALSE;
		chkChecked = FALSE;
		wsprintf(wszCheckBoxText,L"");
		chkComboBox = FALSE;
		wsprintf(wszComboBoxText,L"");
		wsprintf(wszComboBoxItems,L"");
		nComboBoxDefaultIdx = 0;
		chkSelectComponents = FALSE;
		wsprintf(wszSelComponentsName,L"");
		wsprintf(wszSelComponentsDescription,L"");
		chkSelComponentsAtLeast = FALSE;
		chkSelComponentsCalcuate = FALSE;
		nSelComponentsCalculateMethodMode = 0;
		chkAddStandardcCmponents = FALSE;
	}
	uint8_t			chkCheckBox;
	uint8_t			chkChecked;
	wchar_t			wszCheckBoxText[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkComboBox;
	wchar_t			wszComboBoxText[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszComboBoxItems[_MAX_INSTALLER_STRING_LEN_];
	int8_t			nComboBoxDefaultIdx;
	uint8_t			chkSelectComponents;
	wchar_t			wszSelComponentsName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszSelComponentsDescription[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkSelComponentsAtLeast;
	uint8_t			chkSelComponentsCalcuate;
	int32_t			nSelComponentsCalculateMethodMode;
	uint8_t			chkAddStandardcCmponents;

};

struct INTERFACE_FINISH//sheet3-page7
{
	INTERFACE_FINISH()
	{
		chkLaunchApp = FALSE;
		wsprintf(wszLaunchAppFile, L"");
		chkLaunchAppCheckedByDefault = FALSE;
		chkLaunchAppRunAsAdmin = FALSE;
		chkCustomAction = FALSE;
		wsprintf(wszCustomActionName, L"Visit Web Site");
		wsprintf(wszCustomActionCommand, L"#WEB_SITE#");
		chkCustomActionCheckedByDefault = FALSE;
		chkCustomActionRunAsAdmin = FALSE;
		chkRestartComputer = FALSE;
	}
	uint8_t			chkLaunchApp;
	wchar_t			wszLaunchAppFile[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkLaunchAppCheckedByDefault;
	uint8_t			chkLaunchAppRunAsAdmin;
	uint8_t			chkCustomAction;
	wchar_t			wszCustomActionName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszCustomActionCommand[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkCustomActionCheckedByDefault;
	uint8_t			chkCustomActionRunAsAdmin;
	uint8_t			chkRestartComputer;
};

struct ADDREGISTRYVALUE{
	ADDREGISTRYVALUE()
	{
		wsprintf(wszBitness, L"");
		wsprintf(wszRegistry_key, L"");
		wsprintf(wszRegistry_value, L"");
		wsprintf(wszValuetype, L"");
		wsprintf(wszValue_data, L"");
		chkOverwrite = 0;
		chkRemove = 0;
	}
	wchar_t			wszBitness[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszRegistry_key[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszRegistry_value[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszValuetype[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszValue_data[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkOverwrite;
	uint8_t			chkRemove;
};

struct SYSTEM_REGISTRY//sheet4-page1
{
	SYSTEM_REGISTRY()
	{
		vecAddRegvalue.clear();
	}
	vector<ADDREGISTRYVALUE>	vecAddRegvalue;
};

struct ADDINIVALUE{
	ADDINIVALUE()
	{
		wsprintf(wszInifile, L"");
		wsprintf(wszSection, L"");
		wsprintf(wszKey, L"");
		wsprintf(wszValue, L"");
		chkSkip = 0;
		chkOverwrite = 0;
		chkRemove = 0;
	}
	wchar_t			wszInifile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszSection[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszKey[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszValue[_MAX_INSTALLER_STRING_LEN_];
	uint8_t			chkSkip;
	uint8_t			chkOverwrite;
	uint8_t			chkRemove;

};

struct SYSTEM_INIFILES//sheet4-page2
{
	SYSTEM_INIFILES()
	{
		vecAddInivalue.clear();
	}
	vector<ADDINIVALUE>		vecAddInivalue;
};

struct ADDVARIABLE{
	ADDVARIABLE()
	{
		wsprintf(wszVariable, L"");
		wsprintf(wszBitness, L"");
		wsprintf(wszRegistryKey, L"");
		wsprintf(wszRegistryValue, L"");
		wsprintf(wszValuetype, L"");
		wsprintf(wszDefault_data, L"");
	}
	wchar_t			wszVariable[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszBitness[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszRegistryKey[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszRegistryValue[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszValuetype[_MAX_INSTALLER_STRING_LEN_];
	wchar_t			wszDefault_data[_MAX_INSTALLER_STRING_LEN_];
};


struct SYSTEM_VARIABLES//sheet4-page3
{
	SYSTEM_VARIABLES()
	{
		vecAddVariable.clear();
	}
	vector<ADDVARIABLE>		vecAddVariable;
};

struct ADDCOMMAND{
	ADDCOMMAND()
	{
		wsprintf(wszfile, L"");
		wsprintf(wszParameters, L"");
		wsprintf(wszLaunchOnOS, L"");
		wsprintf(wszShow, L"");
		wsprintf(wszTiming, L"");
		chkWait4Process2Finish = 0;
		chkRunAsAdmin = 0;
	}
	wchar_t		wszfile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszParameters[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszLaunchOnOS[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszShow[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszTiming[_MAX_INSTALLER_STRING_LEN_];
	uint8_t		chkWait4Process2Finish;
	uint8_t		chkRunAsAdmin;
};


struct SYSTEM_COMMANDS//sheet4-page4
{
	SYSTEM_COMMANDS()
	{
		vecAddCommand.clear();
	}
	vector<ADDCOMMAND>		vecAddCommand;
};


struct ADDEXTENSION{
	ADDEXTENSION()
	{
		nIconIndex = 0;
	}

	wchar_t		wszExtension[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszAssociatedFiles[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszDescription[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszIconFile[_MAX_INSTALLER_STRING_LEN_];
	int8_t		nIconIndex;
};
struct REGISTER_FILEASSOCIATIONS//sheet5-page1
{
	REGISTER_FILEASSOCIATIONS()
	{
		vecAddExtension.clear();
	}
	vector<ADDEXTENSION>		vecAddExtension;
};


struct ADDENVIRONMENTVARIABLE{
	ADDENVIRONMENTVARIABLE()
	{
		wszIfExists[0] = 0;
		wszLevel[0] = 0;
		chkRollback = TRUE;
	}
	wchar_t		wszVariableName[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszValue[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszIfExists[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszLevel[_MAX_INSTALLER_STRING_LEN_];
	uint8_t		chkRollback;
};
struct REGISTER_ENVIRONMENTVARIABLES//sheet5-page2
{
	REGISTER_ENVIRONMENTVARIABLES()
	{
		vecAddEnvironmentVariable.clear();
	}
	vector<ADDENVIRONMENTVARIABLE>		vecAddEnvironmentVariable;
};

struct	_REGISTER{
	wchar_t		wszFile[_MAX_INSTALLER_STRING_LEN_];
	wchar_t		wszRegisteras[_MAX_INSTALLER_STRING_LEN_];
	uint8_t		chkUnregister;
};

struct REGISTER_OTHERS//sheet5-page3
{
	REGISTER_OTHERS()
	{
		vecRegister.clear();
	}
	vector<_REGISTER>	vecRegister;
};


struct STD_GENERAL{
	GENERAL_INFORMATION		info;
	GENERAL_PARAMETERS		params;
	GENERAL_REQUIREMENTS	requires;
	GENERAL_PREREQUISTES	prerequisites;
	GENERAL_OUTPUT			output;
};

struct STD_INSTALLATION{
	INSTALLATION_FILESANDFOLDERS	filesandfolders;
	INSTALLATION_SHORTCUTS			shortcuts;
	INSTALLATION_UNINSTALLATION		uninst;
	INSTALLATION_UPDATER			updater;
	INSTALLATION_ADVANCED			advanced;
};

struct STD_INTERFACE{
	INTERFACE_MAIN					main;
	INTERFACE_LANGUAGES				lang;
	INTERFACE_USERINFORMATION		userinfo;
	INTERFACE_CUSTOM				custom;
	INTERFACE_FINISH				finish;
};

struct STD_SYSTEM{
	SYSTEM_REGISTRY			registry;
	SYSTEM_INIFILES			inifiles;
	SYSTEM_VARIABLES		variables;
	SYSTEM_COMMANDS			commands;
};

struct STD_REGISTER{
	REGISTER_FILEASSOCIATIONS		fileassoc;
	REGISTER_ENVIRONMENTVARIABLES	envvar;
	REGISTER_OTHERS					others;
};


struct STD_GLOBAL_DATA
{
	STD_GENERAL			general_sh1;
	STD_INSTALLATION	install_sh2;
	STD_INTERFACE		interface_sh3;
	STD_SYSTEM			sys_sh4;
	STD_REGISTER		register_sh5;
};
extern STD_GLOBAL_DATA g_globaldata;


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

struct PREREQUISITES_LINKS
{
	PREREQUISITES_LINKS()
	{
		wsprintf(wszNetLink, L"https://download.visualstudio.microsoft.com/download/pr/63764a19-21d6-4e7b-b0b1-dddc215143ee/0e7fddbacf4fd9c42e8dd7718e88cd4c/dotnet-sdk-5.0.301-win-x86.zip");
		wsprintf(wszAcrobatLink, L"https://get.adobe.com/reader/download/?installer=Reader_DC_2021.005.20048_English_for_Windows&os=Windows%2010&browser_type=Gecko&browser_dist=Firefox&dualoffer=false&mdualoffer=true&cr=true&stype=7623&a=McAfee_Security_Scan_Plus&a=McAfee_Safe_Connect");
		wsprintf(wszJAVALink, L"https://javadl.oracle.com/webapps/download/AutoDL?BundleId=244582_d7fc238d0cbf4b0dac67be84580cfb4b");
		wsprintf(wszSQLCompactLink, L"https://www.microsoft.com/en-us/download/details.aspx?id=30709");
		wsprintf(wszSQLExpressLink, L"https://download.microsoft.com/download/5/E/9/5E9B18CC-8FD5-467E-B5BF-BADE39C51F73/SQLServer2017-SSEI-Expr.exe");
		wsprintf(wszVCRuntimeForX86Link, L"https://aka.ms/vs/16/release/vc_redist.x86.exe");
		wsprintf(wszVCRuntimeForX64Link, L"https://aka.ms/vs/16/release/vc_redist.x64.exe");
		wsprintf(wszVCRuntimeForARM64Link, L"https://aka.ms/vs/16/release/vc_redist.arm64.exe");
		nUseDownloader = 0;
	}
	wchar_t  wszNetLink[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszAcrobatLink[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszJAVALink[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszSQLCompactLink[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszSQLExpressLink[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszVCRuntimeForX86Link[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszVCRuntimeForX64Link[_MAX_INSTALLER_STRING_LEN_];
	wchar_t  wszVCRuntimeForARM64Link[_MAX_INSTALLER_STRING_LEN_];
	uint8_t		nUseDownloader;
};

struct FILE_OPTIONS
{
	FILE_OPTIONS()
	{
		chkDoNotAddHiddenFilesAndFolders = 1;
		chkDoNotAddEmptyFolders = 1;
		chkDoNotAddZeroSizeFile = 1;
		wsprintf(wszDoNotAddFilesWithExtensions, L"");
		chkDoNotShowFilesTreeView = 0;
		chkUseRelativePaths = 0;
		chkSignSetupFileAfterBuilding = 0;
		wsprintf(wszCommand, L"\"C:\\Program Files (x86)\\Windows Kits\\10\\App Certification Kit\\signtool.exe\" sign /f _code_signing_ssl.pfx /p \"PASSWORD\" /tr http://sha256timestamp.ws.symantec.com/sha256/timestamp /fd SHA256  <EXECUTABLE FULL PATH>");
		chkUninstallUpdaterDownloader = 0;
	}

	uint8_t  chkDoNotAddHiddenFilesAndFolders;
	uint8_t  chkDoNotAddEmptyFolders;
	uint8_t	 chkDoNotAddZeroSizeFile;
	wchar_t  wszDoNotAddFilesWithExtensions[_MAX_INSTALLER_STRING_LEN_];
	uint8_t  chkDoNotShowFilesTreeView;
	uint8_t  chkUseRelativePaths;
	uint8_t  chkSignSetupFileAfterBuilding;
	wchar_t  wszCommand[_MAX_INSTALLER_STRING_LEN_];
	uint8_t  chkUninstallUpdaterDownloader;
};

struct GENERAL_OPTIONS
{
	GENERAL_OPTIONS()
	{
		chkUsingWizard = 1;
		nOnStartupMode = 0;
		chkCheckForUpdates = 0;
		
		chkCommonOption = 0;
		chkUpdateFileVersionInfo = 0;
		chkCompressEngineFile = 0;
		chkEncryptSensitiveData = 0;
		chkGetProductVersionFromMainExecutable = 0;
	}

	int32_t chkUsingWizard;
	int32_t nOnStartupMode;
	uint8_t chkCheckForUpdates;

	uint8_t chkCommonOption;
	uint8_t chkUpdateFileVersionInfo;
	uint8_t chkCompressEngineFile;
	uint8_t chkEncryptSensitiveData;
	uint8_t chkGetProductVersionFromMainExecutable;
};

extern GENERAL_OPTIONS g_generalOptions;
extern FILE_OPTIONS g_fileOptions;
extern PREREQUISITES_LINKS g_preLinks;

void SaveProjectOptionsIntoRegistry();
void GetProjectOptionsFromRegistry();

extern wchar_t g_wszModulePathName[_MAX_INSTALLER_STRING_LEN_];

void GetModuleFullPathName();

extern CMyFile g_rfile;
extern byte* g_pLicBuff;
extern byte* g_pReadmeBuff;

int8_t  CompareGlobalData(STD_GLOBAL_DATA* pData1, STD_GLOBAL_DATA* pData2);//return value is 0:equal, 1:changed
void CopyGlobalData(STD_GLOBAL_DATA* pDataDst, STD_GLOBAL_DATA* pDataSrc);

//log//
extern CLog* g_pLog;
void init_log();
void close_log();

///////General Section//////////
void General_Requirements();
extern CString g_strSetupPrerequisitesExePathName;
BOOL General_Prerequisites();


BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor, DWORD& dwServicePack);
BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor);

BOOL CheckOS(CString& strOS);
BOOL CheckOSBit(CString& strBit);
void CloseRunningApp1();
void CloseRunningApp2();
BOOL DowndloadPrerequisites(wchar_t* wszURL);

BOOL check_net_framework();
BOOL check_internet_explorer();
BOOL check_acrobat_reader();
BOOL check_java();
BOOL check_sql_server_compact();
BOOL check_sql_server_express();
BOOL check_vc_runtime();

//////Installation Section//////
BOOL ConfirmDiskFreeSpace(UINT64 len);

//updater//
#define		CONFIG_INIFILE			L"updater_config.ini"
#define		UPDATE_TXT				L"update_on_server.txt"
#define		TMP_FILE				L"tmp";


#define		CONFIG_SECTION			L"CONFIG"

#define  KEY_PRODUCT_DETAILS_NAME									L"ProductDetails_Name"
#define  KEY_PRODUCT_DETAILS_MAIN_EXE								L"ProductDetails_MainExe"
#define  KEY_PRODUCT_DETAILS_GET_CURRENT_VER_METHOD					L"ProductDetails_GetCurrentVerMethod"
#define  KEY_PRODUCT_DETAILS_GET_CURRENT_VER_TEXT					L"ProductDetails_GetCurrentVerText"
#define  KEY_PRODUCT_DETAILS_APP_INSTALLED_FOR_ALL_USERS			L"ProductDetails_AppInstalledForAllUsers"
#define  KEY_PRODUCT_DETAILS_APP_IS_64BIT							L"ProductDetails_AppIs64Bit"

#define  KEY_ADVANCED_OPTION_PRODUCT_NAME_IN_TITLE					L"AdvancedOption_ProductNameInTitle"
#define  KEY_ADVANCED_OPTION_NEW_DIALOG								L"AdvancedOption_NewDialog"
#define  KEY_ADVANCED_OPTION_ALLOW_USERS_TO_CHANGE_OPTION			L"AdvancedOption_AllowUsersToChangeOption"
#define  KEY_ADVANCED_OPTION_USE_DOWNLOADER							L"AdvancedOption_UseDownloader"
#define  KEY_ADVANCED_OPTION_UPDATE_MAIN_EXE_ONLY					L"AdvancedOption_UpdateMainExeOnly"
#define  KEY_ADVANCED_OPTION_AUTO_CHECK_FOR_UPDATE					L"AdvancedOption_AutoCheckForUpdate"
#define  KEY_ADVANCED_OPTION_AUTO_CHECK_METHOD						L"AdvancedOption_AutoCheckMethod"
#define  KEY_ADVANCED_OPTION_INSTALL_UPDATES_SILETLY				L"AdvancedOption_InstallUpdatesSilently"
#define  KEY_ADVANCED_OPTION_USE_SMART_NOTIFICATION					L"AdvancedOption_UseSmartNotification"
#define  KEY_ADVANCED_OPTION_SMART_NOTIFICATION_METHOD				L"AdvancedOption_SmartNotificationMethod"
#define  KEY_ADVANCED_OPTION_ADDITIONAL_PARAMS						L"AdvancedOption_AdditionalParams"
#define  KEY_ADVANCED_OPTION_PASS_PARAMS_TO_NEW_VERSION_URL			L"AdvancedOption_PassParamsToNewVersionURL"

#define  KEY_UPDATE_INFORMATION_FILE_UPDATE_TEXT_URL				L"UpdateInformationFile_URLofUpdateInformationFile"

int save_updateinfo_to_ini(CString strFile);

CString ExecCmd(const char* cmd);


CString GetRealPathName(wchar_t* wszPathName);

////////////////missing features//////////////////////
BOOL IsProcessElevated();//administrator privileges
BOOL InternetConnected();//internet connection
void FinishTab();//	interface\finish tab

void SystemRegistry();//system\registry tab
void SystemVariable();//system\variable tab

void RegisterFileAssociations();//registry\file association tab

#define  UNINSTLST		L"uninst.lst"
extern CLog* g_pUninstLst;
void init_uninstlst();
void close_uninstlst();
void WriteUninstallList(BOOL bFile, CString strFullPath);


//===============error================//
enum {
	SINST_NO_ERROR = 0x0,
	SINST_DOWNLOAD_PKG_FAILED,
	SINST_PKG_FILE_NOT_OPENED,
};

extern INT g_nErr;