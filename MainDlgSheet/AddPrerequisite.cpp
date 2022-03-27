// AddPrerequisite.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "AddPrerequisite.h"
#include "afxdialogex.h"


// CAddPrerequisite dialog

IMPLEMENT_DYNAMIC(CAddPrerequisite, CDialogEx)

CAddPrerequisite::CAddPrerequisite(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddPrerequisite::IDD, pParent)
{
	m_nSelCond = -1;
	m_bType = ADD_TYPE;
	m_vecAddedPre.clear();
}

CAddPrerequisite::~CAddPrerequisite()
{
}

void CAddPrerequisite::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADD_PREREQUISITE_COND, m_listCtrlCond);
	DDX_Control(pDX, IDC_COMBO_ADD_PREREQUISITE_COND_MATCH, m_ctrlComboCondMatch);
	DDX_Control(pDX, IDC_CHECK_ADD_PREREQUISITE_SKIP_FAILED, m_ctrlCheckSkip);
	DDX_Control(pDX, IDC_COMBO_ADD_PREREQUISITE_NAME, m_ctrlComboName);
}


BEGIN_MESSAGE_MAP(CAddPrerequisite, CDialogEx)
	ON_BN_CLICKED(IDC_BTN__ADD_PREREQUISITE_OPEN_COMMAND, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteOpenCommand)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_ADD, &CAddPrerequisite::OnBnClickedBtnAddToList)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_CHANGE, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteChange)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_REMOVE, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteRemove)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_OK, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteOk)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_CANCEL, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteCancel)
	ON_BN_CLICKED(IDC_BTN_ADD_PREREQUISITE_HELP, &CAddPrerequisite::OnBnClickedBtnAddPrerequisiteHelp)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ADD_PREREQUISITE_COND, &CAddPrerequisite::OnNMClickListAddPrerequisiteCond)
END_MESSAGE_MAP()


// CAddPrerequisite message handlers


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteOpenCommand()
{
	// TODO: Add your control notification handler code here
	CString sFilter = L"Execute File(*.exe)|*.exe|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_ALLOWMULTISELECT, sFilter, this);
	dlg.m_ofn.lpstrTitle = L"Add File ...";
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		SetDlgItemTextW(IDC_EDIT_ADD_PREREQUISITE_COMMAND, strPathName);
	}
}

CString GetChkString(INT k)
{
	CString str;
	if (k == BST_CHECKED)
		str = L"Checked";
	else
		str = L"Unchecked";

	return str;
}


void CAddPrerequisite::OnBnClickedBtnAddToList()
{
	// TODO: Add your control notification handler code here
	CString strName;
	GetDlgItemText(IDC_COMBO_ADD_PREREQUISITE_NAME, strName);
	CString strCommand;
	GetDlgItemText(IDC_EDIT_ADD_PREREQUISITE_COMMAND, strCommand);

	INT nSkip = IsDlgButtonChecked(IDC_CHECK_ADD_PREREQUISITE_SKIP_FAILED);
	CString strChecked = GetChkString(nSkip);
	
	//insert//
	INT nCount = m_listCtrlCond.GetItemCount();
	m_listCtrlCond.InsertItem(nCount, strName);
	m_listCtrlCond.SetItemText(nCount,1, strCommand);
	m_listCtrlCond.SetItemText(nCount,2, strChecked);

	//add to vector//
	ADDPREREQUISITE prereq;
	wsprintf(prereq.wszName, strName);
	wsprintf(prereq.wszCommand, strCommand);
	prereq.chkSkipFailed = nSkip;
	m_vecAddedPre.push_back(prereq);
}


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteChange()
{
	if(m_listCtrlCond.GetItemCount() > 0 && m_nSelCond > -1)
	{
		CString strCond, strObject, strValue;
		m_listCtrlCond.SetItemText(m_nSelCond, 0, strCond); 
		m_listCtrlCond.SetItemText(m_nSelCond,1, strObject);
		m_listCtrlCond.SetItemText(m_nSelCond,2, strValue);
	}
}


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteRemove()
{
	if(m_listCtrlCond.GetItemCount() > 0 && m_nSelCond > -1)
		m_listCtrlCond.DeleteItem(m_nSelCond);
}


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteOk()
{
	// TODO: Add your control notification handler code here
	if (m_bType == EDIT_TYPE)
	{
		CString strName;
		GetDlgItemText(IDC_COMBO_ADD_PREREQUISITE_NAME, strName);
		CString strCommand;
		GetDlgItemText(IDC_EDIT_ADD_PREREQUISITE_COMMAND, strCommand);

		INT nSkip = IsDlgButtonChecked(IDC_CHECK_ADD_PREREQUISITE_SKIP_FAILED);
	
		//add to vector//
		ADDPREREQUISITE prereq;
		wsprintf(prereq.wszName, strName);
		wsprintf(prereq.wszCommand, strCommand);
		prereq.chkSkipFailed = nSkip;
		m_vecAddedPre.push_back(prereq);
	}
	

	CDialogEx::OnOK();
}


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CAddPrerequisite::OnBnClickedBtnAddPrerequisiteHelp()
{
	// TODO: Add your control notification handler code here
}


BOOL CAddPrerequisite::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_ctrlComboName.SetCurSel(0);
	m_ctrlComboCondMatch.SetCurSel(0);
	m_ctrlCheckSkip.SetCheck(BST_UNCHECKED);
	SetDlgItemTextW(IDC_EDIT_ADD_PREREQUISITE_COMMAND, L"http://www.mysite.com/file.exe");

	InitLst();

	if(m_bType == EDIT_TYPE)
	{
		GetDlgItem(IDC_BTN_ADD_PREREQUISITE_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD_PREREQUISITE_REMOVE)->EnableWindow(FALSE);

		SetWindowText(L"Edit Current Item");
		SetDlgItemText(IDC_EDIT_ADD_PREREQUISITE_COMMAND, m_strCommand);
		m_ctrlComboName.SetWindowText(m_strName);
		m_ctrlCheckSkip.SetCheck(m_nChkSkip);
		
		CString m_strTemp = m_strConditions;
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAddPrerequisite::InitLst()
{
	m_listCtrlCond.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );
	m_listCtrlCond.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);

	m_listCtrlCond.InsertColumn(0, L"Name", LVCFMT_LEFT, 120);
	m_listCtrlCond.InsertColumn(1, L"Command", LVCFMT_LEFT,120);
	m_listCtrlCond.InsertColumn(2, L"Skip if failed to install", LVCFMT_LEFT,120);

}

void CAddPrerequisite::OnNMClickListAddPrerequisiteCond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_nSelCond = pNMItemActivate->iItem;
	if(m_listCtrlCond.GetItemCount() > 0 && m_nSelCond > -1)
	{
		
	}
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CAddPrerequisite::SetData(ADDPREREQUISITE *pAddData)
{
	m_bType = EDIT_TYPE;
	m_strName.Format(L"%s", pAddData->wszName);
	m_strCommand.Format(L"%s", pAddData->wszCommand);
	m_nChkSkip = pAddData->chkSkipFailed;
}
