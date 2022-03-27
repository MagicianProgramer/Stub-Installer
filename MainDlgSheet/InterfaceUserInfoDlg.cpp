// InterfaceUserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceUserInfoDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"
#include "GenerateSNDlg.h"
// CInterfaceUserInfoDlg dialog

IMPLEMENT_DYNAMIC(CInterfaceUserInfoDlg, CPropertyPage)

CInterfaceUserInfoDlg::CInterfaceUserInfoDlg()
	: CPropertyPage(CInterfaceUserInfoDlg::IDD)
{
	m_bRefresh = TRUE;
}

CInterfaceUserInfoDlg::~CInterfaceUserInfoDlg()
{
}

void CInterfaceUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIELD_TEXT, m_strFieldText);
	DDX_Text(pDX, IDC_EDIT_SERIAL_NUMBER, m_strSNList);

	DDX_Control(pDX, IDC_CHECK_USER_NAME, m_ctrlCheckUserName);
	DDX_Control(pDX, IDC_CHECK_INFORMATION_USER_EMAIL, m_ctrlCheckUserEmail);
	DDX_Control(pDX, IDC_CHECK_COMPANY_NAME, m_ctrlCheckCompany);
	DDX_Control(pDX, IDC_CHECK_CUSTOM_FIELD, m_ctrlCheckCustomField);
	DDX_Control(pDX, IDC_COMBO_FIELD_TYPE, m_ctrlComboFieldType);
	DDX_Control(pDX, IDC_CHECK_FILE_FOLDER_EXIST, m_ctrlCheckFileFolder);
	DDX_Control(pDX, IDC_CHECK_SERIAL_NUMBER, m_ctrlCheckSerialNumber);
	DDX_Control(pDX, IDC_CHECK_REQUIRED_USER_NAME, m_ctrlCheckUserNameReq);
	DDX_Control(pDX, IDC_CHECK_REQUIRED_USER_EMAIL, m_ctrlCheckUserEmailReq);
	DDX_Control(pDX, IDC_CHECK_REQUIRED_COMPANY_NAME, m_ctrlCheckCompanyReq);
	DDX_Control(pDX, IDC_CHECK_REQUIRED_CUSTOM_FIELD, m_ctrlCheckCustomReq);
	DDX_Control(pDX, IDC_CHECK_REQUIRED_SERIAL_NUMBER, m_ctrlCheckSerialReq);
}


BEGIN_MESSAGE_MAP(CInterfaceUserInfoDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_COMPANY_NAME, &CInterfaceUserInfoDlg::OnBnClickedCheckCompanyName)
	ON_BN_CLICKED(IDC_CHECK_CUSTOM_FIELD, &CInterfaceUserInfoDlg::OnBnClickedCheckCustomField)
	ON_BN_CLICKED(IDC_CHECK_REQUIRED_USER_NAME, &CInterfaceUserInfoDlg::OnBnClickedCheckRequiredUserName)
	ON_BN_CLICKED(IDC_CHECK_REQUIRED_COMPANY_NAME, &CInterfaceUserInfoDlg::OnBnClickedCheckRequiredCompanyName)
	ON_BN_CLICKED(IDC_CHECK_REQUIRED_CUSTOM_FIELD, &CInterfaceUserInfoDlg::OnBnClickedCheckRequiredCustomField)
	ON_EN_CHANGE(IDC_EDIT_FIELD_TEXT, &CInterfaceUserInfoDlg::OnEnChangeEditFieldText)
	ON_CBN_SELCHANGE(IDC_COMBO_FIELD_TYPE, &CInterfaceUserInfoDlg::OnCbnSelchangeComboFieldType)
	ON_BN_CLICKED(IDC_CHECK_FILE_FOLDER_EXIST, &CInterfaceUserInfoDlg::OnBnClickedCheckFileFolderExist)
	ON_BN_CLICKED(IDC_CHECK_SERIAL_NUMBER, &CInterfaceUserInfoDlg::OnBnClickedCheckSerialNumber)
	ON_BN_CLICKED(IDC_CHECK_REQUIRED_SERIAL_NUMBER, &CInterfaceUserInfoDlg::OnBnClickedCheckRequiredSerialNumber)
	ON_EN_CHANGE(IDC_EDIT_SERIAL_NUMBER, &CInterfaceUserInfoDlg::OnEnChangeSerialNumber)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CInterfaceUserInfoDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CInterfaceUserInfoDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, &CInterfaceUserInfoDlg::OnBnClickedButtonGenerate)
	ON_BN_CLICKED(IDC_CHECK_INFORMATION_USER_EMAIL, &CInterfaceUserInfoDlg::OnBnClickedCheckInformationUserEmail)
	ON_BN_CLICKED(IDC_CHECK_REQUIRED_USER_EMAIL, &CInterfaceUserInfoDlg::OnBnClickedCheckRequiredUserEmail)
	ON_BN_CLICKED(IDC_CHECK_USER_NAME, &CInterfaceUserInfoDlg::OnBnClickedCheckUserName)
END_MESSAGE_MAP()


// CInterfaceUserInfoDlg message handlers

void CInterfaceUserInfoDlg::OnBnClickedCheckCompanyName()
{
	m_bCompanyName = m_ctrlCheckCompany.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkCompanyName = m_bCompanyName;
	//required//
	g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired = g_globaldata.interface_sh3.userinfo.chkCompanyName;
	m_ctrlCheckCompanyReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkCompanyName);
	m_ctrlCheckCompanyReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCompanyName);
}


void CInterfaceUserInfoDlg::OnBnClickedCheckCustomField()
{
	m_bCustomField = m_ctrlCheckCustomField.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkCustomFiled = m_bCustomField;

	//required//
	g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired = g_globaldata.interface_sh3.userinfo.chkCustomFiled;
	m_ctrlCheckCustomReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
	m_ctrlCheckCustomReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
	GetDlgItem(IDC_EDIT_FIELD_TEXT)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
	m_ctrlComboFieldType.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
	m_ctrlCheckFileFolder.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
}


void CInterfaceUserInfoDlg::OnBnClickedCheckRequiredUserName()
{
	m_bUserNameRequired = m_ctrlCheckUserNameReq.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkUserNameRequired = m_bUserNameRequired;
}


void CInterfaceUserInfoDlg::OnBnClickedCheckRequiredCompanyName()
{
	m_bCompanyRequire = m_ctrlCheckCompanyReq.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired = m_bCompanyRequire;
}


void CInterfaceUserInfoDlg::OnBnClickedCheckRequiredCustomField()
{
	m_bCustomRequired = m_ctrlCheckCustomReq.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired = m_bCustomRequired;
}


void CInterfaceUserInfoDlg::OnEnChangeEditFieldText()
{
	GetDlgItemText(IDC_EDIT_FIELD_TEXT, m_strFieldText);
	wsprintf(g_globaldata.interface_sh3.userinfo.wszFieldText, m_strFieldText.GetBuffer());
}


void CInterfaceUserInfoDlg::OnCbnSelchangeComboFieldType()
{
	m_nFieldType = m_ctrlComboFieldType.GetCurSel();
	g_globaldata.interface_sh3.userinfo.nFieldTypeMode = m_nFieldType;
}


void CInterfaceUserInfoDlg::OnBnClickedCheckFileFolderExist()
{
	m_bFileFolder = m_ctrlCheckFileFolder.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkFieldTypeFileFolder = m_bFileFolder;
}


void CInterfaceUserInfoDlg::OnBnClickedCheckSerialNumber()
{
	m_bSerialNumber = m_ctrlCheckSerialNumber.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkSerialNumber = m_bSerialNumber;
	//required//
	g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired = g_globaldata.interface_sh3.userinfo.chkSerialNumber;
	m_ctrlCheckSerialReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
	m_ctrlCheckSerialReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
	GetDlgItem(IDC_EDIT_SERIAL_NUMBER)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
	GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
	GetDlgItem(IDC_BUTTON_GENERATE)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
}


void CInterfaceUserInfoDlg::OnBnClickedCheckRequiredSerialNumber()
{
	m_bSNRequired = m_ctrlCheckSerialReq.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired = m_bSNRequired;
}


void CInterfaceUserInfoDlg::OnEnChangeSerialNumber()
{
	GetDlgItemText(IDC_EDIT_SERIAL_NUMBER, m_strSNList);
}


void CInterfaceUserInfoDlg::OnBnClickedButtonLoad()
{
	CString filetype = L"Text Files(*.txt)|*.txt|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.GetPathName();

		CFile file;
		if (file.Open(strFile, CFile::modeRead|CFile::shareDenyNone))
		{
			char szbuffer[_MAX_INSTALLER_STRING_LEN_ * 100];
			file.Read(szbuffer, file.GetLength());
			file.Close();
			CString str(szbuffer);
			wsprintf(g_globaldata.interface_sh3.userinfo.wszSNList, str.GetBuffer());
			SetDlgItemText(IDC_EDIT_SERIAL_NUMBER, g_globaldata.interface_sh3.userinfo.wszSNList);
		}
	}
}


void CInterfaceUserInfoDlg::OnBnClickedButtonSave()
{
	CString filetype = L"Text Files(*.txt)|*.txt|";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		CStringA strA(g_globaldata.interface_sh3.userinfo.wszSNList);
		CString strFile = dlg.GetPathName();
		CString strtmp;
		strtmp = strFile.Right(4);
		if (strtmp != L".txt")
			strFile += L".txt";
		
		CFile file;
		if (file.Open(strFile, CFile::modeCreate|CFile::modeWrite))
		{
			file.Write(strA.GetBuffer(), strA.GetLength() + 1);
			file.Close();
		}
	}
}


void CInterfaceUserInfoDlg::OnBnClickedButtonGenerate()
{
	CGenerateSNDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		CString str(dlg.m_szLicenses);
		SetDlgItemText(IDC_EDIT_SERIAL_NUMBER, str);
		wsprintf(g_globaldata.interface_sh3.userinfo.wszSNList, str.GetBuffer());
	}
}


BOOL CInterfaceUserInfoDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bRefresh)
	{
		SetComponent(g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation);
		m_bRefresh = FALSE;		
	}


	return CPropertyPage::OnSetActive();
}


void CInterfaceUserInfoDlg::OnBnClickedCheckInformationUserEmail()
{
	g_globaldata.interface_sh3.userinfo.chkUserEmail = m_ctrlCheckUserEmail.GetCheck();

	//required//
	g_globaldata.interface_sh3.userinfo.chkUserEmailRequired = g_globaldata.interface_sh3.userinfo.chkUserEmail;
	m_ctrlCheckUserEmailReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkUserEmailRequired);
	m_ctrlCheckUserEmailReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkUserEmail);
}


void CInterfaceUserInfoDlg::OnBnClickedCheckRequiredUserEmail()
{
	g_globaldata.interface_sh3.userinfo.chkUserEmailRequired = m_ctrlCheckUserEmailReq.GetCheck();
}

void CInterfaceUserInfoDlg::OnBnClickedCheckUserName()
{
	// TODO: Add your control notification handler code here
	m_bUserName = m_ctrlCheckUserName.GetCheck();
	g_globaldata.interface_sh3.userinfo.chkUserName = m_bUserName;
	
	//required//
	g_globaldata.interface_sh3.userinfo.chkUserNameRequired = g_globaldata.interface_sh3.userinfo.chkUserName;
	m_ctrlCheckUserNameReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkUserNameRequired);
	m_ctrlCheckUserNameReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkUserName);
}


void CInterfaceUserInfoDlg::SetComponent(BOOL bEnable)
{
	if (!bEnable)
	{
		m_ctrlCheckUserName.EnableWindow(FALSE);
		m_ctrlCheckUserNameReq.EnableWindow(FALSE);
		m_ctrlCheckUserEmail.EnableWindow(FALSE);
		m_ctrlCheckUserEmailReq.EnableWindow(FALSE);
		m_ctrlCheckCompany.EnableWindow(FALSE);
		m_ctrlCheckCompanyReq.EnableWindow(FALSE);
		m_ctrlCheckCustomField.EnableWindow(FALSE);
		m_ctrlCheckCustomReq.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FIELD_TEXT)->EnableWindow(FALSE);
		m_ctrlComboFieldType.EnableWindow(FALSE);
		m_ctrlCheckFileFolder.EnableWindow(FALSE);
		m_ctrlCheckSerialNumber.EnableWindow(FALSE);
		m_ctrlCheckSerialReq.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SERIAL_NUMBER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GENERATE)->EnableWindow(FALSE);

		////////////////////////////////////////////////////
		g_globaldata.interface_sh3.userinfo.chkUserName = 0;
		g_globaldata.interface_sh3.userinfo.chkCompanyName = 0;
		g_globaldata.interface_sh3.userinfo.chkCustomFiled = 0;
		g_globaldata.interface_sh3.userinfo.chkUserNameRequired = 0;
		g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired = 0;
		g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired = 0;
		wsprintf(g_globaldata.interface_sh3.userinfo.wszFieldText, L"");
		g_globaldata.interface_sh3.userinfo.chkFieldTypeFileFolder = 0;
		g_globaldata.interface_sh3.userinfo.chkSerialNumber = 0;
		g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired = 0;
		wsprintf(g_globaldata.interface_sh3.userinfo.wszSNList, L"");
		g_globaldata.interface_sh3.userinfo.nFieldTypeMode = 0;
		g_globaldata.interface_sh3.userinfo.chkUserEmail = 0;
		g_globaldata.interface_sh3.userinfo.chkUserEmailRequired = 0;
	}
	else
	{
		m_ctrlCheckUserName.EnableWindow(TRUE);
		m_ctrlCheckUserEmail.EnableWindow(TRUE);
		m_ctrlCheckCompany.EnableWindow(TRUE);
		m_ctrlCheckCustomField.EnableWindow(TRUE);
		m_ctrlCheckSerialNumber.EnableWindow(TRUE);

		m_ctrlCheckUserNameReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkUserName);
		m_ctrlCheckUserEmailReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkUserEmail);
		m_ctrlCheckCompanyReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCompanyName);
		m_ctrlCheckCustomReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
		GetDlgItem(IDC_EDIT_FIELD_TEXT)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
		m_ctrlComboFieldType.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
		m_ctrlCheckFileFolder.EnableWindow(g_globaldata.interface_sh3.userinfo.chkCustomFiled);
		m_ctrlCheckSerialReq.EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
		GetDlgItem(IDC_EDIT_SERIAL_NUMBER)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
		GetDlgItem(IDC_BUTTON_LOAD)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);
		GetDlgItem(IDC_BUTTON_GENERATE)->EnableWindow(g_globaldata.interface_sh3.userinfo.chkSerialNumber);


		m_bUserName = g_globaldata.interface_sh3.userinfo.chkUserName;
		m_bCompanyName = g_globaldata.interface_sh3.userinfo.chkCompanyName;
		m_bCustomField = g_globaldata.interface_sh3.userinfo.chkCustomFiled;
		m_bUserNameRequired = g_globaldata.interface_sh3.userinfo.chkUserNameRequired;
		m_bCompanyRequire = g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired;
		m_bCustomRequired = g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired;
		m_strFieldText = g_globaldata.interface_sh3.userinfo.wszFieldText;
		m_bFileFolder = g_globaldata.interface_sh3.userinfo.chkFieldTypeFileFolder;
		m_bSerialNumber = g_globaldata.interface_sh3.userinfo.chkSerialNumber;
		m_bSNRequired = g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired;
		m_strSNList = g_globaldata.interface_sh3.userinfo.wszSNList;
		m_nFieldType = g_globaldata.interface_sh3.userinfo.nFieldTypeMode;

		m_ctrlCheckUserName.SetCheck(m_bUserName);
		m_ctrlCheckCompany.SetCheck(m_bCompanyName);
		m_ctrlCheckCustomField.SetCheck(m_bCustomField);
		m_ctrlCheckUserNameReq.SetCheck(m_bUserNameRequired);
		m_ctrlCheckCompanyReq.SetCheck(m_bCompanyRequire);
		m_ctrlCheckCustomReq.SetCheck(m_bCustomRequired);

		SetDlgItemText(IDC_EDIT_FIELD_TEXT, m_strFieldText);
		m_ctrlComboFieldType.SetCurSel(m_nFieldType);
		m_ctrlCheckFileFolder.SetCheck(m_bFileFolder);
		m_ctrlCheckSerialNumber.SetCheck(m_bSerialNumber);
		m_ctrlCheckSerialReq.SetCheck(m_bSNRequired);

		SetDlgItemText(IDC_EDIT_SERIAL_NUMBER, m_strSNList);
		m_ctrlCheckUserEmail.SetCheck(g_globaldata.interface_sh3.userinfo.chkUserEmail);
		m_ctrlCheckUserEmailReq.SetCheck(g_globaldata.interface_sh3.userinfo.chkUserEmailRequired);
	}
}