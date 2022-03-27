#pragma once
#include "afxwin.h"


// CGeneralOutputDlg dialog

class CGeneralOutputDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneralOutputDlg)

public:
	CGeneralOutputDlg();
	virtual ~CGeneralOutputDlg();

// Dialog Data
	enum { IDD = IDD_DLG_GENERAL_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnCbnSelchangeComboPackageType();
	afx_msg void OnEnChangeEditDataFileName();
	afx_msg void OnEnChangeEditOutputFolder();
	afx_msg void OnEnChangeEditOutputFileName();
	afx_msg void OnEnChangeEditOutputIconFile();
	afx_msg void OnBnClickedBtnOutputFolder();
	afx_msg void OnBnClickedBtnOutputIconFile();
	afx_msg void OnPaint();
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();

private:	
	CStatic m_ctrlPicIcon;
	void DrawIcon2PicCtrl(CString strPathName);
	CImage	m_img;
	BOOL m_bLoadImg;
	
	CComboBox m_ctrlComboPackage;
protected:
	CMFCEditBrowseCtrl m_outputFolderEditCtrl;
	CMFCEditBrowseCtrl m_outputIconFileEditCtrl;
};
