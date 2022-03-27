// GenerateSNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GenerateSNDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"

char sztemp[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
// CGenerateSNDlg dialog

IMPLEMENT_DYNAMIC(CGenerateSNDlg, CDialogEx)

CGenerateSNDlg::CGenerateSNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGenerateSNDlg::IDD, pParent)
{

}

CGenerateSNDlg::~CGenerateSNDlg()
{
}

void CGenerateSNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGenerateSNDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_GENERATE_SERIAL_NUMBER_ADD, &CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberAdd)
	ON_BN_CLICKED(IDC_BTN_GENERATE_SERIAL_NUMBER_GEN, &CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberGen)
	ON_BN_CLICKED(IDC_BTN_GENERATE_SERIAL_NUMBER_CLEAR, &CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberClear)
END_MESSAGE_MAP()


// CGenerateSNDlg message handlers


void CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberAdd()
{
	// TODO: Add your control notification handler code here
	//GetDlgItemTextA(this->m_hWnd, IDC_EDIT_GENERATE_SERIAL_NUMBER_GENERATED, m_strLicenses);
	OnOK();
}


void CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberGen()
{
	// TODO: Add your control notification handler code here
	//clear//
	m_szLicenses = "";

	//get params//
	int nCount = GetDlgItemInt(IDC_EDIT_GENERATE_SERIAL_NUMBERS_COUNT);
	CString strTemplate;
	GetDlgItemText(IDC_EDIT_GENERATE_SERIAL_NUMBERS_TEMPLATE, strTemplate);

	if (strTemplate.GetLength() * nCount > _MAX_INSTALLER_STRING_LEN_ * 100)
		nCount = (int)(_MAX_INSTALLER_STRING_LEN_ * 100 / strTemplate.GetLength());

	//generate//
	for (int i = 0; i < nCount; i++)
	{
		CStringA strSN = "";
		for (int j = 0; j < strTemplate.GetLength(); j++)
		{
			char szA[2];
			memset(szA, 0, 2);
			if (strTemplate.Mid(j, 1) == L'#')//0-9
			{
				szA[0] = sztemp[rand() % 10];
			}
			else if (strTemplate.Mid(j, 1) == L'?')//A-Z
			{
				szA[0] = sztemp[(rand() % (strlen(sztemp) - 10)) + 10];
			}
			else if (strTemplate.Mid(j, 1) == L'*')//0-9,A-Z
			{
				szA[0] = sztemp[rand() % strlen(sztemp)];
			}
			else
			{
				szA[0] = '-';
			}

			strSN.Format("%s%s", strSN, szA);
		}

		m_szLicenses = m_szLicenses + strSN + "\r\n";
	}

	////////////////////////
	SetDlgItemTextA(this->m_hWnd, IDC_EDIT_GENERATE_SERIAL_NUMBER_GENERATED, m_szLicenses);
}


void CGenerateSNDlg::OnBnClickedBtnGenerateSerialNumberClear()
{
	// TODO: Add your control notification handler code here
	m_szLicenses = "";
	SetDlgItemTextA(this->m_hWnd, IDC_EDIT_GENERATE_SERIAL_NUMBER_GENERATED, m_szLicenses);
}


BOOL CGenerateSNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_EDIT_GENERATE_SERIAL_NUMBERS_COUNT, L"10");
	SetDlgItemText(IDC_EDIT_GENERATE_SERIAL_NUMBERS_TEMPLATE, L"####-####-####");

	srand(GetTickCount());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
