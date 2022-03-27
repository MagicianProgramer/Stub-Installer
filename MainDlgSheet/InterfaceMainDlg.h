#pragma once
#include "afxwin.h"


// CInterfaceMainDlg dialog

class CInterfaceMainDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInterfaceMainDlg)

public:
	CInterfaceMainDlg();
	virtual ~CInterfaceMainDlg();

	BOOL m_bRefresh;
// Dialog Data
	enum { IDD = IDD_DLG_INTERFACE_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedCheckDialogsToShowWelcome();
	afx_msg void OnBnClickedCheckDialogsToShowDestinationFolder();
	afx_msg void OnBnClickedCheckDialogsToShowLicenseAgreement();
	afx_msg void OnBnClickedCheckDialogsToShowAdditionalTasks();
	afx_msg void OnBnClickedCheckDialogsToShowReadme();
	afx_msg void OnBnClickedCheckDialogsToShowReadyToInstall();
	afx_msg void OnBnClickedCheckDialogsToShowUserInformation();
	afx_msg void OnBnClickedCheckDialogsToShowFinish();
	afx_msg void OnBnClickedCheckShowCopyrightAtWelcomeDialog();
	afx_msg void OnBnClickedCheckHideFileNamesDuringCopyProcess();
	afx_msg void OnCbnSelchangeComboInterface();
	afx_msg void OnBnClickedBtnLogoBitmap();
	afx_msg void OnCbnSelchangeComboLogoBitmap();
	afx_msg void OnCbnEditchangeComboLogoBitmap();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	INT m_bWelcom;
	INT m_bLicenseAgreement;
	INT m_bReadme;
	INT m_bUserInfo;
	INT m_bDestinationFolder;
	INT m_bAdditionalTasks;
	INT m_bReadyInstall;
	INT m_bFinish;
	INT m_bShowCopyright;
	INT m_bHidefile;
	INT m_bRunAsAdmin;
	INT m_nInterface;
	CString m_strLogoBitmap;

	void SetComponentFromInterfaceSel(INT nSel);

protected:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedBtnOptionsLicfileOpen();
	afx_msg void OnBnClickedBtnOptionsReadmeOpen();
	afx_msg void OnEnChangeEditOptionsLicfile();
	afx_msg void OnEnChangeEditOptionsReadmefile();
	afx_msg void OnBnClickedCheckOptionsShowAcceptAgree();
	afx_msg void OnEnChangeEditGraphicWizardbitmap();
	afx_msg void OnEnChangeEditGraphicHeaderbitmap();
	afx_msg void OnBnClickedBtnWizardbitmapOpen();
	afx_msg void OnBnClickedBtnGraphicHeaderbitmapOpen();

private:
	INT m_nChkOptionsShowAgree;
	CString m_strLicFile;
	CString m_strReadMeFile;
	CString m_strWizardBmp;
	CString m_strHdrBmp;
	CComboBox m_ctrlInterfaceCmb;
	CButton m_ctrlShowWelcome;
	CButton m_ctrlShowLicense;
	CButton m_ctrlShowReadme;
	CButton m_ctrlShowUser;
	CButton m_ctrlShowDest;
	CButton m_ctrlShowTask;
	CButton m_ctrlShowReady;
	CButton m_ctrlShowFinish;
	CButton m_ctrlOptAccept;
	CButton m_ctrlOptCopy;
	CButton m_ctrlOptHide;
	CComboBox m_ctrlLogoBitmapCmb;
	CButton m_ctrlLogoBitmapBtn;
private:
	afx_msg void OnNMClickSyslinkViewSample(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkUseDefaultBitmaps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslinkTemplateEditor(NMHDR *pNMHDR, LRESULT *pResult);

	CMFCEditBrowseCtrl m_InterfaceOptionsLicFileEditCtrl;
	CMFCEditBrowseCtrl m_InterfaceOptionsReadmeFileEditCtrl;

	CMFCEditBrowseCtrl m_GraphicHeaderBmpEditCtrl;
	CMFCEditBrowseCtrl m_GraphicWzdBmpEditCtrl;
};
