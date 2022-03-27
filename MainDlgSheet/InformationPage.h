#pragma once


// CInformationPage dialog

class CInformationPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CInformationPage)

public:
	CInformationPage();
	virtual ~CInformationPage();

// Dialog Data
	enum { IDD = IDD_INSTALLER_WIZARD_DIALOG_INFORMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	void GetParams();
	
	virtual BOOL OnSetActive();
};
