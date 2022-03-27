#pragma once


// CGenerateSNDlg dialog

class CGenerateSNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGenerateSNDlg)

public:
	CGenerateSNDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGenerateSNDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GENERATE_SERIAL_NUMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedBtnGenerateSerialNumberAdd();
	afx_msg void OnBnClickedBtnGenerateSerialNumberGen();
	afx_msg void OnBnClickedBtnGenerateSerialNumberClear();
	virtual BOOL OnInitDialog();
public:
	CStringA m_szLicenses;
	
};
