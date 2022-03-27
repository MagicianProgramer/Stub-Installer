// InterfaceLanguages.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceLanguagesDlg.h"
#include "afxdialogex.h"


// CInterfaceLanguages dialog

IMPLEMENT_DYNAMIC(CInterfaceLanguagesDlg, CPropertyPage)

CInterfaceLanguagesDlg::CInterfaceLanguagesDlg()
	: CPropertyPage(CInterfaceLanguagesDlg::IDD)
{

}

CInterfaceLanguagesDlg::~CInterfaceLanguagesDlg()
{
}

void CInterfaceLanguagesDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInterfaceLanguagesDlg, CPropertyPage)
END_MESSAGE_MAP()


// CInterfaceLanguages message handlers
