// InstallationFileEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationFileEditDlg.h"
#include "afxdialogex.h"


// CInstallationFileEditDlg dialog

IMPLEMENT_DYNAMIC(CInstallationFileEditDlg, CDialogEx)

CInstallationFileEditDlg::CInstallationFileEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstallationFileEditDlg::IDD, pParent)
{

}

CInstallationFileEditDlg::~CInstallationFileEditDlg()
{
}

void CInstallationFileEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OPTION_FILE_EXISTS, m_ctrlComboFileExist);
	DDX_Control(pDX, IDC_COMBO_DESTINATION_FOLDER, m_ctrlComboDestFolder);
	DDX_Control(pDX, IDC_CHECK_REMOVE_ON_UNINSTALL, m_ctrlCheckRemove);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_LAST_FOLDER_NAME, m_ctrlCheckInclude);
}


BEGIN_MESSAGE_MAP(CInstallationFileEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInstallationFileEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInstallationFileEditDlg message handlers

BOOL CInstallationFileEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitFunction();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInstallationFileEditDlg::InitFunction()
{
	m_ctrlComboDestFolder.SetWindowText(m_strDstDirName);
	SetDlgItemText(IDC_EDIT_PATH, m_strSrcName);
	SetDlgItemText(IDC_EDIT_DESTINATION_FILE_NAME, m_strDstFileName);

	m_ctrlCheckRemove.SetCheck(m_bRemoveUninstall);
	m_ctrlCheckInclude.SetCheck(m_bIncludeLastFolder);
	m_ctrlComboFileExist.SetCurSel(m_nFileExist);

	switch (m_type)
	{
	case CASE_ADD_FOLDER://Add folder
		{
			GetDlgItem(IDC_BTN_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
			m_ctrlComboDestFolder.EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_DESTINATION_FOLDER)->EnableWindow(TRUE);	
			GetDlgItem(IDC_EDIT_DESTINATION_FILE_NAME)->EnableWindow(FALSE);	
			m_ctrlComboFileExist.EnableWindow(TRUE);
			m_ctrlCheckRemove.EnableWindow(TRUE);
			m_ctrlCheckInclude.EnableWindow(TRUE);
		}
		break;
	case CASE_ADD_FILE: //add file
		{
			GetDlgItem(IDC_BTN_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(TRUE);
			m_ctrlComboDestFolder.EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_DESTINATION_FOLDER)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_DESTINATION_FILE_NAME)->EnableWindow(FALSE);			
			m_ctrlComboFileExist.EnableWindow(TRUE);
			m_ctrlCheckRemove.EnableWindow(FALSE);
			m_ctrlCheckInclude.EnableWindow(FALSE);
		}
		break;
	case CASE_FILE_EDIT://file edit
		{
		GetDlgItem(IDC_BTN_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);

		m_ctrlComboDestFolder.EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DESTINATION_FOLDER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DESTINATION_FILE_NAME)->EnableWindow(TRUE);			

		m_ctrlComboFileExist.EnableWindow(TRUE);
		m_ctrlCheckRemove.EnableWindow(FALSE);
		m_ctrlCheckInclude.EnableWindow(FALSE);

		}
		break;
	case CASE_FOLDER_EDIT://folder edit
		{
			//m_comCtrlFileExist.SetCurSel(m_nDstName);
			GetDlgItem(IDC_BTN_PATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);

			m_ctrlComboDestFolder.EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_DESTINATION_FOLDER)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_DESTINATION_FILE_NAME)->EnableWindow(FALSE);			

			m_ctrlComboFileExist.EnableWindow(TRUE);
			m_ctrlCheckRemove.EnableWindow(FALSE);
			m_ctrlCheckInclude.EnableWindow(FALSE);
		}
		break;
	}

	if (m_strDstDirName.IsEmpty())
	{
		m_ctrlComboDestFolder.SetCurSel(0);
	}
}

void CInstallationFileEditDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_PATH, m_strSrcName);
	m_ctrlComboDestFolder.GetWindowText(m_strDstDirName);
	GetDlgItemText(IDC_EDIT_DESTINATION_FILE_NAME, m_strDstFileName);

	m_nFileExist = m_ctrlComboFileExist.GetCurSel();
	m_bRemoveUninstall = m_ctrlCheckRemove.GetCheck();
	m_bIncludeLastFolder = m_ctrlCheckInclude.GetCheck();

	if(m_bIncludeLastFolder)
	{
		CString strLastDirName;
		strLastDirName = m_strSrcName.Right(m_strSrcName.GetLength() - m_strSrcName.ReverseFind('\\') - 1);
		m_strDstDirName += L"\\" + strLastDirName;
	}

	if (m_strDstDirName.ReverseFind(L'\\') == m_strDstDirName.GetLength() - 1)
		m_strDstDirName = m_strDstDirName.Left(m_strDstDirName.GetLength() - 1);

	CDialogEx::OnOK();
}

void CInstallationFileEditDlg::SetData(VIEW_TYPE nType, CString strSrc, CString strDest, INT nFileExsit, BOOL bRemoveUninstall, BOOL bIncludeLast)
{
	m_type = nType;
	m_strSrcName = strSrc;
	m_strDstDirName = strDest;
	m_nFileExist = nFileExsit;
	m_bRemoveUninstall = bRemoveUninstall;
	m_bIncludeLastFolder = bIncludeLast;

	if(m_type == CASE_FILE_EDIT)
	{
		INT index = m_strDstDirName.ReverseFind('\\');
		m_strDstFileName = m_strDstDirName.Right(m_strDstDirName.GetLength() - index - 1);
		m_strDstDirName = m_strDstDirName.Left(index);
	}
	else if(m_type == CASE_ADD_FILE)
	{
		m_strDstFileName = strSrc.Right(strSrc.GetLength() - strSrc.ReverseFind(L'\\') - 1);
	}
}