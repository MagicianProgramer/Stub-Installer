#pragma once
#include "afxcmn.h"


// CInterfaceSelectFileFolderDlg dialog

class CInterfaceSelectFileFolderDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInterfaceSelectFileFolderDlg)

public:
	CInterfaceSelectFileFolderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInterfaceSelectFileFolderDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SELECT_FILE_FOLDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString m_strSectedFullName;
	void SetInitPathStr(CString strPath);
private:
	void InitTree();
	void GetFullNameFromHTreeItem(HTREEITEM hitem, CString &strFullName);
	HTREEITEM GetHTreeItemFromDestName(CString strDestFullName, BOOL  bFromGlbal);
	void SetTreeCtrlItemsFromGlobalVariable();
protected:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CImageList m_imgList;
	CTreeCtrl m_ctrlTreeFileFolder;
	afx_msg void OnTvnSelchangedTreeSelectFileFolder(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
};
