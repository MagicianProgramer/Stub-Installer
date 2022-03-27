
#ifndef __TestWizardSheet_h__
#define __TestWizardSheet_h__


#include "TestWizardWelcomePage.h"
#include "TestWizardCompletionPage.h"

/////////////////////////////////
#include "CLicAgreePage.h"
#include "CAddInfoPage.h"
#include "CAddTaskPage.h"
#include "CAppFolderPage.h"
#include "CReadMePage.h"
#include "CReadyToInstPage.h"
#include "CAlreadyInstalledPage.h"

#include "../../common/common.h"


class CTestWizardSheet :
	public CWizard97SheetImpl<CTestWizardSheet>,
	public CTestWizardInfoRef
{
protected:
// Typedefs
	typedef CTestWizardSheet thisClass;
	typedef CWizard97SheetImpl<CTestWizardSheet> baseClass;
	typedef CTestWizardInfoRef infoRefClass;

// Data members
	CTestWizardWelcomePage m_pageWelcome;
	CTestWizardCompletionPage m_pageCompletion;

	//////////////add////////////
	CLicAgreePage m_pageLicAgree;
	CAddInfoPage m_pageAddInfo;
	CAddTaskPage m_pageAddTask;
	CAppFolderPage m_pageAppFolder;
	CReadMePage m_pageReadme;
	CReadyToInstPage m_pageReadyToInst;
	CAlreadyInstalledPage m_pageAlreadyInstalled;
public:
// Constructors
	CTestWizardSheet(CTestWizardInfo* pTestWizardInfo, UINT uStartPage = 0, HWND hWndParent = NULL);
	~CTestWizardSheet();

// Message Handlers
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_HELP, OnHelp)
		MESSAGE_HANDLER(UM_SET_TITLE, OnSetWindowTitle)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDHELP, OnBnHelp)
		
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);
	LRESULT OnBnHelp(WORD, WORD, HWND, BOOL&);
	LRESULT OnSetWindowTitle(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	/*void NextPage(BOOL bFinish = FALSE);*/
////////////add stub code////////////
private:
	unsigned char* m_CprPtr;
	unsigned char* m_DataPtr;
	
	void InitFunction();
	void CloseFunction();
	BOOL GetGlobalOptionParam();

	byte* UnZipBuffer(bool btext = false);

	void GetBitmaps();
	void GetLicAndReadme();
	byte* m_pWizardBuff;
	byte* m_pHdrBuff;
	void DrawBackground();

	void RemoveApp();

	/////////////////
	
};

#endif // __TestWizardSheet_h__


