
// InstallationFilesAndFoldersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationFilesAndFoldersDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

#include "ProjectOptionsDlg.h"

#include "PopupMenu.h"
#include "InstallationFileEditDlg.h"
#include "InstallationAddNewfolderDlg.h"

#define UNISTALL_FLG		-1
#define UPDATER_FLG			-2
#define UPDATER_INI_FLG		-3
// CInstallationFilesAndFoldersDlg dialog
IMPLEMENT_DYNAMIC(CInstallationFilesAndFoldersDlg, CPropertyPage)

CInstallationFilesAndFoldersDlg::CInstallationFilesAndFoldersDlg(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CInstallationFilesAndFoldersDlg::IDD)
{
	m_bRefresh = TRUE;
	m_hSelectedItem = 0;
}

CInstallationFilesAndFoldersDlg::~CInstallationFilesAndFoldersDlg()
{
}

void CInstallationFilesAndFoldersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_INSTALLATION_FILES, m_LisctrlTreeInstallationFiles);
	DDX_Control(pDX, IDC_COMBO_MAIN_EXECUTABLE, m_ctrlComboMainExecutable);
	DDX_Control(pDX, IDC_EDIT_SOURCE_DIRECTORY, m_SrcDirEditCtrl);
	//DDX_Control(pDX, IDC_MFCEDITBROWSE_INSTALLATION_FILES_AND_FOLDERS_SRC_DIR, m_SrcDirEditCtrl);
	DDX_Control(pDX, IDC_EDIT_INSTALLATION_DIRECTORY, m_InstallationDirEditCtrl);
}


BEGIN_MESSAGE_MAP(CInstallationFilesAndFoldersDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_ADD_FOLDER, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnAddFolder)
	ON_BN_CLICKED(IDC_BTN_SOURCE_DIRECTORY, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnSourceDirectory)
	ON_BN_CLICKED(IDC_BTN_INSTALLATION_DIRECTORY, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnInstallationDirectory)
	ON_BN_CLICKED(IDC_BTN_ADD_FILES, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnAddFiles)
	ON_BN_CLICKED(IDC_BTN_ADD_EDIT, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_ADD_DELETE, &CInstallationFilesAndFoldersDlg::OnBnClickedBtnRemove)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_FILE_OPTIONS, &CInstallationFilesAndFoldersDlg::OnNMClickSyslinkFileOptions)
	ON_EN_CHANGE(IDC_EDIT_SOURCE_DIRECTORY, &CInstallationFilesAndFoldersDlg::OnEnChangeEditSourceDirectory)
	ON_EN_CHANGE(IDC_EDIT_INSTALLATION_DIRECTORY, &CInstallationFilesAndFoldersDlg::OnEnChangeEditInstallationDirectory)
	ON_COMMAND(ID_ISFILES_SETASMAINEXECUTABLE, &CInstallationFilesAndFoldersDlg::OnIsfilesSetasmainexecutable)
	ON_COMMAND(ID_ISFILES_CREATESHORTCUT, &CInstallationFilesAndFoldersDlg::OnIsfilesCreateshortcut)
	ON_COMMAND(ID_ISFILES_EDIT, &CInstallationFilesAndFoldersDlg::OnIsfilesEdit)
	ON_COMMAND(ID_ISFILES_NEWFOLDER, &CInstallationFilesAndFoldersDlg::OnIsfilesNewfolder)
	ON_COMMAND(ID_ISFILES_OPENFILELOCATION, &CInstallationFilesAndFoldersDlg::OnIsfilesOpenfilelocation)
	ON_COMMAND(ID_ISFILES_REMOVE, &CInstallationFilesAndFoldersDlg::OnIsfilesRemove)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_INSTALLATION_FILES, &CInstallationFilesAndFoldersDlg::OnDblclkTreeInstallationFiles)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_INSTALLATION_FILES, &CInstallationFilesAndFoldersDlg::OnRclickTreeInstallationFiles)
	ON_CBN_SELCHANGE(IDC_COMBO_MAIN_EXECUTABLE, &CInstallationFilesAndFoldersDlg::OnCbnSelchangeComboMainExecutable)
	ON_BN_CLICKED(IDC_CHECK_INSTALLATION_64BIT_APP, &CInstallationFilesAndFoldersDlg::OnBnClickedCheckInstallation64bitApp)
END_MESSAGE_MAP()


// CInstallationFilesAndFoldersDlg message handlers

BOOL ExceptExtension(CString strName)
{
	CString strExt = strName.Right(strName.GetLength() - strName.ReverseFind(L'.'));
	CString strExcept = g_fileOptions.wszDoNotAddFilesWithExtensions;

	if (strExcept.Find(strExt) >= 0)
		return TRUE;		
	else
		return FALSE;
}

BOOL CheckingFileSize(CString strFile)
{
	BOOL bRet = FALSE;
	CFile fp;
	if(fp.Open(strFile, CFile::modeRead|CFile::shareDenyNone))
	{
		if(fp.GetLength())	bRet = TRUE;
		fp.Close();
	}
	return bRet;
}


BOOL CInstallationFilesAndFoldersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	InitTree();


	m_SrcDirEditCtrl.EnableFolderBrowseButton();
	m_InstallationDirEditCtrl.EnableBrowseButton();
	/*m_SrcDirEditCtrl.SetBrowseButtonImage(AfxGetApp()->LoadIcon(IDI_ICON_FILE));*/

	//set combo box control to read only method//
	HWND h_wnd = ::FindWindowEx(m_ctrlComboMainExecutable.m_hWnd, NULL, _T("Edit"), NULL);
	((CEdit*)CWnd::FromHandle(h_wnd))->SetReadOnly(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInstallationFilesAndFoldersDlg::OnBnClickedBtnAddFolder()
{
	CFolderPickerDialog folderpickerdlg(NULL, OFN_READONLY | /*OFN_OVERWRITEPROMPT |*/ OFN_ALLOWMULTISELECT, this);
	folderpickerdlg.m_ofn.lpstrTitle  = _T("Add Folders ...");
	if (folderpickerdlg.DoModal() == IDOK)
	{
		//...
		CString strFolder = folderpickerdlg.GetPathName();
		CInstallationFileEditDlg editdlg;

		HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
		if(hitem != NULL)
		{
			CString strFull;
			GetFullNameFromHTreeItem(hitem, strFull);
			INT index = GetIndexFromDestFullName(strFull);
			if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == 0)//File
			{
				hitem = m_LisctrlTreeInstallationFiles.GetParentItem(hitem);
				GetFullNameFromHTreeItem(hitem, strFull);
				index = GetIndexFromDestFullName(strFull);

				editdlg.SetData(CASE_ADD_FOLDER, strFolder, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
			}
			else
				editdlg.SetData(CASE_ADD_FOLDER, strFolder, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
		}
		else
		{
			CString strInsDir = L"#INSTALL_DIR#";
			editdlg.SetData(CASE_ADD_FOLDER, strFolder, strInsDir);
		}

		//////////////////////////////////////////
		if (editdlg.DoModal() == IDOK)
		{
			//////////////////////////
			CString strDstFullName = editdlg.m_strDstDirName;
			strDstFullName.Replace(L"\\\\", L"\\");
			while(1)
			{
				INT k = strDstFullName.Find(L"\\\\");
				if (k < 0)	break;
				strDstFullName.Replace(L"\\\\", L"\\");
			}
			if (strDstFullName.ReverseFind(L'\\') == strDstFullName.GetLength() - 1)
			{
				strDstFullName = strDstFullName.Left(strDstFullName.ReverseFind(L'\\'));
			}

			////////////////////////////
			HTREEITEM hAfterItem = TVI_FIRST;
			InsertTreeCtrlFromDirectroy(editdlg.m_strSrcName, strDstFullName, hAfterItem);
		}
	}
}

void CInstallationFilesAndFoldersDlg::OnBnClickedBtnSourceDirectory()
{
	CFolderPickerDialog folderpickerdlg(NULL, OFN_READONLY | OFN_OVERWRITEPROMPT , this);
	folderpickerdlg.m_ofn.lpstrTitle  = _T("Select Source Directory ...");
	if (folderpickerdlg.DoModal() == IDOK)
	{
		CString strPathName;
		strPathName = folderpickerdlg.GetFolderPath();
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszSourceDirectory, strPathName.GetBuffer());
		SetDlgItemText(IDC_EDIT_SOURCE_DIRECTORY, strPathName);

		//insert into tree//
		m_LisctrlTreeInstallationFiles.DeleteAllItems();
		g_globaldata.install_sh2.filesandfolders.vecInstallationData.clear();
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, L"");

		/*HTREEITEM hItem = m_LisctrlTreeInstallationFiles.GetChildItem(0);
		RemoveItem(hItem);*/
		InsertTreeCtrlFromDirectroy(strPathName, L"#INSTALL_DIR#");

		AddUninstaller(g_globaldata.install_sh2.uninst.chkIncludeUninstaller);
		AddUpdater(g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool);
	}
}


void CInstallationFilesAndFoldersDlg::OnBnClickedBtnInstallationDirectory()
{
	
}


void CInstallationFilesAndFoldersDlg::OnBnClickedBtnAddFiles()
{
	CString sFilter = L"All Files(*.*)|*.*|";
	CFileDialog filedlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_READONLY /*| OFN_ALLOWMULTISELECT*/, sFilter, this);
	filedlg.m_ofn.lpstrTitle = L"Add Files ...";
	if (filedlg.DoModal() == IDOK)
	{
		//...
		CString strFile;
		CStringArray strArray;

		POSITION pos;
		pos = filedlg.GetStartPosition();

		INT nCnt = 0;

		while(pos)
		{	
			nCnt ++;
			strFile = filedlg.GetNextPathName(pos);
			strArray.Add(strFile);
		}

		/*strFile.Format(L"%d files", nCnt);*/
		

		CInstallationFileEditDlg editdlg;

		HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();

		if(hitem != NULL)
		{
			CString strFull;
			GetFullNameFromHTreeItem(hitem, strFull);
			INT index = GetIndexFromDestFullName(strFull);
			if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == 0)//File
			{
				hitem = m_LisctrlTreeInstallationFiles.GetParentItem(hitem);
				GetFullNameFromHTreeItem(hitem, strFull);
				index = GetIndexFromDestFullName(strFull);
				editdlg.SetData(CASE_ADD_FILE, strFile, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
			}
			else
				editdlg.SetData(CASE_ADD_FILE, strFile, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
		}
		else
		{
			CString strInsDir = L"#INSTALL_DIR#";
			editdlg.SetData(CASE_ADD_FILE, strFile, strInsDir);
		}

		if (editdlg.DoModal() == IDOK)
		{
			CString strDstFullName = editdlg.m_strDstDirName;
			//////////////////////////
			strDstFullName.Replace(L"\\\\", L"\\");
			while(1)
			{
				INT k = strDstFullName.Find(L"\\\\");
				if (k < 0)	break;
				strDstFullName.Replace(L"\\\\", L"\\");
			}
			if (strDstFullName.ReverseFind(L'\\') == strDstFullName.GetLength() - 1)
			{
				strDstFullName = strDstFullName.Left(strDstFullName.ReverseFind(L'\\'));
			}

			////////////////////////////
			hitem = GetParentItemFromDestName(strDstFullName);
			for (INT i = 0 ; i < nCnt ; i ++)
			{
				//checking hidden files//
				if (g_fileOptions.chkDoNotAddHiddenFilesAndFolders && 
					(GetFileAttributes(strArray[i]) & FILE_ATTRIBUTE_HIDDEN))
					continue;

				//checking extensions//
				if (wcslen(g_fileOptions.wszDoNotAddFilesWithExtensions) > 0 
					&& ExceptExtension(strArray[i]))
					continue;

				//checking file size
				if (g_fileOptions.chkDoNotAddZeroSizeFile && !CheckingFileSize(strArray[i]))
					continue;

				ADDINSTALLATIONFILES val;
				val.nChkDir = 0;
				wsprintf(val.wszSrcFullName, L"%s", strArray[i]);

				CString strF = strArray[i];
				strF = strF.Right(strF.GetLength() - strF.ReverseFind('\\') - 1);
				wsprintf(val.wszDestName, L"%s", strF);
				wsprintf(val.wszDestFullName, L"%s\\%s", strDstFullName, strF);
				INT nIdx = CheckFileAlreadyAdded(strDstFullName + L"\\" + strF);
				if (nIdx == -1)//new
				{
					g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);

					//checking//
					if (!g_fileOptions.chkDoNotShowFilesTreeView)
						m_LisctrlTreeInstallationFiles.InsertItem(strF, 0, 0, hitem, NULL);
				}
				else if (nIdx == -2)//reject
				{
				}
				else//already exist//
				{
					g_globaldata.install_sh2.filesandfolders.vecInstallationData[nIdx] = val;
					if (!g_fileOptions.chkDoNotShowFilesTreeView)
						SetTreeCtrlItemsFromGlobalVariable();
				}
				
			}
			//SetTreeCtrlItemsFromGlobalVariable();		
		}
	}
}

INT CInstallationFilesAndFoldersDlg::CheckFileAlreadyAdded(CString strDestFullName)
{
	INT nRet = -1;
	for (INT i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
	{
		CString strname = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
		if (strname == strDestFullName)
		{
			nRet = i;
			break;
		}
	}

	if (nRet >= 0)
	{
		if(MessageBox(strDestFullName + L" already exists. Do you want to replace it?", INSTALLER_NAME, MB_YESNO | MB_ICONWARNING) == IDYES)
		{
			
		}
		else
		{
			nRet = -2;
		}
	}

	return nRet;
}


void CInstallationFilesAndFoldersDlg::OnBnClickedBtnEdit()
{
	HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
	
	if(hitem)
	{
		CString strFull;
		INT index;
		GetFullNameFromHTreeItem(hitem, strFull);
		index = GetIndexFromDestFullName(strFull);
		if (index < 0)		return;
		
		CInstallationFileEditDlg dlg;
		if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == 0)//File
			dlg.SetData(CASE_FILE_EDIT, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszSrcFullName, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
		else
			dlg.SetData(CASE_FOLDER_EDIT, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszSrcFullName, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);

		if (dlg.DoModal() == IDOK)
		{
			CString strDstFullName = dlg.m_strDstDirName;
			CString strDstFileName = dlg.m_strDstFileName;
			CString strSrcFullName = dlg.m_strSrcName;

			CString strDT;
			strDT.Format(L"%s", g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);
			
			//refresh list item//
			/*m_ctrlTreeInstallationFiles.DeleteItem(hitem);*/

			//global data//
			if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == 0)
			{
				CString strOldeDstFullName;
				strOldeDstFullName.Format(L"%s", g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);

				INT len = strOldeDstFullName.GetLength();
				for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
				{
					CString strT;
					strT.Format(L"%s", g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName);
					if(wcscmp(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName, strT) == 0)
					{
						wsprintf(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, L"%s\\%s", strDstFullName, strDstFileName);
					}
				}
				wsprintf(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestName, L"%s", strDstFileName);
			}
			else//folder
			{
				
				CString strOldeDstFullName;
				strOldeDstFullName.Format(L"%s", g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestFullName);

				INT len = strOldeDstFullName.GetLength();
				for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
				{
					CString strT;
					strT.Format(L"%s", g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName);
					if(wcscmp(strOldeDstFullName, strT.Left(len)) == 0)
					{
						wsprintf(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, L"%s%s", strDstFullName, strT.Right(strT.GetLength() - len));
					}
				}

				wsprintf(g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestName, L"%s", strDstFileName);

				m_LisctrlTreeInstallationFiles.DeleteItem(hitem);
			}

			if (!g_fileOptions.chkDoNotShowFilesTreeView)
				SetTreeCtrlItemsFromGlobalVariable();
		}
	}
}

void CInstallationFilesAndFoldersDlg::GetChildList(HTREEITEM hitem, vector<INT> &childlist, BOOL sub)
{
	if(hitem == NULL)
		return;
	
	while(hitem)
	{
		CString strItem;
		GetFullNameFromHTreeItem(hitem, strItem);
		INT index = GetIndexFromDestFullName(strItem);
		if (index >= 0)
		{
			GetChildList(m_LisctrlTreeInstallationFiles.GetChildItem(hitem), childlist, TRUE);
			childlist.push_back(index);

			if(sub == FALSE)
				break;
		}
		hitem = m_LisctrlTreeInstallationFiles.GetNextItem(hitem, TVGN_NEXT);
	}

	
}


void CInstallationFilesAndFoldersDlg::GetRemovingItems(vector<HTREEITEM>& vecItem)
{
	HTREEITEM hSelectedItem = m_LisctrlTreeInstallationFiles.GetFirstSelectedItem();
	while (hSelectedItem) 
	{
		BOOL bRemove = TRUE;

		HTREEITEM hParentItem = m_LisctrlTreeInstallationFiles.GetParentItem(hSelectedItem);
		while (hParentItem)
		{
			for (INT i = 0; i < vecItem.size(); i++)
			{
				if (vecItem[i] == hParentItem)
				{
					bRemove = FALSE;
					break;
				}
			}

			if (!bRemove)	break;
			hParentItem = m_LisctrlTreeInstallationFiles.GetParentItem(hParentItem);
		}

		if(bRemove)		vecItem.push_back(hSelectedItem);
		hSelectedItem = m_LisctrlTreeInstallationFiles.GetNextSelectedItem(hSelectedItem);
	}
}


void CInstallationFilesAndFoldersDlg::RemoveItem()
{
	vector<HTREEITEM> vecitem;

	/*m_LisctrlTreeInstallationFiles.CollectSelectedItems();
	for (INT i = 0; i < m_LisctrlTreeInstallationFiles.m_aSelectedItems.GetSize(); i++)
	{
		vecitem.push_back(m_LisctrlTreeInstallationFiles.m_aSelectedItems[i]);
	}*/

	/*HTREEITEM hSelectedItem = m_LisctrlTreeInstallationFiles.GetFirstSelectedItem();
	while (hSelectedItem) {
		vecitem.push_back(hSelectedItem);
		hSelectedItem = m_LisctrlTreeInstallationFiles.GetNextSelectedItem(hSelectedItem);
	}*/

	//get all removing items//
	GetRemovingItems(vecitem);

	//remove//
	for (INT k = 0; k < vecitem.size(); k++)
	{
		HTREEITEM hItem;
		hItem = vecitem[k];
		CString strItem;
		GetFullNameFromHTreeItem(hItem, strItem);
		//INT index = GetIndexFromDestFullName(strItem);
		///////////////////////////////
		vector<INT> DelList;
		GetChildList(hItem, DelList);

		//never delete install dir(root)//
		BOOL bRootDir = FALSE;
		for (INT i = 0; i < DelList.size(); i++)
		{
			CString strName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[DelList[i]].wszDestFullName;
			if (strName == L"#INSTALL_DIR#")
			{
				DelList.erase(DelList.begin() + i);
				bRootDir = TRUE;
				break;
			}
		}

		/////////////////////////////////////
		vector<ADDINSTALLATIONFILES> temVec;
		temVec.clear();

		for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
		{
			BOOL bDel = FALSE;
			for (INT j = 0 ; j < DelList.size() ; j ++)
			{
				if(i == DelList[j])
				{
					bDel = TRUE;
					break;
				}
			}

			if(bDel == FALSE)
			{
				temVec.push_back(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i]);
			}
			else
			{
				if (wcscmp(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName) == 0)
				{
					wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, L"");
					/*SetDlgItemText(IDC_EDIT_MAIN_EXECUTABLE, L"");*/

					for (INT k = 0; k < g_globaldata.install_sh2.shortcuts.vecDetail.size(); k++)
					{
						ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[k];
						if (wcscmp(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, value.wszTargetFile) == 0)
						{
							g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + k);
							k--;
						}
					}

					CWnd* pParentWnd = GetParent();
					if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_SHORTCUT_REFRESH);
				}
			}
		}

		g_globaldata.install_sh2.filesandfolders.vecInstallationData.clear();

		for (INT i = 0 ; i < temVec.size() ; i ++)
			g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(temVec[i]);


		if (!bRootDir)
		{
			if (m_LisctrlTreeInstallationFiles.GetItemData(hItem) == UNISTALL_FLG)
			{
				g_globaldata.install_sh2.uninst.chkIncludeUninstaller = FALSE;
				CWnd* pParentWnd = GetParent();
				if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UNINSTALLATION_REFRESH);
			}
			else if (m_LisctrlTreeInstallationFiles.GetItemData(hItem) == UPDATER_FLG)
			{
				g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool = FALSE;
				CWnd* pParentWnd = GetParent();
				if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UPDATER_REFRESH);
			}
			else if (m_LisctrlTreeInstallationFiles.GetItemData(hItem) == UPDATER_INI_FLG)
			{
				g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool = FALSE;
				CWnd* pParentWnd = GetParent();
				if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UPDATER_REFRESH);
			}

			m_LisctrlTreeInstallationFiles.DeleteItem(hItem);
		}
		else if(m_LisctrlTreeInstallationFiles.ItemHasChildren(hItem))
		{
			HTREEITEM hChildItem;
			do 
			{
				hChildItem = m_LisctrlTreeInstallationFiles.GetChildItem(hItem);
				if (hChildItem)		
				{
					if (m_LisctrlTreeInstallationFiles.GetItemData(hChildItem) == UNISTALL_FLG)
					{
						g_globaldata.install_sh2.uninst.chkIncludeUninstaller = FALSE;
						CWnd* pParentWnd = GetParent();
						if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UNINSTALLATION_REFRESH);
					}
					else if (m_LisctrlTreeInstallationFiles.GetItemData(hChildItem) == UPDATER_FLG)
					{
						g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool = FALSE;
						CWnd* pParentWnd = GetParent();
						if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UPDATER_REFRESH);
					}
					else if (m_LisctrlTreeInstallationFiles.GetItemData(hChildItem) == UPDATER_INI_FLG)
					{
						g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool = FALSE;
						CWnd* pParentWnd = GetParent();
						if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_UPDATER_REFRESH);
					}
					m_LisctrlTreeInstallationFiles.DeleteItem(hChildItem);
				}
			} while (hChildItem);
		}
	}

	//set main executables into combobox//
	SetMainExcutablesIntoCombobox();
}

void CInstallationFilesAndFoldersDlg::OnBnClickedBtnRemove()
{
	RemoveItem();
}


void CInstallationFilesAndFoldersDlg::OnNMClickSyslinkFileOptions(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CProjectOptionsDlg dlg(1);
	if(dlg.DoModal() == IDOK)
	{
		SaveProjectOptionsIntoRegistry();
	}	
}


void CInstallationFilesAndFoldersDlg::OnEnChangeEditSourceDirectory()
{
	CString strPathName;
	GetDlgItemText(IDC_EDIT_SOURCE_DIRECTORY, strPathName);
	wsprintf(g_globaldata.install_sh2.filesandfolders.wszSourceDirectory, strPathName.GetBuffer());

	if (PathFileExists(strPathName) && !m_bRefresh)
	{
		//insert into tree//
		m_LisctrlTreeInstallationFiles.DeleteAllItems();
		g_globaldata.install_sh2.filesandfolders.vecInstallationData.clear();
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, L"");

		/*HTREEITEM hItem = m_LisctrlTreeInstallationFiles.GetChildItem(0);
		RemoveItem(hItem);*/
		InsertTreeCtrlFromDirectroy(strPathName, L"#INSTALL_DIR#");

		AddUninstaller(g_globaldata.install_sh2.uninst.chkIncludeUninstaller);
		AddUpdater(g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool);
	}	
}


void CInstallationFilesAndFoldersDlg::OnEnChangeEditInstallationDirectory()
{
	CString str;
	GetDlgItemText(IDC_EDIT_INSTALLATION_DIRECTORY, str);
	wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, str.GetBuffer());
}



BOOL CInstallationFilesAndFoldersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	GetDlgItem(IDC_BTN_ADD_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ADD_DELETE)->EnableWindow(FALSE);

	//tree ctrl//
	if (m_bRefresh)
	{
		SetDlgItemText(IDC_EDIT_SOURCE_DIRECTORY, g_globaldata.install_sh2.filesandfolders.wszSourceDirectory);
		SetDlgItemText(IDC_EDIT_INSTALLATION_DIRECTORY, g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory);
		//SetDlgItemText(IDC_EDIT_MAIN_EXECUTABLE, g_globaldata.install_sh2.filesandfolders.wszMainExecutable);
		
		if (!g_fileOptions.chkDoNotShowFilesTreeView)
			SetTreeCtrlItemsFromGlobalVariable();

		AddUninstaller(g_globaldata.install_sh2.uninst.chkIncludeUninstaller);
		AddUpdater(g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool);

		//set main executables into combo box//
		SetMainExcutablesIntoCombobox();

		CheckDlgButton(IDC_CHECK_INSTALLATION_64BIT_APP, g_globaldata.install_sh2.filesandfolders.nChk64BitApplication);


		CString strInstallationDir = g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory;
		g_globaldata.install_sh2.filesandfolders.nChk64BitApplication = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_64BIT_APP);
		if (g_globaldata.install_sh2.filesandfolders.nChk64BitApplication)
		{
			strInstallationDir.Replace(L"#PROGRAM_FILES_X86#", L"#PROGRAM_FILES#");
		}
		else
		{
			strInstallationDir.Replace(L"#PROGRAM_FILES#", L"#PROGRAM_FILES_X86#");
		}

		wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, strInstallationDir);
		SetDlgItemText(IDC_EDIT_INSTALLATION_DIRECTORY, strInstallationDir);

		m_bRefresh = FALSE;
	}

	return CPropertyPage::OnSetActive();
}


void CInstallationFilesAndFoldersDlg::SetTreeCtrlItemsFromGlobalVariable()
{
	m_LisctrlTreeInstallationFiles.DeleteAllItems();

	//insert dir//
	for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
	{
		if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].nChkDir == 1)
		{
			HTREEITEM hitem = GetParentItemFromDestName(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, TRUE);
		}
	}

	//insert file//
	for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
	{
		if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].nChkDir == 0)
		{
			wchar_t szDir[_MAX_INSTALLER_STRING_LEN_];
			wcscpy(szDir, g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName);
			wchar_t* wsz;
			wsz = wcsrchr(szDir, _T('\\'));
			int32_t k = wcslen(szDir) - wcslen(wsz);
			szDir[k] = 0;

			HTREEITEM hitem = GetParentItemFromDestName(szDir, TRUE);
			CString strF = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName;
			strF = strF.Right(strF.GetLength() - strF.ReverseFind('\\') - 1);
			m_LisctrlTreeInstallationFiles.InsertItem(strF, 0, 0, hitem, NULL);
		}
	}

	//expand//
	HTREEITEM hRoots = m_LisctrlTreeInstallationFiles.GetRootItem();

	while(hRoots)
	{
		m_LisctrlTreeInstallationFiles.Expand(hRoots, TVE_EXPAND);
		hRoots = m_LisctrlTreeInstallationFiles.GetNextItem(hRoots, TVGN_NEXT);
	}
}


void CInstallationFilesAndFoldersDlg::InsertTreeCtrlFromDirectroy(CString strSrcDir, CString strDestDir,  HTREEITEM hAfterItem)
{
	HTREEITEM hParentItem;
	hParentItem = GetParentItemFromDestName(strDestDir);
	InsertTreeCtrlFromDirByHItem(strSrcDir, strDestDir, hParentItem, hAfterItem);
	m_LisctrlTreeInstallationFiles.Expand(hParentItem, TVE_EXPAND);

	//set main executables into combo box//
	SetMainExcutablesIntoCombobox();
}

INT GetFileCountInDirectory(CString strDir)
{
	INT nRet = 0;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strDir + L"\\*.*");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())	continue;
		if (finder.IsDirectory())
			nRet = nRet + GetFileCountInDirectory(strDir + L"\\" + finder.GetFileName());
		else
			nRet++;
	}

	return nRet;
}



INT CInstallationFilesAndFoldersDlg::InsertTreeCtrlFromDirByHItem(CString strSrcDir, CString strDestDir, HTREEITEM hParent, HTREEITEM hAfterItem)
{
	INT nRet = 0;

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strSrcDir + L"\\*.*");
	while(bWorking)// add dir
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())	continue;//
		if (finder.IsDirectory())//case directory
		{
			//checking empty directory//
			if (g_fileOptions.chkDoNotAddEmptyFolders && !GetFileCountInDirectory(strSrcDir + L"\\" + finder.GetFileName()))
				continue;

			//checking hidden directory//
			if (g_fileOptions.chkDoNotAddHiddenFilesAndFolders && 
				(GetFileAttributes(strSrcDir + L"\\" + finder.GetFileName()) & FILE_ATTRIBUTE_HIDDEN))
				continue;
			
			//add//
			CString strName = finder.GetFileName();
			ADDINSTALLATIONFILES val;

			val.nChkDir = 1;
			wsprintf(val.wszSrcFullName, L"%s\\%s", strSrcDir, strName);
			wsprintf(val.wszDestName, L"%s", strName);
			wsprintf(val.wszDestFullName, L"%s\\%s", strDestDir, strName);
			g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);
			
			//checking donot show files tree view//
			if (!g_fileOptions.chkDoNotShowFilesTreeView)
			{
				HTREEITEM hItem = m_LisctrlTreeInstallationFiles.InsertItem(strName, 1, 1, hParent, hAfterItem);
				InsertTreeCtrlFromDirByHItem(strSrcDir + L"\\" + strName, strDestDir + L"\\" + strName, hItem);
				hAfterItem = hItem;
			}			
		}
	}

	bWorking = finder.FindFile(strSrcDir + L"\\*.*");
	while(bWorking)//add file
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDots())	continue;//
		if (!finder.IsDirectory())//case file
		{
			//checking hidden file//
			if (g_fileOptions.chkDoNotAddHiddenFilesAndFolders && 
				(GetFileAttributes(strSrcDir + L"\\" + finder.GetFileName()) & FILE_ATTRIBUTE_HIDDEN))
				continue;

			//checking extensions//
			if (wcslen(g_fileOptions.wszDoNotAddFilesWithExtensions) > 0 
				&& ExceptExtension(finder.GetFileName()))
				continue;

			//checking file size//
			if (g_fileOptions.chkDoNotAddZeroSizeFile && !CheckingFileSize(strSrcDir + L"\\" + finder.GetFileName()))
				continue;

			//add//
			CString strName = finder.GetFileName();
			ADDINSTALLATIONFILES val;
			val.nChkDir = 0;
			wsprintf(val.wszSrcFullName, L"%s\\%s",strSrcDir, strName);
			wsprintf(val.wszDestName, L"%s", strName);
			wsprintf(val.wszDestFullName, L"%s\\%s", strDestDir, strName);
			
			INT nIdx = CheckFileAlreadyAdded(strDestDir + L"\\" + strName);
			if (nIdx == -1)//new
			{
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);

				//checking//
				if (!g_fileOptions.chkDoNotShowFilesTreeView)
				{
					HTREEITEM hItem = m_LisctrlTreeInstallationFiles.InsertItem(strName, 0, 0, hParent, TVI_LAST);

					//auto set main executable//
					if (strName.Find(L".exe") > 0 && 0)
					{
						CString strmsg;
						strmsg = L"Would you like to set the following file as Main Executable?\n";
						strmsg.AppendFormat(L"%s\\%s", strSrcDir, strName);
						if (::MessageBox(this->m_hWnd, strmsg, L"Installer", MB_YESNO) == IDYES)
						{
							HTREEITEM hOld = m_hSelectedItem;
							m_hSelectedItem = hItem;
							OnIsfilesSetasmainexecutable();

							m_hSelectedItem = hOld;
						}
					}			
				}
					
				nRet++;
			}
			else if (nIdx == -2)//reject
			{
			}
			else//already exist (replace)//
			{
				g_globaldata.install_sh2.filesandfolders.vecInstallationData[nIdx] = val;
				if (!g_fileOptions.chkDoNotShowFilesTreeView)
					SetTreeCtrlItemsFromGlobalVariable();
			}			
		}
	}	

	return nRet;
}

HTREEITEM CInstallationFilesAndFoldersDlg::GetParentItemFromDestName(CString strDestFullName, BOOL  bFromGlbal)
{
	HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetRootItem();

	if(strDestFullName == L"")
		return hitem;

	if(strDestFullName.ReverseFind('\\') != 0)
		strDestFullName += L"\\";


	CString strDestName = strDestFullName;
	INT res = -1;
	while(TRUE)
	{
		INT index = strDestFullName.Find(L"\\");
		if(index < 0)
			break;

		strDestName = strDestFullName.Left(index);
		strDestFullName = strDestFullName.Right(strDestFullName.GetLength() - index - 1);

		if(res == 1)
		{
			hitem = m_LisctrlTreeInstallationFiles.InsertItem(strDestName, 1 ,1, hitem, NULL);

			if(bFromGlbal == FALSE)
			{
				CString strTemp;
				GetFullNameFromHTreeItem(hitem, strTemp);

				ADDINSTALLATIONFILES v;
				v.nChkDir = 1;					
				wsprintf(v.wszDestFullName, L"%s", strTemp);
				wsprintf(v.wszDestName, L"%s", strDestName);
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(v);
			}
		}

		while(TRUE)
		{		
			if(wcscmp(strDestName, m_LisctrlTreeInstallationFiles.GetItemText(hitem)) == 0)
			{ 
				if(m_LisctrlTreeInstallationFiles.GetChildItem(hitem) == NULL)
				{
					res = 1;//Exist Dir
				}
				else
				{
					if(strDestFullName != L"")
						hitem = m_LisctrlTreeInstallationFiles.GetChildItem(hitem);
				}
				break;
			}

			if(m_LisctrlTreeInstallationFiles.GetNextItem(hitem, TVGN_NEXT) == NULL)
			{
				res = 0;//No Nextitem...
				hitem = m_LisctrlTreeInstallationFiles.GetParentItem(hitem);

				hitem = m_LisctrlTreeInstallationFiles.InsertItem(strDestName, 1 ,1, hitem, NULL);

				if(bFromGlbal == FALSE)
				{
					CString strTemp;
					GetFullNameFromHTreeItem(hitem, strTemp);

					ADDINSTALLATIONFILES v;
					v.nChkDir = 1;					
					wsprintf(v.wszDestFullName, L"%s", strTemp);
					wsprintf(v.wszDestName, L"%s", strDestName);
					g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(v);
				}
				res = 1;
				break;
			}
			else
			{
				hitem = m_LisctrlTreeInstallationFiles.GetNextItem(hitem, TVGN_NEXT);
			}

		}
	}

	return hitem;
}

void CInstallationFilesAndFoldersDlg::InitTree()
{
	m_imgList.Create( 16, 16, ILC_COLOR8, 0, 0);

	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FILE));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER));

	/*m_LisctrlTreeInstallationFiles.SetExtendedStyle(m_LisctrlTreeInstallationFiles.GetExtendedStyle() | TVS_EX_MULTISELECT,
		m_LisctrlTreeInstallationFiles.GetExtendedStyle() | TVS_EX_MULTISELECT);*/

	m_LisctrlTreeInstallationFiles.SetImageList(&m_imgList, TVSIL_NORMAL);

	CString strInsDir = L"#INSTALL_DIR#";
	HTREEITEM hitm = m_LisctrlTreeInstallationFiles.InsertItem(strInsDir, 1, 1, NULL, NULL);

	ADDINSTALLATIONFILES v;
	v.nChkDir = 1;	
	wsprintf(v.wszDestFullName, strInsDir.GetBuffer());
	wsprintf(v.wszDestName, strInsDir.GetBuffer());

	g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(v);	
}

INT CInstallationFilesAndFoldersDlg::GetIndexFromDestFullName(CString strDestFullName)
{
	int32_t i = 0;
	for (i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i ++)
	{
		if(wcscmp(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, strDestFullName.GetBuffer()) == 0)
		{
			break;
		}
	}
	if(i == g_globaldata.install_sh2.filesandfolders.vecInstallationData.size())
		i = -1;

	return i;
}

void CInstallationFilesAndFoldersDlg::GetFullNameFromHTreeItem(HTREEITEM hitem, CString &strFullName)
{
	HTREEITEM hParent = hitem;
	strFullName = m_LisctrlTreeInstallationFiles.GetItemText(hitem);
	while(TRUE)
	{
		hParent = m_LisctrlTreeInstallationFiles.GetParentItem(hParent);

		if(hParent == NULL)
			break;

		strFullName = m_LisctrlTreeInstallationFiles.GetItemText(hParent) + L"\\" + strFullName;
	}
	
	return ;
}



void CInstallationFilesAndFoldersDlg::OnIsfilesEdit()
{
	// TODO: Add your command handler code here
	OnBnClickedBtnEdit();
}


void CInstallationFilesAndFoldersDlg::OnIsfilesSetasmainexecutable()
{
	// TODO: Add your command handler code here
	/*HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();*/
	HTREEITEM hitem = m_hSelectedItem;
	if(hitem)
	{
		CString strItem;
		GetFullNameFromHTreeItem(hitem, strItem);
		SetDlgItemText(IDC_COMBO_MAIN_EXECUTABLE, strItem);

		//INT index = GetIndexFromDestFullName(strItem);
		//wcscpy(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszDestName);
		//SetDlgItemText(IDC_EDIT_MAIN_EXECUTABLE, g_globaldata.install_sh2.filesandfolders.wszMainExecutable);
		/*CString strtmp1 = strItem.Left(strItem.Find(L'\\'));
		CString strtmp2 = strItem.Right(strItem.GetLength() - strItem.Find(L'\\'));
		strItem = L"%" + strtmp1 + L"%" + strtmp2;*/

		wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, strItem.GetBuffer());

		//case main executable//
		//remove old main exe shortcuts//
		for (INT k = 0; k < g_globaldata.install_sh2.shortcuts.vecDetail.size(); k++)
		{
			ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[k];
			if (value.nMainExe == 1)
			{
				g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + k);
				k--;
			}
		}

		CWnd* pParentWnd = GetParent();
		if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_SHORTCUT_REFRESH);

		///////////////////////////////////////

		//post message//
		wsprintf(g_globaldata.interface_sh3.finish.wszLaunchAppFile, strItem.GetBuffer());
		
		if (g_pMainDlgSheet)	g_pMainDlgSheet->PostMessage(UM_INTERFACE_FINISH_REFRESH);

		
		if (pParentWnd)			pParentWnd->PostMessage(UM_INSTALLATION_MAIN_SHORTCUT);

		//uninst setting//
		CString strMainExecutable = g_globaldata.install_sh2.filesandfolders.wszMainExecutable;
		strMainExecutable = strMainExecutable.Right(strMainExecutable.GetLength() - strMainExecutable.ReverseFind(L'\\') - 1);
		wsprintf(g_globaldata.install_sh2.uninst.wszOptionsCloseRunningAppFile, strMainExecutable.GetBuffer());
		if (pParentWnd)			pParentWnd->PostMessage(UM_INSTALLATION_UNINSTALLATION_REFRESH);
	}
}


void CInstallationFilesAndFoldersDlg::OnIsfilesCreateshortcut()
{
	// TODO: Add your command handler code here
	HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
	if(hitem)
	{
		//get targetfile name//
		CString strItem;
		GetFullNameFromHTreeItem(hitem, strItem);
		wchar_t* wszTargetFile = new wchar_t[_MAX_INSTALLER_STRING_LEN_];
		wsprintf(wszTargetFile, strItem.GetBuffer());

		//post message//
		CWnd* pParentWnd = GetParent();
		if (pParentWnd)	
			pParentWnd->PostMessage(UM_INSTALLATION_CREATE_SHORTCUT, (WPARAM)wszTargetFile);
	}
}


void CInstallationFilesAndFoldersDlg::OnIsfilesNewfolder()
{
	CInstallationAddNewfolderDlg dlgAddNewfolder;

	if(dlgAddNewfolder.DoModal() == IDOK)
	{
		HTREEITEM hParentItem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
		CString strItem;
		GetFullNameFromHTreeItem(hParentItem, strItem);
		INT index = GetIndexFromDestFullName(strItem);
		ADDINSTALLATIONFILES val;

		val.nChkDir = 1;
		wsprintf(val.wszSrcFullName, L"");
		wsprintf(val.wszDestName, L"%s", dlgAddNewfolder.m_strAddNewFolderName);
		wsprintf(val.wszDestFullName, L"%s\\%s", strItem, dlgAddNewfolder.m_strAddNewFolderName);
		g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);

		/*HTREEITEM hNextItem = NULL;
		//find position//
		m_ctrlTreeInstallationFiles.GetChildItem( hParentItem );
		while (hNextItem)
		{
			if (m_ctrlTreeInstallationFiles.GetChildItem(hNextItem) == NULL)
			{
				hNextItem = m_ctrlTreeInstallationFiles.GetNextItem(hNextItem, TVGN_PREVIOUS);
				break;
			}
			// Get next sibling to parent
			hNextItem = m_ctrlTreeInstallationFiles.GetNextItem(hNextItem, TVGN_NEXT);
		}*/

		//insert//
		//checking show//
		if (!g_fileOptions.chkDoNotShowFilesTreeView)
		{
			m_LisctrlTreeInstallationFiles.InsertItem(dlgAddNewfolder.m_strAddNewFolderName, 1, 1, hParentItem, TVI_FIRST);
			m_LisctrlTreeInstallationFiles.Expand(hParentItem, TVE_EXPAND);
		}
	}
}


void CInstallationFilesAndFoldersDlg::OnIsfilesOpenfilelocation()
{
	// TODO: Add your command handler code here
	HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
	if(hitem)
	{
		CString strItem;
		GetFullNameFromHTreeItem(hitem, strItem);
		INT index = GetIndexFromDestFullName(strItem);
		CString strPathName = g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].wszSrcFullName;
		INT k = strPathName.ReverseFind(L'\\');
		strPathName = strPathName.Left(k);
		ShellExecute(NULL, L"open", strPathName, NULL, NULL, SW_NORMAL);
	}
}


void CInstallationFilesAndFoldersDlg::OnIsfilesRemove()
{
	// TODO: Add your command handler code here
	RemoveItem();
}


void CInstallationFilesAndFoldersDlg::OnDblclkTreeInstallationFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_ADD_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ADD_DELETE)->EnableWindow(TRUE);

	HTREEITEM hitem = m_LisctrlTreeInstallationFiles.GetSelectedItem();
	if (hitem)
	{
		CString strFull;
		INT index;
		GetFullNameFromHTreeItem(hitem, strFull);
		index = GetIndexFromDestFullName(strFull);
		if (index >=0 && g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == 0)
		{
			OnBnClickedBtnEdit();
		}
	}
	
	*pResult = 0;
}

void CInstallationFilesAndFoldersDlg::OnRclickTreeInstallationFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	UNREFERENCED_PARAMETER(pNMItemActivate);

	CPoint CurrentPosition;
	::GetCursorPos(&CurrentPosition);

	m_LisctrlTreeInstallationFiles.ScreenToClient(&CurrentPosition);
	HTREEITEM hCurItem = m_LisctrlTreeInstallationFiles.HitTest(CurrentPosition);
	if (!hCurItem)		return;
	m_hSelectedItem = hCurItem;
	////////////////////////////////////////////////////////////////////////

	GetDlgItem(IDC_BTN_ADD_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ADD_DELETE)->EnableWindow(TRUE);
	
	//HTREEITEM hCurItem = m_ctrlTreeInstallationFiles.GetSelectedItem();
	if (m_LisctrlTreeInstallationFiles.GetItemData(hCurItem) == UNISTALL_FLG ||
		m_LisctrlTreeInstallationFiles.GetItemData(hCurItem) == UPDATER_FLG ||
		m_LisctrlTreeInstallationFiles.GetItemData(hCurItem) == UPDATER_INI_FLG)
	{
		vector<UINT>	disableMenu;
		disableMenu.push_back(ID_ISFILES_NEWFOLDER);
		disableMenu.push_back(ID_ISFILES_EDIT);
		disableMenu.push_back(ID_ISFILES_SETASMAINEXECUTABLE);
		disableMenu.push_back(ID_ISFILES_OPENFILELOCATION);
		disableMenu.push_back(ID_ISFILES_CREATESHORTCUT);
		PopupMenu(IDR_MENU_INSFILES, 0, disableMenu, this);
	}
	else if (hCurItem)
	{
		CString strTem;
		GetFullNameFromHTreeItem(hCurItem, strTem);
		int index = GetIndexFromDestFullName(strTem);
		if (index < 0)		return;

		if (g_globaldata.install_sh2.filesandfolders.vecInstallationData[index].nChkDir == TRUE)
		{
			vector<UINT>	disableMenu;
			disableMenu.push_back(ID_ISFILES_OPENFILELOCATION);
			disableMenu.push_back(ID_ISFILES_SETASMAINEXECUTABLE);
			PopupMenu(IDR_MENU_INSFILES, 0, disableMenu, this);
		}
		else
		{
			vector<UINT>	disableMenu;
			disableMenu.push_back(ID_ISFILES_NEWFOLDER);
			PopupMenu(IDR_MENU_INSFILES, 0, disableMenu, this);
		}
	}
	*pResult = 0;
}

LRESULT CInstallationFilesAndFoldersDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_USER + 0x50)
	{
		GetDlgItem(IDC_BTN_ADD_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADD_DELETE)->EnableWindow(TRUE);
	}

	return CPropertyPage::WindowProc(message, wParam, lParam);
}


void CInstallationFilesAndFoldersDlg::AddUninstaller(BOOL bAdd)
{
	HTREEITEM hParentItem = m_LisctrlTreeInstallationFiles.GetChildItem(NULL);
	if (bAdd)
	{
		HTREEITEM hItem = m_LisctrlTreeInstallationFiles.InsertItem(g_globaldata.install_sh2.advanced.wszUninstallerFileName, 0, 0, hParentItem, NULL);
		m_LisctrlTreeInstallationFiles.SetItemData(hItem, UNISTALL_FLG);
	}
	else if(m_LisctrlTreeInstallationFiles.ItemHasChildren(hParentItem))
	{
		HTREEITEM hUnistItem = m_LisctrlTreeInstallationFiles.GetChildItem(hParentItem);
		do
		{
			if (hUnistItem)
			{
				DWORD dwData = m_LisctrlTreeInstallationFiles.GetItemData(hUnistItem);
				if (dwData == UNISTALL_FLG)//uninstaller
				{
					m_LisctrlTreeInstallationFiles.DeleteItem(hUnistItem);
					break;
				}
			}

			hUnistItem = m_LisctrlTreeInstallationFiles.GetNextItem(hUnistItem, TVGN_NEXT);
		} while (hUnistItem);
	}
}

void CInstallationFilesAndFoldersDlg::AddUpdater(BOOL bAdd)
{
	HTREEITEM hParentItem = m_LisctrlTreeInstallationFiles.GetChildItem(NULL);
	if (bAdd)
	{
		HTREEITEM hItem = m_LisctrlTreeInstallationFiles.InsertItem(L"Updater.exe", 0, 0, hParentItem, NULL);
		m_LisctrlTreeInstallationFiles.SetItemData(hItem, UPDATER_FLG);
		
		hItem = m_LisctrlTreeInstallationFiles.InsertItem(L"updater_config.ini", 0, 0, hParentItem, NULL);
		m_LisctrlTreeInstallationFiles.SetItemData(hItem, UPDATER_INI_FLG);
	}
	else if(m_LisctrlTreeInstallationFiles.ItemHasChildren(hParentItem))
	{
		HTREEITEM hUnistItem = m_LisctrlTreeInstallationFiles.GetChildItem(hParentItem);
		do
		{
			if (hUnistItem)
			{
				DWORD dwData = m_LisctrlTreeInstallationFiles.GetItemData(hUnistItem);
				if (dwData == UPDATER_FLG)//updater
				{
					m_LisctrlTreeInstallationFiles.DeleteItem(hUnistItem);
					break;
				}
			}

			hUnistItem = m_LisctrlTreeInstallationFiles.GetNextItem(hUnistItem, TVGN_NEXT);
		} while (hUnistItem);

		hUnistItem = m_LisctrlTreeInstallationFiles.GetChildItem(hParentItem);
		do
		{
			if (hUnistItem)
			{
				DWORD dwData = m_LisctrlTreeInstallationFiles.GetItemData(hUnistItem);
				if (dwData == UPDATER_INI_FLG)//updater
				{
					m_LisctrlTreeInstallationFiles.DeleteItem(hUnistItem);
					break;
				}
			}

			hUnistItem = m_LisctrlTreeInstallationFiles.GetNextItem(hUnistItem, TVGN_NEXT);
		} while (hUnistItem);
	}
}

void CInstallationFilesAndFoldersDlg::SettingInstallFilesFromWizard()
{
	CString strPathName;
	strPathName = g_globaldata.install_sh2.filesandfolders.wszSourceDirectory;
	if (!PathFileExists(strPathName))	return;
	wsprintf(g_globaldata.install_sh2.filesandfolders.wszSourceDirectory, strPathName.GetBuffer());
	SetDlgItemText(IDC_EDIT_SOURCE_DIRECTORY, strPathName);

	//insert into tree//
	m_LisctrlTreeInstallationFiles.DeleteAllItems();
	g_globaldata.install_sh2.filesandfolders.vecInstallationData.clear();
	InsertTreeCtrlFromDirectroy(strPathName, L"#INSTALL_DIR#");

	AddUninstaller(g_globaldata.install_sh2.uninst.chkIncludeUninstaller);
	AddUpdater(g_globaldata.install_sh2.updater.nChkIncludeUpdaterTool);

	//set main executable//
	if (wcslen(g_globaldata.install_sh2.filesandfolders.wszMainExecutable) > 0)
	{
		//remove old main exe shortcuts//
		for (INT k = 0; k < g_globaldata.install_sh2.shortcuts.vecDetail.size(); k++)
		{
			ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[k];
			if (value.nMainExe == 1)
			{
				g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + k);
				k--;
			}
		}

		CWnd* pParentWnd = GetParent();
		if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_SHORTCUT_REFRESH);

		///////////////////////////////////////

		wsprintf(g_globaldata.interface_sh3.finish.wszLaunchAppFile, strPathName.GetBuffer());
		
		if (g_pMainDlgSheet)	g_pMainDlgSheet->PostMessage(UM_INTERFACE_FINISH_REFRESH);
		if (pParentWnd)			pParentWnd->PostMessage(UM_INSTALLATION_MAIN_SHORTCUT);
	}	
}

void CInstallationFilesAndFoldersDlg::AddFontFile(CString strFontFileFullPath)
{
	CString strName;
	strName = strFontFileFullPath.Right(strFontFileFullPath.GetLength() - strFontFileFullPath.ReverseFind(L'\\') - 1);
	ADDINSTALLATIONFILES val;
	val.nChkDir = 0;
	wsprintf(val.wszSrcFullName, strFontFileFullPath.GetBuffer());
	wsprintf(val.wszDestName, L"%s", strName);
	wsprintf(val.wszDestFullName, L"%s\\%s", L"#FONTS_DIR#", strName);

	g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);
}

void CInstallationFilesAndFoldersDlg::OnCbnSelchangeComboMainExecutable()
{
	// TODO: Add your control notification handler code here
	//set main executable//
	CString strMainExe = L"";
	m_ctrlComboMainExecutable.GetLBText(m_ctrlComboMainExecutable.GetCurSel(), strMainExe);

	if (!strMainExe.IsEmpty())
	{
		//remove old main exe shortcuts//
		for (INT k = 0; k < g_globaldata.install_sh2.shortcuts.vecDetail.size(); k++)
		{
			ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[k];
			if (value.nMainExe == 1)
			{
				g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + k);
				k--;
			}
		}

		CWnd* pParentWnd = GetParent();
		if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_SHORTCUT_REFRESH);

		///////////////////////////////////////
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, strMainExe.GetBuffer());
		wsprintf(g_globaldata.interface_sh3.finish.wszLaunchAppFile, strMainExe.GetBuffer());

		if (g_pMainDlgSheet)	g_pMainDlgSheet->PostMessage(UM_INTERFACE_FINISH_REFRESH);
		if (pParentWnd)			pParentWnd->PostMessage(UM_INSTALLATION_MAIN_SHORTCUT);
	}
}


void CInstallationFilesAndFoldersDlg::SetMainExcutablesIntoCombobox()
{
	m_ctrlComboMainExecutable.ResetContent();	

	INT nCnt = 0;
	for (INT i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
	{
		ADDINSTALLATIONFILES val = g_globaldata.install_sh2.filesandfolders.vecInstallationData[i];
		CString strDestFullName(val.wszDestFullName);
		if (strDestFullName.Find(L".exe") > 0)
		{
			m_ctrlComboMainExecutable.AddString(strDestFullName);
			nCnt++;
		}
	}

	//set main executable//
	CString strMainExe = L"";
	if (wcslen(g_globaldata.install_sh2.filesandfolders.wszMainExecutable) > 0)
		strMainExe.Format(L"%s", g_globaldata.install_sh2.filesandfolders.wszMainExecutable);
	else if (nCnt)
		m_ctrlComboMainExecutable.GetLBText(0, strMainExe);

	SetDlgItemText(IDC_COMBO_MAIN_EXECUTABLE, strMainExe);

	if (!strMainExe.IsEmpty())
	{
		//remove old main exe shortcuts//
		for (INT k = 0; k < g_globaldata.install_sh2.shortcuts.vecDetail.size(); k++)
		{
			ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[k];
			if (value.nMainExe == 1)
			{
				g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + k);
				k--;
			}
		}

		CWnd* pParentWnd = GetParent();
		if (pParentWnd)		pParentWnd->PostMessage(UM_INSTALLATION_SHORTCUT_REFRESH);

		/////////////////
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszMainExecutable, strMainExe.GetBuffer());
		wsprintf(g_globaldata.interface_sh3.finish.wszLaunchAppFile, strMainExe.GetBuffer());
		
		//set finish tab//
		if (g_pMainDlgSheet)	g_pMainDlgSheet->PostMessage(UM_INTERFACE_FINISH_REFRESH);

		//set shortcuts//
		if (pParentWnd)			pParentWnd->PostMessage(UM_INSTALLATION_MAIN_SHORTCUT);
	}
}

void CInstallationFilesAndFoldersDlg::OnBnClickedCheckInstallation64bitApp()
{
	// TODO: Add your control notification handler code here
	CString strInstallationDir = g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory;
	g_globaldata.install_sh2.filesandfolders.nChk64BitApplication = IsDlgButtonChecked(IDC_CHECK_INSTALLATION_64BIT_APP);
	if (g_globaldata.install_sh2.filesandfolders.nChk64BitApplication)
	{
		strInstallationDir.Replace(L"#PROGRAM_FILES_X86#", L"#PROGRAM_FILES#");
	}
	else
	{
		strInstallationDir.Replace(L"#PROGRAM_FILES#", L"#PROGRAM_FILES_X86#");
	}
	
	wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, strInstallationDir);
	SetDlgItemText(IDC_EDIT_INSTALLATION_DIRECTORY, strInstallationDir);
}