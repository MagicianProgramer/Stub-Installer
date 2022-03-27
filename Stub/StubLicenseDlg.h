#pragma once
#include "afxcmn.h"


// CStubLicenseDlg dialog

class CStubLicenseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStubLicenseDlg)

public:
	CStubLicenseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStubLicenseDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STUB_LICENSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetViewLicense(BYTE* pLicBuff);
private:
	CRichEditCtrl m_richLicense;
	BYTE* m_pBuffer;
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnStubLicenseBack();
};
