// GeneralParametersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GeneralParametersDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"




wchar_t *CGeneralParametersDlg::GenerateProductGuid()
{
	GUID newId;
	CoCreateGuid(&newId);
	
	wchar_t bytes[MAX_PATH];
	wsprintf(bytes, L"{%08X-%04X-%04X-%X%X%X%X-%X%X%X}", newId.Data1, newId.Data2, newId.Data3
		, newId.Data4[0], newId.Data4[1], newId.Data4[2], newId.Data4[3], newId.Data4[4], newId.Data4[5], newId.Data4[6], newId.Data4[7]);

	wsprintf(g_globaldata.general_sh1.params.wszProductGUID, bytes);

	return bytes;
}
// CGeneralParametersDlg dialog

IMPLEMENT_DYNAMIC(CGeneralParametersDlg, CPropertyPage)

CGeneralParametersDlg::CGeneralParametersDlg()
	: CPropertyPage(CGeneralParametersDlg::IDD)
{
	m_nChangeBtnEnable = 0;
	GenerateProductGuid();
}

CGeneralParametersDlg::~CGeneralParametersDlg()
{
}

void CGeneralParametersDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INSTALL_PRODUCT_FOR, m_ctrlComboInstallPro);
	DDX_Control(pDX, IDC_CHECK_UNINSTALL_PREVIOUS_VERSION, m_ctrlCheckUninstPrev);
	DDX_Control(pDX, IDC_CHECK_UNINSTALL_SILENTLY, m_ctrlCheckUninstSilently);
	DDX_Control(pDX, IDC_CHECK_ALLOW_MULTIPLE_VERSIONS, m_ctrlCheckAllow);
}


BEGIN_MESSAGE_MAP(CGeneralParametersDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_CHANGE, &CGeneralParametersDlg::OnBnClickedBtnChange)
	ON_CBN_SELCHANGE(IDC_COMBO_INSTALL_PRODUCT_FOR, &CGeneralParametersDlg::OnCbnSelchangeComboInstallProductFor)
	ON_EN_CHANGE(IDC_EDIT_PRODUCT_GUID, &CGeneralParametersDlg::OnEnChangeEditProductGuid)
	ON_BN_CLICKED(IDC_CHECK_UNINSTALL_PREVIOUS_VERSION, &CGeneralParametersDlg::OnBnClickedCheckUninstallPreviousVersion)
	ON_BN_CLICKED(IDC_CHECK_UNINSTALL_SILENTLY, &CGeneralParametersDlg::OnBnClickedCheckUninstallSilently)
	ON_BN_CLICKED(IDC_CHECK_ALLOW_MULTIPLE_VERSIONS, &CGeneralParametersDlg::OnBnClickedCheckAllowMultipleVersions)
END_MESSAGE_MAP()


// CGeneralParametersDlg message handlers

void CGeneralParametersDlg::OnBnClickedBtnChange()
{
	// TODO: Add your control notification handler code here
	if (!m_nChangeBtnEnable)
	{
		CString str = L"Changing Product GUID might break version compatibility.\nDo you really want to change it?";
		if (MessageBox(str, INSTALLER_NAME, MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			//GetDlgItem(IDC_EDIT_PRODUCT_GUID)->EnableWindow(TRUE);
			m_nChangeBtnEnable = 1;
		}
	}
	else
	{
		//change GUID//
		GenerateProductGuid();
		m_strProductGUID.Format(L"%s", g_globaldata.general_sh1.params.wszProductGUID);
		SetDlgItemText(IDC_EDIT_PRODUCT_GUID, m_strProductGUID);
	}
}



void CGeneralParametersDlg::OnCbnSelchangeComboInstallProductFor()
{
	// TODO: Add your control notification handler code here
	m_nInstallProductIdx = m_ctrlComboInstallPro.GetCurSel();
	g_globaldata.general_sh1.params.nInstallProduct4 = m_nInstallProductIdx;
	
	if (g_globaldata.general_sh1.params.nInstallProduct4 == 0 || g_globaldata.general_sh1.params.nInstallProduct4 == 2)
	{
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#APP_DATA#\\#APPLICATION_NAME#");
	}
	else if (g_globaldata.general_sh1.params.nInstallProduct4 == 1 || g_globaldata.general_sh1.params.nInstallProduct4 == 3)
	{
		if (g_globaldata.install_sh2.filesandfolders.nChk64BitApplication)
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES#\\#APPLICATION_NAME#");
		}
		else
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES_X86#\\#APPLICATION_NAME#");
		}		
	}

	g_pMainDlgSheet->PostMessage(UM_INSTALLATION_FILESANDFOLDERS_REFRESH);
}


void CGeneralParametersDlg::OnEnChangeEditProductGuid()
{
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_PRODUCT_GUID, m_strProductGUID);
	wsprintf(g_globaldata.general_sh1.params.wszProductGUID, m_strProductGUID.GetBuffer());
}


void CGeneralParametersDlg::OnBnClickedCheckUninstallPreviousVersion()
{
	// TODO: Add your control notification handler code here
	m_nCHkUninstPrev = m_ctrlCheckUninstPrev.GetCheck();
	g_globaldata.general_sh1.params.chkOptionsUninstallPrevious = m_nCHkUninstPrev;
	g_globaldata.general_sh1.params.chkOptionsAllowMutiple = 0;
	CheckDlgButton(IDC_CHECK_ALLOW_MULTIPLE_VERSIONS, g_globaldata.general_sh1.params.chkOptionsAllowMutiple);
}


void CGeneralParametersDlg::OnBnClickedCheckUninstallSilently()
{
	// TODO: Add your control notification handler code here
	m_nChkUninstSilently = m_ctrlCheckUninstSilently.GetCheck();
	g_globaldata.general_sh1.params.chkOptionsUninstallSilently = m_nChkUninstSilently;
}


void CGeneralParametersDlg::OnBnClickedCheckAllowMultipleVersions()
{
	// TODO: Add your control notification handler code here
	m_nChkAllowMulti = m_ctrlCheckAllow.GetCheck();
	g_globaldata.general_sh1.params.chkOptionsAllowMutiple = m_nChkAllowMulti;
	g_globaldata.general_sh1.params.chkOptionsUninstallPrevious = 0;
	CheckDlgButton(IDC_CHECK_UNINSTALL_PREVIOUS_VERSION, g_globaldata.general_sh1.params.chkOptionsUninstallPrevious);
}


BOOL CGeneralParametersDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	m_nInstallModeIdx = (INT)g_globaldata.general_sh1.params.nInstallMode;
	m_nInstallProductIdx = (INT)g_globaldata.general_sh1.params.nInstallProduct4;
	m_strProductGUID.Format(L"%s", g_globaldata.general_sh1.params.wszProductGUID);
	m_nCHkUninstPrev = g_globaldata.general_sh1.params.chkOptionsUninstallPrevious;
	m_nChkUninstSilently = g_globaldata.general_sh1.params.chkOptionsUninstallSilently;
	m_nChkAllowMulti = g_globaldata.general_sh1.params.chkOptionsAllowMutiple;

	m_ctrlComboInstallPro.SetCurSel(m_nInstallProductIdx);
	m_ctrlCheckUninstPrev.SetCheck(m_nCHkUninstPrev);
	m_ctrlCheckUninstSilently.SetCheck(m_nChkUninstSilently);
	m_ctrlCheckAllow.SetCheck(m_nChkAllowMulti);

	SetDlgItemText(IDC_EDIT_PRODUCT_GUID, m_strProductGUID);

	return CPropertyPage::OnSetActive();
}


BOOL CGeneralParametersDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	GetDlgItem(IDC_CHECK_UNINSTALL_SILENTLY)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
