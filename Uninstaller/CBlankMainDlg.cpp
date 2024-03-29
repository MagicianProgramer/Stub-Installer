// CBlankMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "CBlankMainDlg.h"
#include "afxdialogex.h"


// CBlankMainDlg dialog

IMPLEMENT_DYNAMIC(CBlankMainDlg, CDialogEx)

CBlankMainDlg::CBlankMainDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BLANK, pParent)
{

}

CBlankMainDlg::~CBlankMainDlg()
{
}

void CBlankMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBlankMainDlg, CDialogEx)
END_MESSAGE_MAP()


// CBlankMainDlg message handlers


BOOL CBlankMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	CRect rect;
	GetClientRect(&rect);
	rect.left = -7;
	rect.top = -30;

	m_blankSheet.Create(this, WS_VISIBLE | WS_CHILD);
	m_blankSheet.MoveWindow(&rect);
	m_blankSheet.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
