
#include "TestWizardInfo.h"

class CAddInfoPage :
	public CWizard97InteriorPageImpl<CAddInfoPage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CAddInfoPage thisClass;
	typedef CWizard97InteriorPageImpl<CAddInfoPage> baseClass;

	// Data members
public:
	// Constructor
	CAddInfoPage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("Additional Informaion"));
		baseClass::SetHeaderSubTitle(_T("Please check additional informations."));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_ADDITIONAL_INFORMAION };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(baseClass)
		COMMAND_HANDLER(IDC_EDIT_SERIAL_NUMBER, EN_CHANGE, OnEnChangedEdit)
		COMMAND_HANDLER(IDC_EDIT_USER_EMAIL, EN_CHANGE, OnEnChangedEdit)
		COMMAND_HANDLER(IDC_EDIT_COMPANY, EN_CHANGE, OnEnChangedEdit)
		COMMAND_HANDLER(IDC_EDIT_CUSTOM_FIELD, EN_CHANGE, OnEnChangedEdit)
		COMMAND_HANDLER(IDC_EDIT_USER_NAME, EN_CHANGE, OnEnChangedEdit)
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

	int CheckUserInfo();

	void SetEnableNextButton();

	bool m_bEnable;
	LRESULT OnEnChangedEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

