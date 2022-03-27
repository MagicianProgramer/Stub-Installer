// AddCommand.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "AddCommand.h"
#include "afxdialogex.h"

#include "InterfaceSelectFileFolderDlg.h"
// CAddCommand dialog

IMPLEMENT_DYNAMIC(CAddCommand, CDialogEx)

CAddCommand::CAddCommand(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddCommand::IDD, pParent)
	, m_strLaunchOS(_T(""))
	, m_strShow(_T(""))
	, m_strTiming(_T(""))
{
	m_bSetData = FALSE;
}

CAddCommand::~CAddCommand()
{
}

void CAddCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_CBString(pDX, IDC_COMBO_LAUNCH, m_strLaunchOS);
	DDX_CBString(pDX, IDC_COMBO_SHOW, m_strShow);
	DDX_CBString(pDX, IDC_COMBO_TIMING, m_strTiming);
	DDX_Control(pDX, IDC_COMBO_LAUNCH, m_ctrlLaunch);
	DDX_Control(pDX, IDC_COMBO_SHOW, m_ctrlShow);
	DDX_Control(pDX, IDC_COMBO_TIMING, m_ctrlTiming);
	DDX_Control(pDX, IDC_CHECK_WAIT, m_ctrlWait);
	DDX_Control(pDX, IDC_CHECK_RUN, m_ctrlRun);
	DDX_Control(pDX, IDC_COMBO_FILE, m_ctrlFile);
}


BEGIN_MESSAGE_MAP(CAddCommand, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CAddCommand::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETERS, &CAddCommand::OnBnClickedButtonParameters)
	ON_BN_CLICKED(IDOK, &CAddCommand::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddCommand::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HELP, &CAddCommand::OnBnClickedButtonHelp)
END_MESSAGE_MAP()


// CAddCommand message handlers


BOOL CAddCommand::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	if (!m_bSetData)
	{
		m_ctrlLaunch.SetCurSel(0);
		m_ctrlShow.SetCurSel(0);
		m_ctrlTiming.SetCurSel(0);
		m_ctrlWait.SetCheck(BST_UNCHECKED);
		m_ctrlRun.SetCheck(BST_UNCHECKED);
	}
	else
	{
		SetDlgItemText(IDC_EDIT_PARAMETERS, m_strParams);
		m_ctrlFile.SetWindowTextW(m_strFile);
		m_ctrlLaunch.SetWindowTextW(m_strLaunchOS);
		m_ctrlShow.SetWindowTextW(m_strShow);
		m_ctrlTiming.SetWindowTextW(m_strTiming);
		m_ctrlWait.SetCheck(m_nWait);
		m_ctrlRun.SetCheck(m_nRun);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAddCommand::OnBnClickedButtonFile()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_COMBO_FILE, strPathName);
	}
}


void CAddCommand::OnBnClickedButtonParameters()
{
	// TODO: Add your control notification handler code here
	CInterfaceSelectFileFolderDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.m_strSectedFullName;//get from dlg//
		SetDlgItemText(IDC_EDIT_PARAMETERS, strPathName);
	}
}


void CAddCommand::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_PARAMETERS, m_strParams);
	m_ctrlFile.GetWindowTextW(m_strFile);
	m_ctrlLaunch.GetWindowTextW(m_strLaunchOS);
	m_ctrlShow.GetWindowTextW(m_strShow);
	m_ctrlTiming.GetWindowTextW(m_strTiming);
	m_nWait = m_ctrlWait.GetCheck();
	m_nRun = m_ctrlRun.GetCheck();

	CDialogEx::OnOK();
}


void CAddCommand::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CAddCommand::OnBnClickedButtonHelp()
{
	// TODO: Add your control notification handler code here
}

void CAddCommand::SetData(ADDCOMMAND *addCmd)
{
	m_strFile.Format(L"%s", addCmd->wszfile);
	m_strParams.Format(L"%s", addCmd->wszParameters);
	m_strLaunchOS.Format(L"%s", addCmd->wszLaunchOnOS);
	m_strShow.Format(L"%s", addCmd->wszShow);
	m_strTiming.Format(L"%s", addCmd->wszTiming);
	
	m_nWait = addCmd->chkWait4Process2Finish;
	m_nRun = addCmd->chkRunAsAdmin;
	m_bSetData = TRUE;
}

void CAddCommand::GetData(ADDCOMMAND *addCmd)
{
	wcscpy_s(addCmd->wszfile, m_strFile);
	wcscpy_s(addCmd->wszParameters, m_strParams);
	wcscpy_s(addCmd->wszLaunchOnOS, m_strLaunchOS);
	wcscpy_s(addCmd->wszShow, m_strShow);
	wcscpy_s(addCmd->wszTiming, m_strTiming);

	addCmd->chkWait4Process2Finish = m_nWait;
	addCmd->chkRunAsAdmin = m_nRun;
}