#pragma once


// CInterfacePage dialog

class CInterfacePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfacePage)

public:
	CInterfacePage();
	virtual ~CInterfacePage();

// Dialog Data
	enum { IDD = IDD_INSTALLER_WIZARD_DIALOG_INTERFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRadioWizardNoInterface();
	afx_msg void OnBnClickedRadioWizardClassicWizard();
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bModernInterface;
	CImage m_imgModernInterface;
	CImage m_imgWizardInterface;
	HDC m_hDestDC;
	int m_nDW;
	int m_nDH;
public:
	void GetParams();
	
	virtual BOOL OnSetActive();
};
