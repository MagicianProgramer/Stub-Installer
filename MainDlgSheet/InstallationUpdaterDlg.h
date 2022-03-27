#pragma once


// CInstallationUpdaterDlg dialog

class CInstallationUpdaterDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInstallationUpdaterDlg)

public:
	CInstallationUpdaterDlg();
	virtual ~CInstallationUpdaterDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_UPDATER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual BOOL OnSetActive();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedCheckInstallationUpdaterIncludeUpdater();
	afx_msg void OnEnChangeEditInstallationUpdaterUrl();
	afx_msg void OnBnClickedBtnInstallationUpdaterUrlOpen();
	afx_msg void OnBnClickedCheckInstallationUpdaterProductNameInUpdater();
	afx_msg void OnBnClickedCheckInstallationUpdaterAllowUsersToChangeOptions();
	afx_msg void OnBnClickedCheckInstallationUpdaterUseDownloader();
	afx_msg void OnBnClickedCheckInstallationUpdaterAutoCheckForUpdates();
	afx_msg void OnCbnSelchangeComboInstallationUpdaterAutoCheckMethod();
	afx_msg void OnBnClickedCheckInstallationUpdaterInstallUpdatesSilently();
	afx_msg void OnBnClickedCheckInstallationUpdaterUseSmartNotification();
	afx_msg void OnCbnSelchangeComboInstallationUpdaterSmartNotificationMethod();
	afx_msg void OnEnChangeEditInstallationUpdaterAdditionalParams();
	afx_msg void OnEnChangeEditInstallationUpdaterPassParamsToNewVersionUrl();
	virtual BOOL OnSetActive();

public:
	BOOL m_bRefresh;

private:
	void SetComponents();
};
