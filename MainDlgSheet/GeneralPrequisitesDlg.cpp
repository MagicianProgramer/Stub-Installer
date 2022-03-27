// GeneralPrequisitesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GeneralPrequisitesDlg.h"
#include "afxdialogex.h"
#include "ProjectOptionsDlg.h"

#include "../common/common.h"
#include "AddPrerequisite.h"

// CGeneralPrequisitesDlg dialog

IMPLEMENT_DYNAMIC(CGeneralPrequisitesDlg, CPropertyPage)

CGeneralPrequisitesDlg::CGeneralPrequisitesDlg()
	: CPropertyPage(CGeneralPrequisitesDlg::IDD)
{
	m_nSelAddPre = -1;
}

CGeneralPrequisitesDlg::~CGeneralPrequisitesDlg()
{
}

void CGeneralPrequisitesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADDITIONAL_PREREQUISITES, m_listCtrlAddPre);
	DDX_Control(pDX, IDC_COMBO_NET_FRAMEWORK, m_ctrlComboNetFramework);
	DDX_Control(pDX, IDC_COMBO_INTERNET_EXPLORER, m_ctrlComboInternetExplorer);
	DDX_Control(pDX, IDC_COMBO_ADOBE_ACROBAT_READER, m_ctrlComboAcrobat);
	DDX_Control(pDX, IDC_COMBO_JAVA, m_ctrlComboJava);
	DDX_Control(pDX, IDC_COMBO_SQL_SERVER_COMPACT, m_ctrlComboSQLCompact);
	DDX_Control(pDX, IDC_COMBO_SQL_SERVER_EXPRESS, m_ctrlComboSQLExpress);
	DDX_Control(pDX, IDC_COMBO_VISUAL_RUNTIME, m_ctrlComboVCRuntimeX86);

	DDX_Control(pDX, IDC_CHECK_NET_FRAMEWORK, m_ctrlCheckNetFramework);
	DDX_Control(pDX, IDC_CHECK_INTERNET_EXPLORER, m_ctrlCheckInternetExplorer);
	DDX_Control(pDX, IDC_CHECK_ADOBE_ACROBAT_READER, m_ctrlCheckAcrobat);
	DDX_Control(pDX, IDC_CHECK_JAVA, m_ctrlCheckJava);
	DDX_Control(pDX, IDC_CHECK_SQL_SERVER_COMPACT, m_ctrlCheckSQLCompact);
	DDX_Control(pDX, IDC_CHECK_SQL_SERVER_EXPRESS, m_ctrlCheckSQLExpress);
	DDX_Control(pDX, IDC_CHECK_VISUAL_RUNTIME, m_ctrlCheckVCRuntimeX86);
	DDX_Control(pDX, IDC_COMBO_VISUAL_RUNTIME_X64, m_ctrlComboVCRuntimeX64);
	DDX_Control(pDX, IDC_COMBO_VISUAL_RUNTIME_ARM64, m_ctrlComboVCRuntimeARM64);
	DDX_Control(pDX, IDC_CHECK_VISUAL_RUNTIME_X64, m_ctrlCheckVCRuntimeX64);
	DDX_Control(pDX, IDC_CHECK_VISUAL_RUNTIME_ARM64, m_ctrlCheckVCRuntimeARM64);
}


BEGIN_MESSAGE_MAP(CGeneralPrequisitesDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_NET_FRAMEWORK, &CGeneralPrequisitesDlg::OnBnClickedCheckNetFramework)
	ON_BN_CLICKED(IDC_CHECK_INTERNET_EXPLORER, &CGeneralPrequisitesDlg::OnBnClickedCheckInternetExplorer)
	ON_BN_CLICKED(IDC_CHECK_ADOBE_ACROBAT_READER, &CGeneralPrequisitesDlg::OnBnClickedCheckAdobeAcrobatReader)
	ON_BN_CLICKED(IDC_CHECK_JAVA, &CGeneralPrequisitesDlg::OnBnClickedCheckJava)
	ON_BN_CLICKED(IDC_CHECK_SQL_SERVER_COMPACT, &CGeneralPrequisitesDlg::OnBnClickedCheckSqlServerCompact)
	ON_BN_CLICKED(IDC_CHECK_SQL_SERVER_EXPRESS, &CGeneralPrequisitesDlg::OnBnClickedCheckSqlServerExpress)
	ON_BN_CLICKED(IDC_CHECK_VISUAL_RUNTIME, &CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntime)
	ON_CBN_SELCHANGE(IDC_COMBO_NET_FRAMEWORK, &CGeneralPrequisitesDlg::OnCbnSelchangeComboNetFramework)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERNET_EXPLORER, &CGeneralPrequisitesDlg::OnCbnSelchangeComboInternetExplorer)
	ON_CBN_SELCHANGE(IDC_COMBO_ADOBE_ACROBAT_READER, &CGeneralPrequisitesDlg::OnCbnSelchangeComboAdobeAcrobatReader)
	ON_CBN_SELCHANGE(IDC_COMBO_JAVA, &CGeneralPrequisitesDlg::OnCbnSelchangeComboJava)
	ON_CBN_SELCHANGE(IDC_COMBO_SQL_SERVER_COMPACT, &CGeneralPrequisitesDlg::OnCbnSelchangeComboSqlServerCompact)
	ON_CBN_SELCHANGE(IDC_COMBO_SQL_SERVER_EXPRESS, &CGeneralPrequisitesDlg::OnCbnSelchangeComboSqlServerExpress)
	ON_CBN_SELCHANGE(IDC_COMBO_VISUAL_RUNTIME, &CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeX86)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_VIEW_EDIT_LINKS, &CGeneralPrequisitesDlg::OnNMClickSyslinkViewEditLinks)
	ON_BN_CLICKED(IDC_BTN_DUPLICATE, &CGeneralPrequisitesDlg::OnBnClickedBtnDuplicate)
	ON_BN_CLICKED(IDC_BTN_AP_ADD, &CGeneralPrequisitesDlg::OnBnClickedBtnApAdd)
	ON_BN_CLICKED(IDC_BTN_AP_EDIT, &CGeneralPrequisitesDlg::OnBnClickedBtnApEdit)
	ON_BN_CLICKED(IDC_BTN_AP_REMOVE, &CGeneralPrequisitesDlg::OnBnClickedBtnApRemove)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CGeneralPrequisitesDlg::OnDeltaposSpin1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ADDITIONAL_PREREQUISITES, &CGeneralPrequisitesDlg::OnNMClickListAdditionalPrerequisites)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ADDITIONAL_PREREQUISITES, &CGeneralPrequisitesDlg::OnNMDblclkListAdditionalPrerequisites)
	ON_BN_CLICKED(IDC_CHECK_VISUAL_RUNTIME_X64, &CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntimeX64)
	ON_BN_CLICKED(IDC_CHECK_VISUAL_RUNTIME_ARM64, &CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntimeArm64)
	ON_CBN_SELCHANGE(IDC_COMBO_VISUAL_RUNTIME_X64, &CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeX64)
	ON_CBN_SELCHANGE(IDC_COMBO_VISUAL_RUNTIME_ARM64, &CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeArm64)
END_MESSAGE_MAP()


// CGeneralPrequisitesDlg message handlers


void CGeneralPrequisitesDlg::OnBnClickedCheckNetFramework()
{
	// TODO: Add your control notification handler code here
	m_nChkDotNet = m_ctrlCheckNetFramework.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkNetFramework = m_nChkDotNet;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckInternetExplorer()
{
	// TODO: Add your control notification handler code here
	m_nChkInterExplorer = m_ctrlCheckInternetExplorer.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkInternetExplorer = m_nChkInterExplorer;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckAdobeAcrobatReader()
{
	// TODO: Add your control notification handler code here
	m_nChkAcrobat = m_ctrlCheckAcrobat.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkAcrobat = m_nChkAcrobat;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckJava()
{
	// TODO: Add your control notification handler code here
	m_nChkJava = m_ctrlCheckJava.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkJava = m_nChkJava;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckSqlServerCompact()
{
	// TODO: Add your control notification handler code here
	m_nChkSQLCompact = m_ctrlCheckSQLCompact.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkSqlServerCompact = m_nChkSQLCompact;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckSqlServerExpress()
{
	// TODO: Add your control notification handler code here
	m_nChkSQLExpress = m_ctrlCheckSQLExpress.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkSqlServerExpress = m_nChkSQLExpress;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntime()
{
	// TODO: Add your control notification handler code here
	m_nChkVCRuntime86 = m_ctrlCheckVCRuntimeX86.GetCheck();
	g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86 = m_nChkVCRuntime86;
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboNetFramework()
{
	// TODO: Add your control notification handler code here
	m_nDotNetIdx = m_ctrlComboNetFramework.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nNetFrameworkMode = m_nDotNetIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_NET_FRAMEWORK, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszNetFramework, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboInternetExplorer()
{
	// TODO: Add your control notification handler code here
	m_nInterExpIdx = m_ctrlComboInternetExplorer.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nInternetExplorerMode = m_nInterExpIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_INTERNET_EXPLORER, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszInternetExplorer, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboAdobeAcrobatReader()
{
	// TODO: Add your control notification handler code here
	m_nAcrobatIdx = m_ctrlComboAcrobat.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nAcrobatMode = m_nAcrobatIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_ADOBE_ACROBAT_READER, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszAcrobatReader, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboJava()
{
	// TODO: Add your control notification handler code here
	m_nJavaIdx = m_ctrlComboJava.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nJavaMode = m_nJavaIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_JAVA, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszJava, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboSqlServerCompact()
{
	// TODO: Add your control notification handler code here
	m_nSqlCompactIdx = m_ctrlComboSQLCompact.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nSqlServerCompactMode = m_nSqlCompactIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_SQL_SERVER_COMPACT, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszSQLServerCompact, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboSqlServerExpress()
{
	// TODO: Add your control notification handler code here
	m_nSqlExpressIdx = m_ctrlComboSQLExpress.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nSqlServerExpressMode = m_nSqlExpressIdx;

	CString str;
	GetDlgItemText(IDC_COMBO_SQL_SERVER_EXPRESS, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszSQLServerExpress, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeX86()
{
	// TODO: Add your control notification handler code here
	m_nVCRuntimeX86Idx = m_ctrlComboVCRuntimeX86.GetCurSel();
	g_globaldata.general_sh1.prerequisites.nVCRuntimeModeX86 = m_nVCRuntimeX86Idx;

	CString str;
	GetDlgItemText(IDC_COMBO_VISUAL_RUNTIME, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszVCRuntimeX86, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnNMClickSyslinkViewEditLinks(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CProjectOptionsDlg dlg(2);
	if(dlg.DoModal() == IDOK)
	{
		SaveProjectOptionsIntoRegistry();
	}	
}


BOOL CGeneralPrequisitesDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	SetViewComponent();
	InitLst();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CGeneralPrequisitesDlg::SetViewComponent()
{
	m_ctrlComboNetFramework.EnableWindow(m_nChkDotNet);
	m_ctrlComboInternetExplorer.EnableWindow(m_nChkInterExplorer);
	m_ctrlComboAcrobat.EnableWindow(m_nChkAcrobat);
	m_ctrlComboJava.EnableWindow(m_nChkJava);
	m_ctrlComboSQLCompact.EnableWindow(m_nChkSQLCompact);
	m_ctrlComboSQLExpress.EnableWindow(m_nChkSQLExpress);
	m_ctrlComboVCRuntimeX86.EnableWindow(m_nChkVCRuntime86);
	m_ctrlComboVCRuntimeX64.EnableWindow(g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64);
	m_ctrlComboVCRuntimeARM64.EnableWindow(g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64);
}

void CGeneralPrequisitesDlg::InitLst()
{
	m_listCtrlAddPre.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );
	m_listCtrlAddPre.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);


	m_listCtrlAddPre.InsertColumn(0, L"Name", LVCFMT_LEFT, 120);
	m_listCtrlAddPre.InsertColumn(1, L"Command", LVCFMT_LEFT,280);
	m_listCtrlAddPre.InsertColumn(2, L"Skip if failed", LVCFMT_LEFT,100);
}

void CGeneralPrequisitesDlg::OnBnClickedBtnDuplicate()
{
	// TODO: Add your control notification handler code here
	if (m_listCtrlAddPre.GetSelectedCount())
	{
		INT selidx = -1;
		selidx = m_listCtrlAddPre.GetNextItem(selidx, LVNI_SELECTED);

		//get prerequisite//
		ADDPREREQUISITE prereq;
		prereq = g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[selidx];

		//insert//
		INT nCount = m_listCtrlAddPre.GetItemCount();
		m_listCtrlAddPre.InsertItem(nCount, prereq.wszName); 
		m_listCtrlAddPre.SetItemText(nCount,1, prereq.wszCommand);
		m_listCtrlAddPre.SetItemText(nCount,2, GetChkString(prereq.chkSkipFailed));

		g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.push_back(prereq);
	}
	
}


void CGeneralPrequisitesDlg::OnBnClickedBtnApAdd()
{
	// TODO: Add your control notification handler code here
	CAddPrerequisite	dlg;
	if (dlg.DoModal() == IDOK)
	{
		BOOL bInstallationRefresh = FALSE;
		for (INT i = 0; i < dlg.m_vecAddedPre.size(); i++)
		{
			//insert into global data//
			g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.push_back(dlg.m_vecAddedPre[i]);

			//insert into list//
			INT nCount = m_listCtrlAddPre.GetItemCount();
			m_listCtrlAddPre.InsertItem(nCount, dlg.m_vecAddedPre[i].wszName);
			m_listCtrlAddPre.SetItemText(nCount, 1, dlg.m_vecAddedPre[i].wszCommand);
			m_listCtrlAddPre.SetItemText(nCount, 2, GetChkString(dlg.m_vecAddedPre[i].chkSkipFailed));

			//if local prerequisites, insert into temp dir/
			ADDINSTALLATIONFILES val;
			val.nChkDir = 0;
			wsprintf(val.wszSrcFullName, L"%s", dlg.m_vecAddedPre[i].wszCommand);

			CString strF = dlg.m_vecAddedPre[i].wszCommand;
			strF = strF.Right(strF.GetLength() - strF.ReverseFind('\\') - 1);
			wsprintf(val.wszDestName, L"%s", strF);
			wsprintf(val.wszDestFullName, L"%s\\%s", L"#SETUP_TEMP_DIR#", strF);
			if (strF.Find(L"http") != 0)
			{
				bInstallationRefresh = TRUE;
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);
			}			
		}

		//refresh installation\files and folders tab
		if (bInstallationRefresh)
			g_pMainDlgSheet->PostMessageW(UM_INSTALLATION_FILESANDFOLDERS_REFRESH, 1);
	}
}


void CGeneralPrequisitesDlg::OnBnClickedBtnApEdit()
{
	if(m_listCtrlAddPre.GetItemCount() > 0 && m_nSelAddPre > -1)
	{
		CAddPrerequisite	dlg;
		dlg.SetData(&g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre]);
		if (dlg.DoModal() == IDOK)
		{
			BOOL bInstallationRefresh = FALSE;

			//delete in temp list//
			for (int i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
			{
				if (wcscmp(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].wszCommand,
					g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszSrcFullName) == 0)
				{
					g_globaldata.install_sh2.filesandfolders.vecInstallationData.erase(g_globaldata.install_sh2.filesandfolders.vecInstallationData.begin() + i);
					bInstallationRefresh = TRUE;
					break;
				}
			}

			//get//
			ADDPREREQUISITE prereq;
			memcpy(&prereq, &dlg.m_vecAddedPre[0], sizeof(ADDPREREQUISITE));
			//insert//

			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 0, prereq.wszName);
			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 1, prereq.wszCommand);
			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 2, GetChkString(prereq.chkSkipFailed));
			memcpy(&g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre], &prereq, sizeof(ADDPREREQUISITE));

			//if local prerequisites, insert into temp dir//
			ADDINSTALLATIONFILES val;
			val.nChkDir = 0;
			wsprintf(val.wszSrcFullName, L"%s", prereq.wszCommand);

			CString strF = prereq.wszCommand;
			strF = strF.Right(strF.GetLength() - strF.ReverseFind('\\') - 1);
			wsprintf(val.wszDestName, L"%s", strF);
			wsprintf(val.wszDestFullName, L"%s\\%s", L"#SETUP_TEMP_DIR#", strF);
			if (strF.Find(L"http") != 0)
			{
				bInstallationRefresh = TRUE;
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);
			}

			//refresh installation\files and folders tab
			if (bInstallationRefresh)
				g_pMainDlgSheet->PostMessageW(UM_INSTALLATION_FILESANDFOLDERS_REFRESH, 1);
		}
	}
}


void CGeneralPrequisitesDlg::OnBnClickedBtnApRemove()
{
	CString str = L"Do you really want to remove?";
	if (m_listCtrlAddPre.GetSelectedCount() > 0 &&
		MessageBox(str, INSTALLER_NAME, MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		BOOL bInstallationRefresh = FALSE;

		//delete in temp list//
		for (int i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
		{
			if (wcscmp(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].wszCommand,
				g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszSrcFullName) == 0)
			{
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.erase(g_globaldata.install_sh2.filesandfolders.vecInstallationData.begin() + i);
				bInstallationRefresh = TRUE;
				break;
			}
		}

		m_listCtrlAddPre.DeleteItem(m_nSelAddPre);
		g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.erase(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.begin() + m_nSelAddPre);

		//refresh installation\files and folders tab
		if (bInstallationRefresh)
			g_pMainDlgSheet->PostMessageW(UM_INSTALLATION_FILESANDFOLDERS_REFRESH, 1);
	}
}


void CGeneralPrequisitesDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	INT nMoveSel = pNMUpDown->iDelta + m_nSelAddPre;

	if(nMoveSel < 0)
		nMoveSel = 0;

	if (nMoveSel >= m_listCtrlAddPre.GetItemCount())
	{
		nMoveSel = m_listCtrlAddPre.GetItemCount() - 1;
	}

	if(nMoveSel == m_nSelAddPre)
	{
		return;
	}
	else
	{
		ADDPREREQUISITE prereq;
		prereq = g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[nMoveSel];
		g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[nMoveSel] = g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre];
		g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre] = prereq;

		m_listCtrlAddPre.SetItemText(nMoveSel, 0, g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[nMoveSel].wszName); 
		m_listCtrlAddPre.SetItemText(nMoveSel, 1, g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[nMoveSel].wszCommand);
		m_listCtrlAddPre.SetItemText(nMoveSel, 2, GetChkString(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[nMoveSel].chkSkipFailed));

		m_listCtrlAddPre.SetItemText(m_nSelAddPre, 0, g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].wszName); 
		m_listCtrlAddPre.SetItemText(m_nSelAddPre, 1, g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].wszCommand);
		m_listCtrlAddPre.SetItemText(m_nSelAddPre, 2, GetChkString(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].chkSkipFailed));
		
		m_listCtrlAddPre.SetItemState(m_nSelAddPre, 0, LVIS_SELECTED);
		m_nSelAddPre = nMoveSel;

		m_listCtrlAddPre.SetItemState(m_nSelAddPre, LVIS_SELECTED, LVIS_SELECTED);
		
		m_listCtrlAddPre.SetFocus();
	}
	*pResult = 0;
}


CString CGeneralPrequisitesDlg::GetChkString(INT k)
{
	CString str;
	if (k == BST_CHECKED)
		str = L"Checked";
	else
		str = L"Unchecked";

	return str;
}

BOOL CGeneralPrequisitesDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	m_nChkDotNet = g_globaldata.general_sh1.prerequisites.chkNetFramework;
	m_nChkInterExplorer = g_globaldata.general_sh1.prerequisites.chkInternetExplorer;
	m_nChkAcrobat = g_globaldata.general_sh1.prerequisites.chkAcrobat;
	m_nChkJava = g_globaldata.general_sh1.prerequisites.chkJava;
	m_nChkSQLCompact = g_globaldata.general_sh1.prerequisites.chkSqlServerCompact;
	m_nChkSQLExpress = g_globaldata.general_sh1.prerequisites.chkSqlServerExpress;
	m_nChkVCRuntime86 = g_globaldata.general_sh1.prerequisites.chkVCRuntimeX86;

	m_nDotNetIdx = g_globaldata.general_sh1.prerequisites.nNetFrameworkMode;
	m_nInterExpIdx = g_globaldata.general_sh1.prerequisites.nInternetExplorerMode;
	m_nAcrobatIdx = g_globaldata.general_sh1.prerequisites.nAcrobatMode;
	m_nJavaIdx = g_globaldata.general_sh1.prerequisites.nJavaMode;
	m_nSqlCompactIdx = g_globaldata.general_sh1.prerequisites.nSqlServerCompactMode;
	m_nSqlExpressIdx = g_globaldata.general_sh1.prerequisites.nSqlServerExpressMode;
	m_nVCRuntimeX86Idx = g_globaldata.general_sh1.prerequisites.nVCRuntimeModeX86;

	//insert into list//
	m_listCtrlAddPre.DeleteAllItems();
	for (INT i = 0; i < g_globaldata.general_sh1.prerequisites.vecAddPrerequisites.size(); i++)
	{
		ADDPREREQUISITE prereq = g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[i];
		//insert//
		INT nCount = m_listCtrlAddPre.GetItemCount();
		m_listCtrlAddPre.InsertItem(nCount, prereq.wszName); 
		m_listCtrlAddPre.SetItemText(nCount,1, prereq.wszCommand);
		m_listCtrlAddPre.SetItemText(nCount,2, GetChkString(prereq.chkSkipFailed));
	}

	m_ctrlCheckNetFramework.SetCheck(m_nChkDotNet);
	m_ctrlCheckInternetExplorer.SetCheck(m_nChkInterExplorer);
	m_ctrlCheckAcrobat.SetCheck(m_nChkAcrobat);
	m_ctrlCheckJava.SetCheck(m_nChkJava);
	m_ctrlCheckSQLCompact.SetCheck(m_nChkSQLCompact);
	m_ctrlCheckSQLExpress.SetCheck(m_nChkSQLExpress);
	m_ctrlCheckVCRuntimeX86.SetCheck(m_nChkVCRuntime86);
	m_ctrlCheckVCRuntimeX64.SetCheck(g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64);
	m_ctrlCheckVCRuntimeARM64.SetCheck(g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64);

	m_ctrlComboNetFramework.SetCurSel(m_nDotNetIdx);
	m_ctrlComboInternetExplorer.SetCurSel(m_nInterExpIdx);
	m_ctrlComboAcrobat.SetCurSel(m_nAcrobatIdx);
	m_ctrlComboJava.SetCurSel(m_nJavaIdx);
	m_ctrlComboSQLCompact.SetCurSel(m_nSqlCompactIdx);
	m_ctrlComboSQLExpress.SetCurSel(m_nSqlExpressIdx);
	m_ctrlComboVCRuntimeX86.SetCurSel(m_nVCRuntimeX86Idx);
	m_ctrlComboVCRuntimeX64.SetCurSel(g_globaldata.general_sh1.prerequisites.nVCRuntimeModeX64);
	m_ctrlComboVCRuntimeARM64.SetCurSel(g_globaldata.general_sh1.prerequisites.nVCRuntimeModeARM64);

	SetViewComponent();

	return CPropertyPage::OnSetActive();
}


void CGeneralPrequisitesDlg::OnNMClickListAdditionalPrerequisites(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	m_nSelAddPre = pNMItemActivate->iItem;

	*pResult = 0;
}


void CGeneralPrequisitesDlg::OnNMDblclkListAdditionalPrerequisites(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nSelAddPre = pNMItemActivate->iItem;
	if(m_listCtrlAddPre.GetItemCount() > 0 && pNMItemActivate->iItem > -1)
	{
		CAddPrerequisite	dlg;
		dlg.SetData(&g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre]);
		if (dlg.DoModal() == IDOK)
		{
			BOOL bInstallationRefresh = FALSE;
			
			//delete in temp list//
			for (int i = 0; i < g_globaldata.install_sh2.filesandfolders.vecInstallationData.size(); i++)
			{
				if (wcscmp(g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre].wszCommand,
					g_globaldata.install_sh2.filesandfolders.vecInstallationData[i].wszSrcFullName) == 0)
				{
					g_globaldata.install_sh2.filesandfolders.vecInstallationData.erase(g_globaldata.install_sh2.filesandfolders.vecInstallationData.begin() + i);
					bInstallationRefresh = TRUE;
					break;
				}
			}

			//get//
			ADDPREREQUISITE prereq;
			memcpy(&prereq, &dlg.m_vecAddedPre[0], sizeof(ADDPREREQUISITE));
			//insert//

			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 0, prereq.wszName);
			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 1, prereq.wszCommand);
			m_listCtrlAddPre.SetItemText(m_nSelAddPre, 2, GetChkString(prereq.chkSkipFailed));
			memcpy(&g_globaldata.general_sh1.prerequisites.vecAddPrerequisites[m_nSelAddPre], &prereq, sizeof(ADDPREREQUISITE));

			//if local prerequisites, insert into temp dir//
			ADDINSTALLATIONFILES val;
			val.nChkDir = 0;
			wsprintf(val.wszSrcFullName, L"%s", prereq.wszCommand);

			CString strF = prereq.wszCommand;
			strF = strF.Right(strF.GetLength() - strF.ReverseFind('\\') - 1);
			wsprintf(val.wszDestName, L"%s", strF);
			wsprintf(val.wszDestFullName, L"%s\\%s", L"#SETUP_TEMP_DIR#", strF);
			if (strF.Find(L"http") != 0)
			{
				bInstallationRefresh = TRUE;
				g_globaldata.install_sh2.filesandfolders.vecInstallationData.push_back(val);
			}

			//refresh installation\files and folders tab
			if (bInstallationRefresh)
				g_pMainDlgSheet->PostMessageW(UM_INSTALLATION_FILESANDFOLDERS_REFRESH, 1);
		}
	}
	*pResult = 0;
}


void CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntimeX64()
{
	// TODO: Add your control notification handler code here
	g_globaldata.general_sh1.prerequisites.chkVCRuntimeX64 = m_ctrlCheckVCRuntimeX64.GetCheck();
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnBnClickedCheckVisualRuntimeArm64()
{
	// TODO: Add your control notification handler code here
	g_globaldata.general_sh1.prerequisites.chkVCRuntimeARM64 = m_ctrlCheckVCRuntimeARM64.GetCheck();
	SetViewComponent();
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeX64()
{
	// TODO: Add your control notification handler code here
	g_globaldata.general_sh1.prerequisites.nVCRuntimeModeX64 = m_ctrlComboVCRuntimeX64.GetCurSel();

	CString str;
	GetDlgItemText(IDC_COMBO_VISUAL_RUNTIME_X64, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszVCRuntimeX64, str.GetBuffer());
}


void CGeneralPrequisitesDlg::OnCbnSelchangeComboVisualRuntimeArm64()
{
	// TODO: Add your control notification handler code here
	g_globaldata.general_sh1.prerequisites.nVCRuntimeModeARM64 = m_ctrlComboVCRuntimeARM64.GetCurSel();

	CString str;
	GetDlgItemText(IDC_COMBO_VISUAL_RUNTIME_ARM64, str);
	wsprintf(g_globaldata.general_sh1.prerequisites.wszVCRuntimeARM64, str.GetBuffer());
}
