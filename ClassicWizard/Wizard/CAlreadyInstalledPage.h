
#include "TestWizardInfo.h"

class CReadyToInstPage;

class CAlreadyInstalledPage :
	public CWizard97InteriorPageImpl<CAlreadyInstalledPage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CAlreadyInstalledPage thisClass;
	typedef CWizard97InteriorPageImpl<CAlreadyInstalledPage> baseClass;

	// Data members
	
public:
	// Constructor
	CAlreadyInstalledPage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("Already Installed"));
		baseClass::SetHeaderSubTitle(_T(""));
		m_pInstaPage = FALSE;
	}

	// Message Handlers
	enum { IDD = IDD_DLG_ALREADY_INSTALLED };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	// Helpers
	void InitializeControls(void);
	void UninitializeControls(void);
	void InitializeValues(void);
	bool StoreValues(void);

	// Overrides from base class
	int OnSetActive();
	int OnWizardNext();
	int OnWizardBack();
	void OnHelp();

	void Working();
	BOOL m_bWorking;
	BOOL m_bStart;

	CWindow m_progBar;
	CReadyToInstPage* m_pInstaPage;
};

