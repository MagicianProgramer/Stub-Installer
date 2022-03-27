// StubLicenseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Stub.h"
#include "StubLicenseDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"

// CStubLicenseDlg dialog

IMPLEMENT_DYNAMIC(CStubLicenseDlg, CDialogEx)

CStubLicenseDlg::CStubLicenseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStubLicenseDlg::IDD, pParent)
{
	m_pBuffer = NULL;
}

CStubLicenseDlg::~CStubLicenseDlg()
{
}

void CStubLicenseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_STUB_LICENSE, m_richLicense);
}


BEGIN_MESSAGE_MAP(CStubLicenseDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STUB_LICENSE_BACK, &CStubLicenseDlg::OnBnClickedBtnStubLicenseBack)
END_MESSAGE_MAP()


// CStubLicenseDlg message handlers


void CStubLicenseDlg::SetViewLicense(BYTE* pLicBuff)
{
	m_pBuffer = pLicBuff;	
}

BOOL CStubLicenseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_pBuffer)
	{
		CString strFile = g_globaldata.interface_sh3.main.wszOptionsLicenseFile;
		if(strFile.Right(3) == L"rtf")
		{
			CString str((char*)m_pBuffer);
			m_richLicense.SetWindowTextW(str);
		}
		else if (strFile.Right(3) == L"txt")
		{
			CStringA str;
			str.Format("%s", (LPCSTR)m_pBuffer);
			CString strReadMe(str);
			m_richLicense.SetWindowText(strReadMe);
		}	
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CStubLicenseDlg::OnBnClickedBtnStubLicenseBack()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
