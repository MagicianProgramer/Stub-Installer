#pragma once


// CInterfaceLanguages dialog

class CInterfaceLanguagesDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfaceLanguagesDlg)

public:
	CInterfaceLanguagesDlg();
	virtual ~CInterfaceLanguagesDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INTERFACE_LANGUAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
