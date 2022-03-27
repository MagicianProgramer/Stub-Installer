// AddRegistryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "AddRegistryDlg.h"
#include "afxdialogex.h"
#include "InterfaceSelectFileFolderDlg.h"

// CAddRegistryDlg dialog

IMPLEMENT_DYNAMIC(CAddRegistryDlg, CDialogEx)

CAddRegistryDlg::CAddRegistryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddRegistryDlg::IDD, pParent)
	, m_strBitness(_T(""))
	, m_strValueType(_T(""))
{
	m_bSetData = TRUE;
}

CAddRegistryDlg::~CAddRegistryDlg()
{
}

void CAddRegistryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_VALUE_TYPE, m_strValueType);
	DDX_Control(pDX, IDC_COMBO_BITNESS, m_ctrlComboBitness);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE, m_ctrlComboValueType);
	DDX_Control(pDX, IDC_CHECK_OVERWRITE_IF_EXISTS, m_ctrlCheckOverWrite);
	DDX_Control(pDX, IDC_CHECK_REMOVE_ON_UNISTALL, m_ctrlCheckRemove);
}


BEGIN_MESSAGE_MAP(CAddRegistryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_VALUE_DATA, &CAddRegistryDlg::OnBnClickedButtonValueData)
	ON_BN_CLICKED(IDOK, &CAddRegistryDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddRegistryDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CAddRegistryDlg::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CAddRegistryDlg message handlers


BOOL CAddRegistryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (m_bSetData)
	{
		SetDlgItemTextW(IDC_EDIT_REGISTRY_KEY, L"HKEY_CURRENT_USER\\SoftWare\\#COMPANY_NAME#\\#APPLICATION_NAME#");
		m_ctrlCheckOverWrite.SetCheck(BST_CHECKED);
		m_ctrlCheckRemove.SetCheck(BST_CHECKED);
		m_ctrlComboBitness.SetCurSel(0);
		m_ctrlComboValueType.SetCurSel(0);
	}
	else
	{
		m_ctrlComboBitness.SetCurSel(m_ctrlComboBitness.FindString(-1, m_strBitness));
		m_ctrlComboValueType.SetCurSel(m_ctrlComboValueType.FindString(-1, m_strValueType));
		SetDlgItemText(IDC_EDIT_REGISTRY_KEY, m_strRegKey);
		SetDlgItemText(IDC_EDIT_REGISTRY_VALUE, m_strRegValue);
		SetDlgItemText(IDC_EDIT_VALUE_DATA, m_strValueData);

		m_ctrlCheckOverWrite.SetCheck(m_nOverwirte);
		m_ctrlCheckRemove.SetCheck(m_nRemoveOnUninstall);
	}

	return TRUE;  
}


void CAddRegistryDlg::OnBnClickedButtonValueData()
{
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_VALUE_DATA, strPathName);
	}
}


void CAddRegistryDlg::OnBnClickedOk()
{
	m_ctrlComboBitness.GetWindowTextW(m_strBitness);
	m_ctrlComboValueType.GetWindowTextW(m_strValueType);
	GetDlgItemText(IDC_EDIT_REGISTRY_KEY, m_strRegKey);
	GetDlgItemText(IDC_EDIT_REGISTRY_VALUE, m_strRegValue);
	GetDlgItemText(IDC_EDIT_VALUE_DATA, m_strValueData);

	m_nOverwirte = m_ctrlCheckOverWrite.GetCheck();
	m_nRemoveOnUninstall = m_ctrlCheckRemove.GetCheck();
	CDialogEx::OnOK();
}


void CAddRegistryDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CAddRegistryDlg::OnBnClickedButtonHelp()
{
	
}

void CAddRegistryDlg::SetData(ADDREGISTRYVALUE *addReg)
{
	m_strBitness.Format(L"%s", addReg->wszBitness);
	m_strRegKey.Format(L"%s", addReg->wszRegistry_key);
	m_strRegValue.Format(L"%s", addReg->wszRegistry_value);
	m_strValueType.Format(L"%s", addReg->wszValuetype);
	m_strValueData.Format(L"%s", addReg->wszValue_data);
	m_nOverwirte = addReg->chkOverwrite;
	m_nRemoveOnUninstall = addReg->chkRemove;
	m_bSetData = FALSE;
}

void CAddRegistryDlg::GetData(ADDREGISTRYVALUE *addReg)
{
	wcscpy_s(addReg->wszBitness, m_strBitness);
	wcscpy_s(addReg->wszRegistry_key, m_strRegKey);
	wcscpy_s(addReg->wszRegistry_value, m_strRegValue);
	wcscpy_s(addReg->wszValuetype, m_strValueType);
	wcscpy_s(addReg->wszValue_data, m_strValueData);
	addReg->chkOverwrite = m_nOverwirte;
	addReg->chkRemove = m_nRemoveOnUninstall;
}