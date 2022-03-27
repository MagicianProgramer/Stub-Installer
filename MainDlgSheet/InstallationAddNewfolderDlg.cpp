// InstallationAddNewfolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationAddNewfolderDlg.h"
#include "afxdialogex.h"


// CInstallationAddNewfolderDlg dialog

IMPLEMENT_DYNAMIC(CInstallationAddNewfolderDlg, CDialogEx)

CInstallationAddNewfolderDlg::CInstallationAddNewfolderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstallationAddNewfolderDlg::IDD, pParent)
{
	m_strAddNewFolderName = L"Newfolder";
}

CInstallationAddNewfolderDlg::~CInstallationAddNewfolderDlg()
{
}

void CInstallationAddNewfolderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstallationAddNewfolderDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInstallationAddNewfolderDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInstallationAddNewfolderDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CInstallationAddNewfolderDlg message handlers


void CInstallationAddNewfolderDlg::OnBnClickedOk()
{
	GetDlgItemText(IDC_EDIT_ADD_NEWFOLDER_NAME, m_strAddNewFolderName);
	CDialogEx::OnOK();
}


void CInstallationAddNewfolderDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL CInstallationAddNewfolderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetDlgItemText(IDC_EDIT_ADD_NEWFOLDER_NAME, m_strAddNewFolderName);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
