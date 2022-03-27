
#include "TestWizardInfo.h"

class CAddTaskPage :
	public CWizard97InteriorPageImpl<CAddTaskPage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CAddTaskPage thisClass;
	typedef CWizard97InteriorPageImpl<CAddTaskPage> baseClass;

	// Data members
public:
	// Constructor
	CAddTaskPage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("Additional Tasks"));
		baseClass::SetHeaderSubTitle(_T("Please check additional tasks."));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_ADDITIONAL_TASK };
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

	int m_nSelProgramGroup;
};
