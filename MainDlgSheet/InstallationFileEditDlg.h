#pragma once
#include "afxwin.h"
#include "../common/common.h"

// CInstallationFileEditDlg dialog

enum VIEW_TYPE{
	CASE_ADD_FOLDER = 0,
	CASE_ADD_FILE,
	CASE_FILE_EDIT,
	CASE_FOLDER_EDIT,
};

class CInstallationFileEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInstallationFileEditDlg)

public:
	CInstallationFileEditDlg(CWnd* pParent = NULL);   // standard constructor
	CInstallationFileEditDlg(INT me, CString strSrcName, CString strDstName);   // standard constructor
	virtual ~CInstallationFileEditDlg();

// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_FILES_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void InitFunction();

private:
	VIEW_TYPE m_type;

	CComboBox m_ctrlComboFileExist;
	CComboBox m_ctrlComboDestFolder;
	CButton m_ctrlCheckRemove;
	CButton m_ctrlCheckInclude;

	INT m_nFileExist;
	BOOL m_bRemoveUninstall;
	BOOL m_bIncludeLastFolder;

public:
	CString m_strDstDirName;
	CString m_strSrcName;
	CString m_strDstFileName;
	void SetData(VIEW_TYPE nType, CString strSrc, CString strDest, INT nFileExsit = 1, BOOL bRemoveUninstall = TRUE, BOOL bIncludeLast = FALSE);
};
