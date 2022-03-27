
#include "TestWizardInfo.h"

class CLicAgreePage :
	public CWizard97InteriorPageImpl<CLicAgreePage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CLicAgreePage thisClass;
	typedef CWizard97InteriorPageImpl<CLicAgreePage> baseClass;

	// Data members
	CButton m_buttonAccept;
	CRichEditCtrl m_richLic;
public:
	// Constructor
	CLicAgreePage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("License Agreement File Content"));
		baseClass::SetHeaderSubTitle(_T("Please check license agreement."));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_LICENSE_AGREE };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(baseClass)
		COMMAND_HANDLER(IDC_CHECK_WIZ97_LIC_AGREEMENT, BN_CLICKED, OnBnClickedCheckWiz97LicAgreement)
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
	LRESULT OnBnClickedCheckWiz97LicAgreement(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

