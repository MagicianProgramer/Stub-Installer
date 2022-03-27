// FontFileRenameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "FontFileRenameDlg.h"
#include "afxdialogex.h"


// CFontFileRenameDlg dialog

IMPLEMENT_DYNAMIC(CFontFileRenameDlg, CDialogEx)

CFontFileRenameDlg::CFontFileRenameDlg(CString strFile, CWnd* pParent /*=NULL*/)
	: CDialogEx(CFontFileRenameDlg::IDD, pParent)
{
	m_strRename = strFile;
}

CFontFileRenameDlg::~CFontFileRenameDlg()
{
}

void CFontFileRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFontFileRenameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFontFileRenameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFontFileRenameDlg message handlers


void CFontFileRenameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL CFontFileRenameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_EDIT_FONT_FILE_NAME, m_strRename);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
