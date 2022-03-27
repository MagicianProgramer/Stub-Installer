
#include "TestWizardInfo.h"

class CReadyToInstPage :
	public CWizard97InteriorPageImpl<CReadyToInstPage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CReadyToInstPage thisClass;
	typedef CWizard97InteriorPageImpl<CReadyToInstPage> baseClass;

	// Data members
	CFont m_fontSummary;
	CRichEditCtrl m_editSummary;
	CWindow m_progWind;
public:
	// Constructor
	CReadyToInstPage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("Ready To Install"));
		baseClass::SetHeaderSubTitle(_T(""));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_READY_TO_INSTALL };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// Helpers
	void InitializeFont(void);
	void InitializeControls(void);
	void UninitializeControls(void);
	void InitializeValues(void);
	bool StoreValues(void);

	// Overrides from base class
	int OnSetActive();
	int OnWizardNext();
	int OnWizardBack();
	void OnHelp();

	void StartMainInstall();
private:
	unsigned char* m_CprPtr;
	unsigned char* m_DataPtr;

	BOOL ExtractZipFiles();
	CString GetSaveFileFullPathName(wchar_t* wszName, CString& strFullDir, BOOL bDir = FALSE);

	//////////////////////////////
	void CreateShortcuts();	
	void CreateLink(CString strLocation, CString strName, CString strTarget);
	/////////////////////////////
	
public:
	HANDLE m_hExtractThread;
	void UnzipProc();
	void OtherProc();

	void DeleteShortcuts();
	void UnregisterValues();
	BOOL m_bRun;
	BOOL m_bComplete;	
	BOOL m_bPause;

	CString UnZipFile(wchar_t* wszPathName, BOOL bSetSts = TRUE, BOOL bPkg = FALSE);

	void RegisterValues(BOOL bRepair = FALSE);

	void ExtractUninstaller();
	void ExtractUpdater();
};

DWORD WINAPI thStartUnzipProc(LPVOID lParam);
void CALLBACK TimerCallbackFunction(HWND hwnd, UINT, UINT, DWORD);
void CALLBACK TimerCheckPrerequisites(HWND hwnd, UINT, UINT, DWORD);