// CWizardMainBlankDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "CWizardMainBlankDlg.h"
#include "afxdialogex.h"


// CWizardMainBlankDlg dialog

IMPLEMENT_DYNAMIC(CWizardMainBlankDlg, CDialogEx)

CWizardMainBlankDlg::CWizardMainBlankDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INSTALLER_WIZARD_MAIN_BLANK_DIALOG, pParent)
{

}

CWizardMainBlankDlg::~CWizardMainBlankDlg()
{
}

void CWizardMainBlankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardMainBlankDlg, CDialogEx)
END_MESSAGE_MAP()


// CWizardMainBlankDlg message handlers


BOOL CWizardMainBlankDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
 	rect.left = -7;
 	rect.top = -28;

	m_sht.Create(this, WS_VISIBLE | WS_CHILD);
	m_sht.MoveWindow(&rect);
	m_sht.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
