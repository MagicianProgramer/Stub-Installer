
#include "TestWizardInfo.h"

class CAppFolderPage :
	public CWizard97InteriorPageImpl<CAppFolderPage>,
	public CTestWizardInfoRef
{
protected:
	// Typedefs
	typedef CAppFolderPage thisClass;
	typedef CWizard97InteriorPageImpl<CAppFolderPage> baseClass;

	// Data members
	CStatic m_labelSaveFileName;
	CEdit m_editPath;
public:
	// Constructor
	CAppFolderPage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title)
	{
		baseClass::SetHeaderTitle(_T("Application Folder"));
		baseClass::SetHeaderSubTitle(_T("Please check desinaion folder path."));
	}

	// Message Handlers
	enum { IDD = IDD_DLG_APP_FOLDER };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

		CHAIN_MSG_MAP(baseClass)
		COMMAND_HANDLER(IDC_BTN_FOLDER_BROWSE, BN_CLICKED, OnBnClickedBtnFolderBrowse)
		COMMAND_HANDLER(IDC_EDIT_DEST_FOLDER, EN_CHANGE, OnEnChangeEditDestFolder)
		COMMAND_HANDLER(IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER, BN_CLICKED, OnBnClickedRadioInstallProductForCurrentUser)
		COMMAND_HANDLER(IDC_RADIO_INSTALL_PRODUCT_FOR_ALL_USERS, BN_CLICKED, OnBnClickedRadioInstallProductForAllUsers)
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
	LRESULT OnBnClickedBtnFolderBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEnChangeEditDestFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioInstallProductForCurrentUser(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedRadioInstallProductForAllUsers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	int m_nInstallProductFor;
};

