// InterfaceSelectFileFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceSelectFileFolderDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

// CInterfaceSelectFileFolderDlg dialog

IMPLEMENT_DYNAMIC(CInterfaceSelectFileFolderDlg, CDialogEx)

CInterfaceSelectFileFolderDlg::CInterfaceSelectFileFolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInterfaceSelectFileFolderDlg::IDD, pParent)
{

}

CInterfaceSelectFileFolderDlg::~CInterfaceSelectFileFolderDlg()
{
}

void CInterfaceSelectFileFolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SELECT_FILE_FOLDER, m_ctrlTreeFileFolder);
}


BEGIN_MESSAGE_MAP(CInterfaceSelectFileFolderDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInterfaceSelectFileFolderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInterfaceSelectFileFolderDlg::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SELECT_FILE_FOLDER, &CInterfaceSelectFileFolderDlg::OnTvnSelchangedTreeSelectFileFolder)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CInterfaceSelectFileFolderDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()


// CInterfaceSelectFileFolderDlg message handlers


void CInterfaceSelectFileFolderDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_SELECT_FILE_FOLDER_PATH, m_strSectedFullName);
	CDialogEx::OnOK();
}


void CInterfaceSelectFileFolderDlg::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}


BOOL CInterfaceSelectFileFolderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitTree();
	SetTreeCtrlItemsFromGlobalVariable();
	SetDlgItemText(IDC_EDIT_SELECT_FILE_FOLDER_PATH, m_strSectedFullName);
	return TRUE;  
}

void CInterfaceSelectFileFolderDlg::InitTree()
{
	m_imgList.Create( 16, 16, ILC_COLOR8, 0, 0);

	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FILE));
	m_imgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER));

	m_ctrlTreeFileFolder.SetImageList(&m_imgList, TVSIL_NORMAL);
}

void CInterfaceSelectFileFolderDlg::GetFullNameFromHTreeItem(HTREEITEM hitem, CString &strFullName)
{
	HTREEITEM hParent = hitem;
	strFullName = m_ctrlTreeFileFolder.GetItemText(hitem);
	while(TRUE)
	{
		hParent = m_ctrlTreeFileFolder.GetParentItem(hParent);

		if(hParent == NULL)
			break;

		strFullName = m_ctrlTreeFileFolder.GetItemText(hParent) + L"\\" + strFullName;
	}

	return ;
}

HTREEITEM CInterfaceSelectFileFolderDlg::GetHTreeItemFromDestName(CString strDestFullName, BOOL  bFromGlbal)
{
	HTREEITEM hitem = m_ctrlTreeFileFolder.GetRootItem();

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
			hitem = m_ctrlTreeFileFolder.InsertItem(strDestName, 1 ,1, hitem);

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
			if(wcscmp(strDestName, m_ctrlTreeFileFolder.GetItemText(hitem)) == 0)
			{ 
				if(m_ctrlTreeFileFolder.GetChildItem(hitem) == NULL)
				{
					res = 1;//Exist Dir
				}
				else
				{
					if(strDestFullName != L"")
						hitem = m_ctrlTreeFileFolder.GetChildItem(hitem);
				}
				break;
			}

			if(m_ctrlTreeFileFolder.GetNextItem(hitem, TVGN_NEXT) == NULL)
			{
				res = 0;//No Nextitem...
				hitem = m_ctrlTreeFileFolder.GetParentItem(hitem);

				hitem = m_ctrlTreeFileFolder.InsertItem(strDestName, 1 ,1, hitem);

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
				hitem = m_ctrlTreeFileFolder.GetNextItem(hitem, TVGN_NEXT);
		}
	}

	return hitem;

}

void CInterfaceSelectFileFolderDlg::SetTreeCtrlItemsFromGlobalVariable()
{

	for (INT i = 0 ; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size() ; i ++)
	{
		wchar_t szDir[_MAX_INSTALLER_STRING_LEN_];
		if(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].nChkDir == 0)
		{
			wcscpy(szDir, g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName);
			wchar_t* wsz;
			wsz = wcsrchr(szDir, _T('\\'));
			int32_t k = wcslen(szDir) - wcslen(wsz);
			szDir[k] = 0;
			//Memory Leak...

			HTREEITEM hitem = GetHTreeItemFromDestName(szDir, TRUE);
			hitem = m_ctrlTreeFileFolder.InsertItem(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestName, 0, 0, hitem);
		}
		else
			HTREEITEM hitem = GetHTreeItemFromDestName(g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszDestFullName, TRUE);
	}

	HTREEITEM hRoots = m_ctrlTreeFileFolder.GetRootItem();

	while(hRoots)
	{
		m_ctrlTreeFileFolder.Expand(hRoots, TVE_EXPAND);
		hRoots = m_ctrlTreeFileFolder.GetNextItem(hRoots, TVGN_NEXT);
	}

}

void CInterfaceSelectFileFolderDlg::OnTvnSelchangedTreeSelectFileFolder(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	
	HTREEITEM hitem = m_ctrlTreeFileFolder.GetSelectedItem();
	GetFullNameFromHTreeItem(hitem, m_strSectedFullName);
	SetDlgItemText(IDC_EDIT_SELECT_FILE_FOLDER_PATH, m_strSectedFullName);
	*pResult = 0;
}

void CInterfaceSelectFileFolderDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString strMsg = L"If you want to select a file from your computer, you need first add it to the project.\nTo make it, go to the Installation\\Files and Folders page and click the 'Add Files' button.\nFor the file destination select <InstallDir>";
	MessageBox(strMsg, INSTALLER_NAME, MB_OK);
	*pResult = 0;
}


void CInterfaceSelectFileFolderDlg::SetInitPathStr(CString strPath)
{
	m_strSectedFullName = strPath;
}