// AddVariable.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "AddVariable.h"
#include "afxdialogex.h"

#include "InterfaceSelectFileFolderDlg.h"

// CAddVariable dialog

IMPLEMENT_DYNAMIC(CAddVariable, CDialogEx)

CAddVariable::CAddVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddVariable::IDD, pParent)
	, m_strBitness(_T(""))
	, m_strValueType(_T(""))
{
	m_bSetData = FALSE;
}

CAddVariable::~CAddVariable()
{
}

void CAddVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_REGISTRY_BITNESS, m_ctrlComboRegBitness);
	DDX_Control(pDX, IDC_COMBO_VALUE_TYPE, m_ctrlComboValueType);
}


BEGIN_MESSAGE_MAP(CAddVariable, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddVariable::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_DATA, &CAddVariable::OnBnClickedButtonDefaultData)
	ON_BN_CLICKED(IDCANCEL, &CAddVariable::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CAddVariable::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CAddVariable message handlers


BOOL CAddVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(!m_bSetData)
	{
		SetDlgItemTextW(IDC_EDIT_VARIABLE, L"<MyVar1>");
		SetDlgItemTextW(IDC_EDIT_REGISTRY_KEY, L"HKEY_CURRENT_USER\\Software\\#APPLICATION_NAME#");
		SetDlgItemTextW(IDC_EDIT_REGISTRY_VALUE, L"InstallPath");
		SetDlgItemTextW(IDC_EDIT_DEFAULT_DATA, L"#PROGRAM_FILES#\\#APPLICATION_NAME#");
		m_ctrlComboRegBitness.SetCurSel(0);
		m_ctrlComboValueType.SetCurSel(0);
	}
	else
	{
		SetDlgItemTextW(IDC_EDIT_VARIABLE, m_strVariable);
		SetDlgItemTextW(IDC_EDIT_REGISTRY_KEY, m_strRegKey);
		SetDlgItemTextW(IDC_EDIT_REGISTRY_VALUE, m_strRegValue);
		SetDlgItemTextW(IDC_EDIT_DEFAULT_DATA, m_strDefaultData);
		m_ctrlComboRegBitness.SetCurSel(m_ctrlComboRegBitness.FindString(-1, m_strBitness));
		m_ctrlComboValueType.SetCurSel(m_ctrlComboValueType.FindString(-1, m_strValueType));
	}
	return TRUE;  
}


void CAddVariable::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_VARIABLE, m_strVariable);
	GetDlgItemText(IDC_EDIT_REGISTRY_KEY, m_strRegKey);
	GetDlgItemText(IDC_EDIT_REGISTRY_VALUE, m_strRegValue);
	GetDlgItemText(IDC_EDIT_DEFAULT_DATA, m_strDefaultData);
	
	m_ctrlComboRegBitness.GetWindowTextW(m_strBitness);
	m_ctrlComboValueType.GetWindowTextW(m_strValueType);

	CDialogEx::OnOK();
}


void CAddVariable::OnBnClickedButtonDefaultData()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_DEFAULT_DATA, strPathName);
	}
}


void CAddVariable::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CAddVariable::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
}

void CAddVariable::SetData(ADDVARIABLE *addVar)
{
	m_bSetData = TRUE;
	m_strVariable.Format(L"%s", addVar->wszVariable);
	m_strRegKey.Format(L"%s", addVar->wszRegistryKey);
	m_strRegValue.Format(L"%s", addVar->wszRegistryValue);
	m_strDefaultData.Format(L"%s", addVar->wszDefault_data);
	m_strBitness.Format(L"%s", addVar->wszBitness);
	m_strValueType.Format(L"%s", addVar->wszValuetype);
}

void CAddVariable::GetData(ADDVARIABLE *addVar)
{
	wcscpy_s(addVar->wszVariable, m_strVariable);
	wcscpy_s(addVar->wszRegistryKey, m_strRegKey);
	wcscpy_s(addVar->wszRegistryValue, m_strRegValue);
	wcscpy_s(addVar->wszDefault_data, m_strDefaultData);
	wcscpy_s(addVar->wszBitness, m_strBitness);
	wcscpy_s(addVar->wszValuetype, m_strValueType);
}