
#ifndef __TestWizardWelcomePage_h__
#define __TestWizardWelcomePage_h__

#include "TestWizardInfo.h"

class CTestWizardSheet;

class CTestWizardWelcomePage :
	public CWizard97ExteriorPageImpl<CTestWizardWelcomePage>,
	public CTestWizardInfoRef
{
protected:
// Typedefs
	typedef CTestWizardWelcomePage thisClass;
	typedef CWizard97ExteriorPageImpl<CTestWizardWelcomePage> baseClass;

// Data members
	bool m_allowWelcomeToHide;

public:
// Constructors
	CTestWizardWelcomePage(_U_STRINGorID title = (LPCTSTR)NULL) :
		baseClass(title),
		m_allowWelcomeToHide(true)
	{
		m_pParent = NULL;	
	}

// Message Handlers
	enum { IDD = IDD_WIZ97_WELCOME };
	BEGIN_MSG_MAP(thisClass)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)

		CHAIN_MSG_MAP(baseClass)
		NOTIFY_HANDLER(IDC_SYSLINK_COMPANY, NM_CLICK, OnBnClickedSysLink)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

// Helper methods
	void InitializeControls(void);
	void InitializeValues(void);
	bool StoreValues(void);

// Overrides from base class
	int OnSetActive();
	int OnWizardNext();
	void OnHelp();

	LRESULT OnBnClickedSysLink(int , LPNMHDR , BOOL&);

public:
	CTestWizardSheet* m_pParent;
};

#endif // __TestWizardWelcomePage_h__
