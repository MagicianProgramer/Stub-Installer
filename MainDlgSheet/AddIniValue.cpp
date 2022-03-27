// AddIniValue.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "AddIniValue.h"
#include "afxdialogex.h"

#include "InterfaceSelectFileFolderDlg.h"

// CAddIniValue dialog

IMPLEMENT_DYNAMIC(CAddIniValue, CDialogEx)

CAddIniValue::CAddIniValue(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddIniValue::IDD, pParent)
{
	m_nSkip = 0;
	m_nOverwrite = 0;
	m_nRemoveUninstall = 0;
}

CAddIniValue::~CAddIniValue()
{
}

void CAddIniValue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_SKIP, m_ctrlCheckSkip);
	DDX_Control(pDX, IDC_CHECK_OVERWRITE, m_ctrlOverwrite);
	DDX_Control(pDX, IDC_CHECK_REMOVE, m_ctrlRemove);
}


BEGIN_MESSAGE_MAP(CAddIniValue, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddIniValue::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddIniValue::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CAddIniValue::OnBnClickedButtonHelp)
	ON_BN_CLICKED(IDC_BUTTON_INI, &CAddIniValue::OnBnClickedButtonIni)
	ON_BN_CLICKED(IDC_BUTTON_VALUE, &CAddIniValue::OnBnClickedButtonValue)
END_MESSAGE_MAP()


// CAddIniValue message handlers


BOOL CAddIniValue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlCheckSkip.SetCheck(m_nSkip);
	m_ctrlOverwrite.SetCheck(m_nOverwrite);
	m_ctrlRemove.SetCheck(m_nRemoveUninstall);

	SetDlgItemText(IDC_EDIT_INI, m_strIniFile);
	SetDlgItemText(IDC_EDIT_SECTION, m_strSection);
	SetDlgItemText(IDC_EDIT_KEY, m_strKey);
	SetDlgItemText(IDC_EDIT_VALUE, m_strValue);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddIniValue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_INI, m_strIniFile);
	GetDlgItemText(IDC_EDIT_SECTION, m_strSection);
	GetDlgItemText(IDC_EDIT_KEY, m_strKey);
	GetDlgItemText(IDC_EDIT_VALUE, m_strValue);

	m_nSkip = m_ctrlCheckSkip.GetCheck();
	m_nOverwrite = m_ctrlOverwrite.GetCheck();
	m_nRemoveUninstall = m_ctrlRemove.GetCheck();

	CDialogEx::OnOK();
}


void CAddIniValue::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CAddIniValue::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
}


void CAddIniValue::OnBnClickedButtonIni()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_INI, strPathName);
	}
}


void CAddIniValue::OnBnClickedButtonValue()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_VALUE, strPathName);
	}
}

void CAddIniValue::SetData(ADDINIVALUE *addIni)
{
	m_strIniFile.Format(L"%s", addIni->wszInifile);
	m_strSection.Format(L"%s", addIni->wszSection);
	m_strKey.Format(L"%s", addIni->wszKey);
	m_strValue.Format(L"%s", addIni->wszValue);
	
	m_nSkip = addIni->chkSkip;
	m_nOverwrite = addIni->chkOverwrite;
	m_nRemoveUninstall = addIni->chkRemove;
}

void CAddIniValue::GetData(ADDINIVALUE *addIni)
{
	wsprintf(addIni->wszInifile, m_strIniFile.GetBuffer());
	wsprintf(addIni->wszSection, m_strSection.GetBuffer());
	wsprintf(addIni->wszKey, m_strKey.GetBuffer());
	wsprintf(addIni->wszValue, m_strValue.GetBuffer());

	addIni->chkSkip = m_nSkip;
	addIni->chkOverwrite = m_nOverwrite;
	addIni->chkRemove = m_nRemoveUninstall;
}