// InstallationShortcutDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InstallationShortcutDetailDlg.h"
#include "afxdialogex.h"

#include "InterfaceSelectFileFolderDlg.h"


// CInstallationShortcutDetailDlg dialog

IMPLEMENT_DYNAMIC(CInstallationShortcutDetailDlg, CDialogEx)

CInstallationShortcutDetailDlg::CInstallationShortcutDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInstallationShortcutDetailDlg::IDD, pParent)
{
	m_strTargetFile = L"";
	m_strShortcutLocation = L"#PROGRAM_GROUP#";
	m_strShortcutName = L"";
	m_strDescription = L"";
	m_strParameters = L"";
	m_strWorkingDirectory = L"";
	m_strIconFile = L"";
	m_nIndex = 0;
	m_strShow = L"Normal";
}

CInstallationShortcutDetailDlg::~CInstallationShortcutDetailDlg()
{
}

void CInstallationShortcutDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SHORTCUT_LOCATION, m_ctrlComboShortcutLocation);
	DDX_Control(pDX, IDC_COMBO_SHORTCUTDETAIL_SHOW, m_ctrlComboShow);
}


BEGIN_MESSAGE_MAP(CInstallationShortcutDetailDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInstallationShortcutDetailDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TARGET_FILE, &CInstallationShortcutDetailDlg::OnBnClickedBtnTargetFile)
	ON_BN_CLICKED(IDC_BTN_ICON_FILE, &CInstallationShortcutDetailDlg::OnBnClickedBtnIconFile)
END_MESSAGE_MAP()


// CInstallationShortcutDetailDlg message handlers


void CInstallationShortcutDetailDlg::OnBnClickedOk()
{
	
	GetDlgItemText(IDC_EDIT_TARGET_FILE, m_strTargetFile);
	m_ctrlComboShortcutLocation.GetWindowText(m_strShortcutLocation);
	GetDlgItemText(IDC_EDIT_SHORTCUT_NAME, m_strShortcutName);
	GetDlgItemText(IDC_EDIT_DESCRIPTION, m_strDescription);
	GetDlgItemText(IDC_EDIT_PARAMETERS, m_strParameters);
	GetDlgItemText(IDC_EDIT_WORKING_DIRECTORY, m_strWorkingDirectory);
	GetDlgItemText(IDC_EDIT_ICON_FILE, m_strIconFile);
	m_nIndex = GetDlgItemInt(IDC_EDIT_ICON_INDEX);
	m_ctrlComboShow.GetWindowText(m_strShow);

	CDialogEx::OnOK();
}


void CInstallationShortcutDetailDlg::OnBnClickedBtnTargetFile()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	dlg.SetInitPathStr(m_strTargetFile);
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_TARGET_FILE, strPathName);

		//get working directory//
		CString strWorkingDir;
		INT k = strPathName.Find(L'\\');
		strWorkingDir = strPathName.Left(k);

		//get file name//
		CString strFileName;
		INT k1 = strPathName.ReverseFind(L'.');
		k = strPathName.ReverseFind(L'\\');
		strFileName = strPathName.Mid(k + 1, k1 - k - 1);
		
		m_strWorkingDirectory = strWorkingDir;
		m_strShortcutName = strFileName;

		//set//
		SetDlgItemText(IDC_EDIT_SHORTCUT_NAME, m_strShortcutName);
		SetDlgItemText(IDC_EDIT_WORKING_DIRECTORY, m_strWorkingDirectory);
	}
}


BOOL CInstallationShortcutDetailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlComboShow.SetCurSel(0);

	m_ctrlComboShow.SetWindowText(m_strShow);
	SetDlgItemText(IDC_EDIT_TARGET_FILE, m_strTargetFile);
	m_ctrlComboShortcutLocation.SetWindowTextW(m_strShortcutLocation);
	SetDlgItemText(IDC_EDIT_SHORTCUT_NAME, m_strShortcutName);
	SetDlgItemText(IDC_EDIT_DESCRIPTION, m_strDescription);
	SetDlgItemText(IDC_EDIT_PARAMETERS, m_strParameters);
	SetDlgItemText(IDC_EDIT_WORKING_DIRECTORY, m_strWorkingDirectory);
	SetDlgItemText(IDC_EDIT_ICON_FILE, m_strIconFile);
	SetDlgItemInt(IDC_EDIT_ICON_INDEX, m_nIndex);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInstallationShortcutDetailDlg::OnBnClickedBtnIconFile()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_ICON_FILE, strPathName);
	}
}
