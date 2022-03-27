// InterfaceCustomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceCustomDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

// CInterfaceCustomDlg dialog

IMPLEMENT_DYNAMIC(CInterfaceCustomDlg, CPropertyPage)

CInterfaceCustomDlg::CInterfaceCustomDlg()
	: CPropertyPage(CInterfaceCustomDlg::IDD)
{
	
}

CInterfaceCustomDlg::~CInterfaceCustomDlg()
{
}

void CInterfaceCustomDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMPONENT, m_ctrlList);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_ctrlListItems);

	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_nCalculateMethod);
	DDX_Control(pDX, IDC_CHECK_CHECKED, m_ctrlCheckChecked);
	DDX_Control(pDX, IDC_CHECK_COMPONENT_SELECTED, m_ctrlCheckComponentAtLeast);
	DDX_Control(pDX, IDC_CHECK_COMPONENT_SIZE, m_ctrlCheckComponentCalculate);
	DDX_Control(pDX, IDC_COMBO_SIZE, m_ctrlComboComponent);
	DDX_Control(pDX, IDC_CHECK_SHOW_CUSTOM_CHECKBOX, m_ctrlCheckBoxChk);
	DDX_Control(pDX, IDC_CHECK_SHOW_CUSTOM_COMBOBOX, m_ctrlComboBoxChk);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_ATION, m_ctrlCheckComponent);
}


BEGIN_MESSAGE_MAP(CInterfaceCustomDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_SHOW_CUSTOM_CHECKBOX, &CInterfaceCustomDlg::OnBnClickedCheckBoxChk)
	ON_BN_CLICKED(IDC_CHECK_CHECKED, &CInterfaceCustomDlg::OnBnClickedCheckedChk)
	ON_BN_CLICKED(IDC_CHECK_SHOW_CUSTOM_COMBOBOX, &CInterfaceCustomDlg::OnBnClickedComboBoxChk)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_ATION, &CInterfaceCustomDlg::OnBnClickedSelectComponentsChk)
	ON_BN_CLICKED(IDC_CHECK_COMPONENT_SELECTED, &CInterfaceCustomDlg::OnBnClickedSelComponentsAtLeastChk)
	ON_BN_CLICKED(IDC_CHECK_COMPONENT_SIZE, &CInterfaceCustomDlg::OnBnClickedSelComponentsCalculateChk)
	ON_CBN_SELCHANGE(IDC_COMBO_SIZE, &CInterfaceCustomDlg::OnCbnSelchangeCalculateMethod)
	ON_EN_CHANGE(IDC_EDIT_CHECK_TEXT, &CInterfaceCustomDlg::OnEnChangeEditCheckText)
	ON_EN_CHANGE(IDC_EDIT_COMBO_TEXT, &CInterfaceCustomDlg::OnEnChangeEditComboText)
	ON_LBN_SELCHANGE(IDC_LIST_ITEMS, &CInterfaceCustomDlg::OnLbnSelchangeListItems)
	ON_EN_CHANGE(IDC_EDIT_COMBO_DEFAULT, &CInterfaceCustomDlg::OnEnChangeEditComboDefault)
	ON_EN_CHANGE(IDC_EDIT_COMPONENT_NAME, &CInterfaceCustomDlg::OnEnChangeEditComponentName)
	ON_EN_CHANGE(IDC_EDIT_COMPONENT_DESCRIPTION, &CInterfaceCustomDlg::OnEnChangeEditComponentDescription)
	ON_EN_CHANGE(IDC_LIST_ITEMS, &CInterfaceCustomDlg::OnEnChangeListItems)
END_MESSAGE_MAP()


// CInterfaceCustomDlg message handlers


void CInterfaceCustomDlg::OnBnClickedCheckBoxChk()
{
	m_bCheckboxChk = m_ctrlCheckBoxChk.GetCheck();
	g_globaldata.interface_sh3.custom.chkCheckBox = m_bCheckboxChk;

	m_ctrlCheckChecked.EnableWindow(m_bCheckboxChk);
	GetDlgItem(IDC_EDIT_CHECK_TEXT)->EnableWindow(m_bCheckboxChk);
}


void CInterfaceCustomDlg::OnBnClickedCheckedChk()
{
	m_bCheckedChk = m_ctrlCheckChecked.GetCheck();
	g_globaldata.interface_sh3.custom.chkChecked =  m_bCheckedChk;
}


void CInterfaceCustomDlg::OnBnClickedComboBoxChk()
{
	m_bComboBoxChk = m_ctrlComboBoxChk.GetCheck();
	g_globaldata.interface_sh3.custom.chkComboBox =  m_bComboBoxChk;

	GetDlgItem(IDC_EDIT_COMBO_TEXT)->EnableWindow(m_bComboBoxChk);
	m_ctrlListItems.EnableWindow(m_bComboBoxChk);
	GetDlgItem(IDC_EDIT_COMBO_DEFAULT)->EnableWindow(m_bComboBoxChk);
}


void CInterfaceCustomDlg::OnBnClickedSelectComponentsChk()
{
	m_bSelectComponentsChk = m_ctrlCheckComponent.GetCheck();
	g_globaldata.interface_sh3.custom.chkSelectComponents = m_bSelectComponentsChk;

	GetDlgItem(IDC_EDIT_COMPONENT_NAME)->EnableWindow(m_bSelectComponentsChk);
	GetDlgItem(IDC_EDIT_COMPONENT_DESCRIPTION)->EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentAtLeast.EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentCalculate.EnableWindow(m_bSelectComponentsChk);
	m_ctrlComboComponent.EnableWindow(m_bSelectComponentsChk);
}


void CInterfaceCustomDlg::OnBnClickedSelComponentsAtLeastChk()
{
	m_bSelComponentAtLeastChk = m_ctrlCheckComponentAtLeast.GetCheck();
	g_globaldata.interface_sh3.custom.chkSelComponentsAtLeast =  m_bSelComponentAtLeastChk;
}


void CInterfaceCustomDlg::OnBnClickedSelComponentsCalculateChk()
{
	m_bSelComponentCalculateChk = m_ctrlCheckComponentCalculate.GetCheck();
	g_globaldata.interface_sh3.custom.chkSelComponentsCalcuate =  m_bSelComponentCalculateChk;
}


void CInterfaceCustomDlg::OnCbnSelchangeCalculateMethod()
{
	m_nCalculateMethod = m_ctrlComboComponent.GetCurSel();
	g_globaldata.interface_sh3.custom.nSelComponentsCalculateMethodMode = m_nCalculateMethod;
}


void CInterfaceCustomDlg::OnEnChangeEditCheckText()
{
	GetDlgItemText(IDC_EDIT_CHECK_TEXT, m_strCheckBoxText);
	wcscpy(g_globaldata.interface_sh3.custom.wszCheckBoxText, m_strCheckBoxText);
}


void CInterfaceCustomDlg::OnEnChangeEditComboText()
{
	GetDlgItemText(IDC_EDIT_COMBO_TEXT, m_strComboBoxText);
	wcscpy(g_globaldata.interface_sh3.custom.wszComboBoxText, m_strComboBoxText);
}


void CInterfaceCustomDlg::OnLbnSelchangeListItems()
{
}


void CInterfaceCustomDlg::OnEnChangeEditComboDefault()
{
	m_nCustomComboDefault = GetDlgItemInt(IDC_EDIT_COMBO_DEFAULT);
	g_globaldata.interface_sh3.custom.nComboBoxDefaultIdx = m_nCustomComboDefault;
}


void CInterfaceCustomDlg::OnEnChangeEditComponentName()
{
	GetDlgItemText(IDC_EDIT_COMPONENT_NAME, m_strSelComponentName);
	wcscpy(g_globaldata.interface_sh3.custom.wszSelComponentsName, m_strSelComponentName);
}


void CInterfaceCustomDlg::OnEnChangeEditComponentDescription()
{
	GetDlgItemText(IDC_EDIT_COMPONENT_DESCRIPTION, m_strSelComponentDesc);
	wcscpy(g_globaldata.interface_sh3.custom.wszSelComponentsDescription, m_strSelComponentDesc);
}


BOOL CInterfaceCustomDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_ctrlCheckChecked.EnableWindow(m_bCheckboxChk);
	GetDlgItem(IDC_EDIT_CHECK_TEXT)->EnableWindow(m_bCheckboxChk);

	GetDlgItem(IDC_EDIT_COMBO_TEXT)->EnableWindow(m_bComboBoxChk);
	m_ctrlListItems.EnableWindow(m_bComboBoxChk);
	GetDlgItem(IDC_EDIT_COMBO_DEFAULT)->EnableWindow(m_bComboBoxChk);

	GetDlgItem(IDC_EDIT_COMPONENT_NAME)->EnableWindow(m_bSelectComponentsChk);
	GetDlgItem(IDC_EDIT_COMPONENT_DESCRIPTION)->EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentAtLeast.EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentCalculate.EnableWindow(m_bSelectComponentsChk);
	m_ctrlComboComponent.EnableWindow(m_bSelectComponentsChk);

	return TRUE; 
}


BOOL CInterfaceCustomDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	m_bCheckboxChk = g_globaldata.interface_sh3.custom.chkCheckBox;
	m_bCheckedChk = g_globaldata.interface_sh3.custom.chkChecked ;
	m_bComboBoxChk = g_globaldata.interface_sh3.custom.chkComboBox ;
	m_bSelectComponentsChk = g_globaldata.interface_sh3.custom.chkSelectComponents ;
	m_bSelComponentAtLeastChk = g_globaldata.interface_sh3.custom.chkSelComponentsAtLeast ;
	m_bSelComponentCalculateChk = g_globaldata.interface_sh3.custom.chkSelComponentsCalcuate ;
	m_nCustomComboDefault = g_globaldata.interface_sh3.custom.nComboBoxDefaultIdx;
	m_nCalculateMethod = g_globaldata.interface_sh3.custom.nSelComponentsCalculateMethodMode;
	m_strCheckBoxText = g_globaldata.interface_sh3.custom.wszCheckBoxText;
	m_strComboBoxText = g_globaldata.interface_sh3.custom.wszComboBoxText;
	m_strSelComponentName = g_globaldata.interface_sh3.custom.wszSelComponentsName;
	m_strSelComponentDesc = g_globaldata.interface_sh3.custom.wszSelComponentsDescription;
	m_strItems = g_globaldata.interface_sh3.custom.wszComboBoxItems;

	m_ctrlCheckBoxChk.SetCheck(m_bCheckboxChk);
	m_ctrlCheckChecked.SetCheck(m_bCheckedChk);
	SetDlgItemText(IDC_EDIT_CHECK_TEXT, m_strCheckBoxText);
	m_ctrlComboBoxChk.SetCheck(m_bComboBoxChk);
	SetDlgItemText(IDC_EDIT_COMBO_TEXT, m_strComboBoxText);
	SetDlgItemText(IDC_LIST_ITEMS, m_strItems);
	SetDlgItemInt(IDC_EDIT_COMBO_DEFAULT, m_nCustomComboDefault);
	m_ctrlCheckComponent.SetCheck(m_bSelectComponentsChk);
	SetDlgItemText(IDC_EDIT_COMPONENT_NAME, m_strSelComponentName);
	SetDlgItemText(IDC_EDIT_DESCRIPTION, m_strSelComponentDesc);
	m_ctrlCheckComponentAtLeast.SetCheck(m_bSelComponentAtLeastChk);
	m_ctrlCheckComponentCalculate.SetCheck(m_bSelComponentCalculateChk);

	m_ctrlComboComponent.SetCurSel(m_nCalculateMethod);
	
	m_ctrlCheckChecked.EnableWindow(m_bCheckboxChk);
	GetDlgItem(IDC_EDIT_CHECK_TEXT)->EnableWindow(m_bCheckboxChk);

	GetDlgItem(IDC_EDIT_COMBO_TEXT)->EnableWindow(m_bComboBoxChk);
	m_ctrlListItems.EnableWindow(m_bComboBoxChk);
	GetDlgItem(IDC_EDIT_COMBO_DEFAULT)->EnableWindow(m_bComboBoxChk);

	GetDlgItem(IDC_EDIT_COMPONENT_NAME)->EnableWindow(m_bSelectComponentsChk);
	GetDlgItem(IDC_EDIT_COMPONENT_DESCRIPTION)->EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentAtLeast.EnableWindow(m_bSelectComponentsChk);
	m_ctrlCheckComponentCalculate.EnableWindow(m_bSelectComponentsChk);
	m_ctrlComboComponent.EnableWindow(m_bSelectComponentsChk);

	return CPropertyPage::OnSetActive();
}


void CInterfaceCustomDlg::OnEnChangeListItems()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString str;
	GetDlgItemText(IDC_LIST_ITEMS, str);
	wsprintf(g_globaldata.interface_sh3.custom.wszComboBoxItems, str.GetBuffer());
}
