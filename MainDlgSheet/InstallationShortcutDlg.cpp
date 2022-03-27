// InstallationShortcutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationShortcutDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"
#include "InstallationShortcutDetailDlg.h"

// CInstallationShortcutDlg dialog

IMPLEMENT_DYNAMIC(CInstallationShortcutDlg, CPropertyPage)

CInstallationShortcutDlg::CInstallationShortcutDlg()
	: CPropertyPage(CInstallationShortcutDlg::IDD)
	,m_strProgramGroupName(_T(""))
{
	m_bRefresh = TRUE;
	wsprintf(m_wszComboContent[0], L"#APPLICATION_NAME#");
	wsprintf(m_wszComboContent[1], L"#APPLICATION_NAME# #APPLICATION_VERSION#");
	wsprintf(m_wszComboContent[2], L"#COMPANY_NAME#\\#APPLICATION_NAME#");
	wsprintf(m_wszComboContent[3], L"#COMPANY_NAME#\\#APPLICATION_NAME# #APPLICATION_VERSION#");
}

CInstallationShortcutDlg::~CInstallationShortcutDlg()
{
}

void CInstallationShortcutDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_PROGRAM_GROUP_NAME, m_strProgramGroupName);
	DDX_Control(pDX, IDC_LIST_INSTALLATION_SHORTCUT, m_listCtrlShortcuts);
	DDX_Control(pDX, IDC_COMBO_PROGRAM_GROUP_NAME, m_ctrlComboProGroupName);
}


BEGIN_MESSAGE_MAP(CInstallationShortcutDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_ADD, &CInstallationShortcutDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_EDIT, &CInstallationShortcutDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_REMOVE, &CInstallationShortcutDlg::OnBnClickedBtnRemove)
	ON_CBN_SELCHANGE(IDC_COMBO_PROGRAM_GROUP_NAME, &CInstallationShortcutDlg::OnCbnSelchangeComboProgramGroupName)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INSTALLATION_SHORTCUT, &CInstallationShortcutDlg::OnDblclkListInstallationShortcut)
	ON_CBN_EDITCHANGE(IDC_COMBO_PROGRAM_GROUP_NAME, &CInstallationShortcutDlg::OnEditchangeComboProgramGroupName)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INSTALLATION_SHORTCUT, &CInstallationShortcutDlg::OnClickListInstallationShortcut)
END_MESSAGE_MAP()


// CInstallationShortcutDlg message handlers


void CInstallationShortcutDlg::OnBnClickedBtnAdd()
{
	CInstallationShortcutDetailDlg dlg;
	dlg.m_strShortcutLocation = L"#PROGRAM_GROUP#";
	dlg.m_nIndex = 0;
	dlg.m_strShow = L"Normal";

	if(dlg.DoModal() == IDOK)
	{
		//insert//
		CString strIndex;
		strIndex.Format(L"%d", dlg.m_nIndex);
		INT nCount = m_listCtrlShortcuts.GetItemCount();
		m_listCtrlShortcuts.InsertItem(nCount,	  dlg.m_strShortcutLocation);
		m_listCtrlShortcuts.SetItemText(nCount,1, dlg.m_strShortcutName);
		m_listCtrlShortcuts.SetItemText(nCount,2, dlg.m_strTargetFile);
		m_listCtrlShortcuts.SetItemText(nCount,3, dlg.m_strDescription);
		m_listCtrlShortcuts.SetItemText(nCount,4, dlg.m_strParameters);
		m_listCtrlShortcuts.SetItemText(nCount,5, dlg.m_strWorkingDirectory);
		m_listCtrlShortcuts.SetItemText(nCount,6, dlg.m_strIconFile);
		m_listCtrlShortcuts.SetItemText(nCount,7, strIndex);
		m_listCtrlShortcuts.SetItemText(nCount,8, dlg.m_strShow);

		//global param//
		ADDSHORTCUTDETAIL value;
		wsprintf(value.wszLocation, dlg.m_strShortcutLocation);
		wsprintf(value.wszShortcutName, dlg.m_strShortcutName);
		wsprintf(value.wszTargetFile, dlg.m_strTargetFile);
		wsprintf(value.wszDescription, dlg.m_strDescription);
		wsprintf(value.wszParameters, dlg.m_strParameters);
		wsprintf(value.wszWorkingDir, dlg.m_strWorkingDirectory);
		wsprintf(value.wszIconFile, dlg.m_strIconFile);
		value.nIconIdx = dlg.m_nIndex;
		wsprintf(value.wszShow, dlg.m_strShow);

		g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
	}
}


void CInstallationShortcutDlg::OnBnClickedBtnEdit()
{
	// TODO: Add your control notification handler code here
	if (m_nSelectedListIdx < 0 || m_listCtrlShortcuts.GetSelectedCount() <= 0)		return;

	CInstallationShortcutDetailDlg dlg;
	dlg.m_strTargetFile = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszTargetFile;
	dlg.m_strShortcutLocation = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszLocation;
	dlg.m_strShortcutName = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszShortcutName;
	dlg.m_strDescription = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszDescription;
	dlg.m_strParameters = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszParameters;
	dlg.m_strWorkingDirectory = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszWorkingDir;
	dlg.m_strIconFile = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszIconFile;
	dlg.m_nIndex = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].nIconIdx;
	dlg.m_strShow = g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx].wszShow;

	if(dlg.DoModal() == IDOK)
	{
		//insert//
		CString strIndex;
		strIndex.Format(L"%d", dlg.m_nIndex);
		INT nCount = m_nSelectedListIdx;
		m_listCtrlShortcuts.SetItemText(nCount,0, dlg.m_strShortcutLocation);
		m_listCtrlShortcuts.SetItemText(nCount,1, dlg.m_strShortcutName);
		m_listCtrlShortcuts.SetItemText(nCount,2, dlg.m_strTargetFile);
		m_listCtrlShortcuts.SetItemText(nCount,3, dlg.m_strDescription);
		m_listCtrlShortcuts.SetItemText(nCount,4, dlg.m_strParameters);
		m_listCtrlShortcuts.SetItemText(nCount,5, dlg.m_strWorkingDirectory);
		m_listCtrlShortcuts.SetItemText(nCount,6, dlg.m_strIconFile);
		m_listCtrlShortcuts.SetItemText(nCount,7, strIndex);
		m_listCtrlShortcuts.SetItemText(nCount,8, dlg.m_strShow);

		//global param//
		ADDSHORTCUTDETAIL value;
		wsprintf(value.wszLocation, dlg.m_strShortcutLocation);
		wsprintf(value.wszShortcutName, dlg.m_strShortcutName);
		wsprintf(value.wszTargetFile, dlg.m_strTargetFile);
		wsprintf(value.wszDescription, dlg.m_strDescription);
		wsprintf(value.wszParameters, dlg.m_strParameters);
		wsprintf(value.wszWorkingDir, dlg.m_strWorkingDirectory);
		wsprintf(value.wszIconFile, dlg.m_strIconFile);
		value.nIconIdx = dlg.m_nIndex;
		wsprintf(value.wszShow, dlg.m_strShow);

		g_globaldata.install_sh2.shortcuts.vecDetail[m_nSelectedListIdx] = value;
	}
}


void CInstallationShortcutDlg::OnBnClickedBtnRemove()
{
	// TODO: Add your control notification handler code here
	if (m_nSelectedListIdx < 0 || m_listCtrlShortcuts.GetSelectedCount() <= 0)		return;

	CString str = L"Do you really want to remove?";
	if (MessageBox(str, INSTALLER_NAME, MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		POSITION pos = m_listCtrlShortcuts.GetFirstSelectedItemPosition();
		if (pos == NULL)	return;
		while (pos)
		{
			INT32 index = m_listCtrlShortcuts.GetNextSelectedItem(pos);
			m_listCtrlShortcuts.DeleteItem(index);
			pos = m_listCtrlShortcuts.GetFirstSelectedItemPosition();
			g_globaldata.install_sh2.shortcuts.vecDetail.erase(g_globaldata.install_sh2.shortcuts.vecDetail.begin() + index);
		}

		m_nSelectedListIdx = -1;
	}
}


void CInstallationShortcutDlg::OnCbnSelchangeComboProgramGroupName()
{
	m_strProgramGroupName = m_wszComboContent[m_ctrlComboProGroupName.GetCurSel()];
	wcscpy(g_globaldata.install_sh2.shortcuts.wszProgramGroupName, m_strProgramGroupName);
}

BOOL CInstallationShortcutDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bRefresh)
	{
		m_strProgramGroupName = g_globaldata.install_sh2.shortcuts.wszProgramGroupName;
		UpdateData(FALSE);

		m_listCtrlShortcuts.DeleteAllItems();
		//insert//
		for (INT i = 0; i < g_globaldata.install_sh2.shortcuts.vecDetail.size(); i++)
		{
			ADDSHORTCUTDETAIL value = g_globaldata.install_sh2.shortcuts.vecDetail[i];
			CString strIndex;
			strIndex.Format(L"%d", value.nIconIdx);
			m_listCtrlShortcuts.InsertItem( i, 	  value.wszLocation);
			m_listCtrlShortcuts.SetItemText(i, 1, value.wszShortcutName);
			m_listCtrlShortcuts.SetItemText(i, 2, value.wszTargetFile);
			m_listCtrlShortcuts.SetItemText(i, 3, value.wszDescription);
			m_listCtrlShortcuts.SetItemText(i, 4, value.wszParameters);
			m_listCtrlShortcuts.SetItemText(i, 5, value.wszWorkingDir);
			m_listCtrlShortcuts.SetItemText(i, 6, value.wszIconFile);
			m_listCtrlShortcuts.SetItemText(i, 7, strIndex);
			m_listCtrlShortcuts.SetItemText(i, 8, value.wszShow);
		}
		m_bRefresh = FALSE;
	}
	
	return CPropertyPage::OnSetActive();
}


BOOL CInstallationShortcutDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here

	//list ctrl//
	m_listCtrlShortcuts.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );
	m_listCtrlShortcuts.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);


	m_listCtrlShortcuts.InsertColumn(0, L"Location", LVCFMT_LEFT, 70);
	m_listCtrlShortcuts.InsertColumn(1, L"Shortcut Name", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(2, L"Target File", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(3, L"Description", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(4, L"Parameters", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(5, L"Working Directory", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(6, L"Icon File", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(7, L"Icon Index", LVCFMT_LEFT,70);
	m_listCtrlShortcuts.InsertColumn(8, L"Show", LVCFMT_LEFT,70);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInstallationShortcutDlg::OnDblclkListInstallationShortcut(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_nSelectedListIdx = pNMItemActivate->iItem;
	OnBnClickedBtnEdit();
}


void CInstallationShortcutDlg::OnEditchangeComboProgramGroupName()
{
	// TODO: Add your control notification handler code here
	m_ctrlComboProGroupName.GetWindowText(m_strProgramGroupName);
	wcscpy(g_globaldata.install_sh2.shortcuts.wszProgramGroupName, m_strProgramGroupName);
}


void CInstallationShortcutDlg::OnClickListInstallationShortcut(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_nSelectedListIdx = pNMItemActivate->iItem;
	*pResult = 0;
}

void CInstallationShortcutDlg::AddCreateShortcutFromFilesAndFolders(CString strTargetFileName)
{
	CInstallationShortcutDetailDlg dlg;
	dlg.m_strTargetFile = strTargetFileName;
	INT k = strTargetFileName.ReverseFind(L'\\');
	dlg.m_strShortcutName = strTargetFileName.Right(strTargetFileName.GetLength() - k - 1);
	dlg.m_strShortcutLocation = L"#PROGRAM_GROUP#";
	dlg.m_nIndex = 0;
	dlg.m_strShow = L"Normal";

	if(dlg.DoModal() == IDOK)
	{
		//insert//
		CString strIndex;
		strIndex.Format(L"%d", dlg.m_nIndex);
		INT nCount = m_listCtrlShortcuts.GetItemCount();
		m_listCtrlShortcuts.InsertItem(nCount,	  dlg.m_strShortcutLocation);
		m_listCtrlShortcuts.SetItemText(nCount,1, dlg.m_strShortcutName);
		m_listCtrlShortcuts.SetItemText(nCount,2, dlg.m_strTargetFile);
		m_listCtrlShortcuts.SetItemText(nCount,3, dlg.m_strDescription);
		m_listCtrlShortcuts.SetItemText(nCount,4, dlg.m_strParameters);
		m_listCtrlShortcuts.SetItemText(nCount,5, dlg.m_strWorkingDirectory);
		m_listCtrlShortcuts.SetItemText(nCount,6, dlg.m_strIconFile);
		m_listCtrlShortcuts.SetItemText(nCount,7, strIndex);
		m_listCtrlShortcuts.SetItemText(nCount,8, dlg.m_strShow);

		//global param//
		ADDSHORTCUTDETAIL value;
		wsprintf(value.wszLocation, dlg.m_strShortcutLocation);
		wsprintf(value.wszShortcutName, dlg.m_strShortcutName);
		wsprintf(value.wszTargetFile, dlg.m_strTargetFile);
		wsprintf(value.wszDescription, dlg.m_strDescription);
		wsprintf(value.wszParameters, dlg.m_strParameters);
		wsprintf(value.wszWorkingDir, dlg.m_strWorkingDirectory);
		wsprintf(value.wszIconFile, dlg.m_strIconFile);
		value.nIconIdx = dlg.m_nIndex;
		wsprintf(value.wszShow, dlg.m_strShow);

		g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
	}
}

void CInstallationShortcutDlg::SetMainShortcut()
{
	CString strTargetFile, strWorkingDir;

	//clear	
	//g_globaldata.install_sh2.shortcuts.vecDetail.clear();

	//get target file & working directory//
	strTargetFile.Format(L"%s", g_globaldata.install_sh2.filesandfolders.wszMainExecutable);
	/*CString strtmp1 = strTargetFile.Left(strTargetFile.Find(L'\\'));
	CString strtmp2 = strTargetFile.Right(strTargetFile.GetLength() - strTargetFile.Find(L'\\'));
	strTargetFile = L"%" + strtmp1 + L"%" + strtmp2;*/

	INT k = strTargetFile.ReverseFind(L'\\');
	strWorkingDir = strTargetFile.Left(k);

	//get main executable name//
	CString strMainExe;
	strMainExe = strTargetFile.Right(strTargetFile.GetLength() - k - 1);
	k = strMainExe.ReverseFind(L'.');
	if (k > 0)
		strMainExe = strMainExe.Left(k);

	//add short cut//
	//global param
	ADDSHORTCUTDETAIL value;
	wsprintf(value.wszLocation, L"#PROGRAM_GROUP#");
	//wsprintf(value.wszShortcutName, L"#APPLICATION_NAME#");
	wsprintf(value.wszShortcutName, strMainExe.GetBuffer());
	wsprintf(value.wszTargetFile, strTargetFile.GetBuffer());
	wsprintf(value.wszDescription, L"");
	wsprintf(value.wszParameters, L"");
	wsprintf(value.wszWorkingDir, strWorkingDir.GetBuffer());
	wsprintf(value.wszIconFile, L"");
	value.nIconIdx = 0;
	wsprintf(value.wszShow, L"Normal");
	value.nMainExe = 1;

	if (g_globaldata.install_sh2.shortcuts.nChkStartMenuShortcutForMainExe)
		g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
	g_globaldata.install_sh2.shortcuts.nChkStartMenuShortcutForMainExe = 1;

	wsprintf(value.wszLocation, L"#DESKTOP#");
	if (g_globaldata.install_sh2.shortcuts.nChkDesktopShortcutForMainExe)
		g_globaldata.install_sh2.shortcuts.vecDetail.push_back(value);
	g_globaldata.install_sh2.shortcuts.nChkDesktopShortcutForMainExe = 1;

	m_bRefresh = TRUE;
}