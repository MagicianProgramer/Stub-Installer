
#include "TestWizardInfo.h"

class CReadMePage :
	public CWizard97InteriorPageImpl<CReadMePage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CReadMePage thisClass;
	typedef CWizard97InteriorPageImpl<CReadMePage> baseClass;

	// Data members
	CRichEditCtrl m_pRichReadme;
public:
	// Constructor
	CReadMePage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("ReadME File Content"));
		baseClass::SetHeaderSubTitle(_T("Please check Read me file."));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_READ_ME };
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
};

