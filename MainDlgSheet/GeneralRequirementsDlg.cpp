// GeneralRequirementsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GeneralRequirementsDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"
#include <math.h>


// CGeneralRequirementsDlg dialog

IMPLEMENT_DYNAMIC(CGeneralRequirementsDlg, CPropertyPage)

CGeneralRequirementsDlg::CGeneralRequirementsDlg()
	: CPropertyPage(CGeneralRequirementsDlg::IDD)
{
	
}

CGeneralRequirementsDlg::~CGeneralRequirementsDlg()
{
}

void CGeneralRequirementsDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BITS_32_64, m_ctrlComboBits);
	DDX_Control(pDX, IDC_CHECK_ADMINISTRATOR_PRIVILEGES, m_ctrlCheckAdminPrivileges);
	DDX_Control(pDX, IDC_CHECK_INTERNET_CONNECTION, m_ctrlCheckInternetConnection);
	DDX_Control(pDX, IDC_CHECK_CLOSE_RUNNING_APP1, m_ctrlCheckCloseRunningApp1);
	DDX_Control(pDX, IDC_CHECK_CLOSE_RUNNING_APP2, m_ctrlCheckCloseRunningApp2);

	for (int i = 0; i < OS_CNT; i++)
		DDX_Control(pDX, IDC_CHECK_OPERATING_WIN_XP + i, m_pctrlCheckOS[i]);
}


BEGIN_MESSAGE_MAP(CGeneralRequirementsDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_ADMINISTRATOR_PRIVILEGES, &CGeneralRequirementsDlg::OnBnClickedCheckAdministratorPrivileges)
	ON_BN_CLICKED(IDC_CHECK_INTERNET_CONNECTION, &CGeneralRequirementsDlg::OnBnClickedCheckInternetConnection)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_RUNNING_APP1, &CGeneralRequirementsDlg::OnBnClickedCheckCloseRunningApp1)
	ON_EN_CHANGE(IDC_EDIT_CRA1_FILE, &CGeneralRequirementsDlg::OnEnChangeEditCra1File)
	ON_EN_CHANGE(IDC_EDIT_CRA1_NAME, &CGeneralRequirementsDlg::OnEnChangeEditCra1Name)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_RUNNING_APP2, &CGeneralRequirementsDlg::OnBnClickedCheckCloseRunningApp2)
	ON_EN_CHANGE(IDC_EDIT_CRA2_FILE, &CGeneralRequirementsDlg::OnEnChangeEditCra2File)
	ON_EN_CHANGE(IDC_EDIT_CRA2_NAME, &CGeneralRequirementsDlg::OnEnChangeEditCra2Name)
	ON_CBN_SELCHANGE(IDC_COMBO_BITS_32_64, &CGeneralRequirementsDlg::OnCbnSelchangeComboBits3264)
END_MESSAGE_MAP()


// CGeneralRequirementsDlg message handlers


void CGeneralRequirementsDlg::OnBnClickedCheckAdministratorPrivileges()
{
	// TODO: Add your control notification handler code here
	m_nChkAdminPriv = m_ctrlCheckAdminPrivileges.GetCheck();
	g_globaldata.general_sh1.requires.chkAdminPrivieges = m_nChkAdminPriv;
}


void CGeneralRequirementsDlg::OnBnClickedCheckInternetConnection()
{
	// TODO: Add your control notification handler code here
	m_nChkInternetConnect = m_ctrlCheckInternetConnection.GetCheck();
	g_globaldata.general_sh1.requires.chkInternetConnection = m_nChkInternetConnect;
}


void CGeneralRequirementsDlg::OnBnClickedCheckCloseRunningApp1()
{
	// TODO: Add your control notification handler code here
	m_nChkCloseApp_1 = m_ctrlCheckCloseRunningApp1.GetCheck();
	g_globaldata.general_sh1.requires.chkCloseRunnigApp1 = m_nChkCloseApp_1;
	GetDlgItem(IDC_EDIT_CRA1_FILE)->EnableWindow(m_nChkCloseApp_1);
	GetDlgItem(IDC_EDIT_CRA1_NAME)->EnableWindow(m_nChkCloseApp_1);
}


void CGeneralRequirementsDlg::OnEnChangeEditCra1File()
{
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_CRA1_FILE, m_strCloseApp_1_file);
	wsprintf(g_globaldata.general_sh1.requires.wszCloseRunnigApp1_File, m_strCloseApp_1_file);
}


void CGeneralRequirementsDlg::OnEnChangeEditCra1Name()
{
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_CRA1_NAME, m_strCloseApp_1_name);
	wsprintf(g_globaldata.general_sh1.requires.wszCloseRunnigApp1_Name, m_strCloseApp_1_name);
	
}


void CGeneralRequirementsDlg::OnBnClickedCheckCloseRunningApp2()
{
	// TODO: Add your control notification handler code here
	m_nChkCloseApp_2 = m_ctrlCheckCloseRunningApp2.GetCheck();
	g_globaldata.general_sh1.requires.chkCloseRunnigApp2 = m_nChkCloseApp_2;
	GetDlgItem(IDC_EDIT_CRA2_FILE)->EnableWindow(m_nChkCloseApp_2);
	GetDlgItem(IDC_EDIT_CRA2_NAME)->EnableWindow(m_nChkCloseApp_2);
}


void CGeneralRequirementsDlg::OnEnChangeEditCra2File()
{
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_CRA2_FILE, m_strCloseApp_2_file);
	wsprintf(g_globaldata.general_sh1.requires.wszCloseRunnigApp2_File, m_strCloseApp_2_file);
}


void CGeneralRequirementsDlg::OnEnChangeEditCra2Name()
{
	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_CRA2_NAME, m_strCloseApp_2_name);
	wsprintf(g_globaldata.general_sh1.requires.wszCloseRunnigApp2_Name, m_strCloseApp_2_name);
}

void CGeneralRequirementsDlg::GetOeratingSystemParam()
{
	uint64_t k = 0;
	for (INT i = 0; i < OS_CNT; i++)
	{
		k = k | ((uint64_t)pow(2.0, i) * m_pChkedOS[i]);
	}

	g_globaldata.general_sh1.requires.nOperatingSystemChk = k;
}


void CGeneralRequirementsDlg::OnCbnSelchangeComboBits3264()
{
	// TODO: Add your control notification handler code here
	m_nOSBitIdx = m_ctrlComboBits.GetCurSel();
	g_globaldata.general_sh1.requires.nOperatingSystemMode = m_nOSBitIdx;
}


BOOL CGeneralRequirementsDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CGeneralRequirementsDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	for (INT i = 0; i < OS_CNT; i++)
	{
		INT k = g_globaldata.general_sh1.requires.nOperatingSystemChk & (INT)(pow(2.0, i));
		if (k > 0)		{m_pChkedOS[i] = 1;}
		else			{m_pChkedOS[i] = 0;}

		m_pctrlCheckOS[i].SetCheck(m_pChkedOS[i]);
	}

	m_nOSBitIdx = g_globaldata.general_sh1.requires.nOperatingSystemMode;
	m_nChkAdminPriv = g_globaldata.general_sh1.requires.chkAdminPrivieges;
	m_nChkInternetConnect = g_globaldata.general_sh1.requires.chkInternetConnection;
	m_nChkCloseApp_1 = g_globaldata.general_sh1.requires.chkCloseRunnigApp1;
	m_nChkCloseApp_2 = g_globaldata.general_sh1.requires.chkCloseRunnigApp2;

	m_strCloseApp_1_file.Format(L"%s", g_globaldata.general_sh1.requires.wszCloseRunnigApp1_File);
	m_strCloseApp_1_name.Format(L"%s", g_globaldata.general_sh1.requires.wszCloseRunnigApp1_Name);
	m_strCloseApp_2_file.Format(L"%s", g_globaldata.general_sh1.requires.wszCloseRunnigApp2_File);
	m_strCloseApp_2_name.Format(L"%s", g_globaldata.general_sh1.requires.wszCloseRunnigApp2_Name);

	m_ctrlComboBits.SetCurSel(m_nOSBitIdx);
	m_ctrlCheckAdminPrivileges.SetCheck(m_nChkAdminPriv);
	m_ctrlCheckInternetConnection.SetCheck(m_nChkInternetConnect);
	m_ctrlCheckCloseRunningApp1.SetCheck(m_nChkCloseApp_1);
	m_ctrlCheckCloseRunningApp2.SetCheck(m_nChkCloseApp_2);

	SetDlgItemText(IDC_EDIT_CRA1_FILE, m_strCloseApp_1_file);
	SetDlgItemText(IDC_EDIT_CRA1_NAME, m_strCloseApp_1_name);
	SetDlgItemText(IDC_EDIT_CRA2_FILE, m_strCloseApp_2_file);
	SetDlgItemText(IDC_EDIT_CRA2_NAME, m_strCloseApp_2_name);

	GetDlgItem(IDC_EDIT_CRA1_FILE)->EnableWindow(m_nChkCloseApp_1);
	GetDlgItem(IDC_EDIT_CRA1_NAME)->EnableWindow(m_nChkCloseApp_1);
	GetDlgItem(IDC_EDIT_CRA2_FILE)->EnableWindow(m_nChkCloseApp_2);
	GetDlgItem(IDC_EDIT_CRA2_NAME)->EnableWindow(m_nChkCloseApp_2);

	return CPropertyPage::OnSetActive();
}


LRESULT CGeneralRequirementsDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_COMMAND
		&& wParam >= IDC_CHECK_OPERATING_WIN_XP 
		&& wParam <= IDC_CHECK_OPERATING_WINSERVER_2019 )
	{
		m_pChkedOS[wParam - IDC_CHECK_OPERATING_WIN_XP] = m_pctrlCheckOS[wParam - IDC_CHECK_OPERATING_WIN_XP].GetCheck();
		GetOeratingSystemParam();
	}

	return CPropertyPage::WindowProc(message, wParam, lParam);
}
