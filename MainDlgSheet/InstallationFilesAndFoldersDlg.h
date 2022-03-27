#pragma once
#include "afxcmn.h"
#include "../common/common.h"
#include "CCustomTreeCtrl.h"
#include "MltiTree.h"

// CInstallationFilesAndFoldersDlg dialog

class CInstallationFilesAndFoldersDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CInstallationFilesAndFoldersDlg)

public:
	CInstallationFilesAndFoldersDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallationFilesAndFoldersDlg();
	BOOL m_bRefresh;
// Dialog Data
	enum { IDD = IDD_DLG_INSTALLATION_FILESANDFOLDERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
		
private:
	CImageList m_imgList;
	CCustomTreeCtrl m_LisctrlTreeInstallationFiles; 
	/*CMultiTree m_LisctrlTreeInstallationFiles;*/
	HTREEITEM m_hSelectedItem;
protected:
	afx_msg void OnBnClickedBtnAddFolder();
	afx_msg void OnBnClickedBtnSourceDirectory();
	afx_msg void OnBnClickedBtnInstallationDirectory();
	afx_msg void OnBnClickedBtnAddFiles();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnRemove();
	afx_msg void OnNMClickSyslinkFileOptions(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSourceDirectory();
	afx_msg void OnEnChangeEditInstallationDirectory();
	
	afx_msg void OnIsfilesSetasmainexecutable();
	afx_msg void OnIsfilesCreateshortcut();
	afx_msg void OnIsfilesEdit();
	afx_msg void OnIsfilesNewfolder();
	afx_msg void OnIsfilesOpenfilelocation();
	afx_msg void OnIsfilesRemove();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();

	afx_msg void OnRclickTreeInstallationFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkTreeInstallationFiles(NMHDR *pNMHDR, LRESULT *pResult);
	
private:
	INT InsertTreeCtrlFromDirByHItem(CString strSrcDir, CString strDestDir, HTREEITEM hParent = NULL, HTREEITEM hAfterItem = NULL);
	void InsertTreeCtrlFromDirectroy(CString strSrcDir, CString strDestDir, HTREEITEM hAfterItem = NULL);
	HTREEITEM GetParentItemFromDestName(CString strDestFullName, BOOL bFromGlbal = FALSE);
	void SetTreeCtrlItemsFromGlobalVariable();
	void GetFullNameFromHTreeItem(HTREEITEM hitem, CString &strFullName);
	INT GetIndexFromDestFullName(CString strDestFullName);
	void GetChildList(HTREEITEM hitem, vector<INT> &childlist, BOOL bSub = FALSE);
	///////////////////////////////////////////////////
	//int GetIndexFromHTreeItem_in_SameLevel(HTREEITEM hitem);
	void InitTree();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void AddUninstaller(BOOL bAdd);
	void AddUpdater(BOOL bAdd);
	void RemoveItem();
	void GetRemovingItems(vector<HTREEITEM>& vecItem);
public:
	void SettingInstallFilesFromWizard();
	void AddFontFile(CString strFontFileFullPath);


	INT CheckFileAlreadyAdded(CString strDestFullName);	

	void SetMainExcutablesIntoCombobox();

protected:
	CComboBox m_ctrlComboMainExecutable;
	afx_msg void OnCbnSelchangeComboMainExecutable();
	afx_msg void OnBnClickedCheckInstallation64bitApp();
	CMFCEditBrowseCtrl m_SrcDirEditCtrl;
	CMFCEditBrowseCtrl m_InstallationDirEditCtrl;
};
