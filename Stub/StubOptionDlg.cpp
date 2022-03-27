// StubOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Stub.h"
#include "StubOptionDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"
#include "../common/other.h"


// CStubOptionDlg dialog

CString strUserName = L"";
CString strUserEmail = L"";
CString strCompany = L"";
CString strFieldText = L"";
CString strSN = L"";

IMPLEMENT_DYNAMIC(CStubOptionDlg, CDialogEx)

CStubOptionDlg::CStubOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStubOptionDlg::IDD, pParent)
	, m_nInstallProductFor(0)
{

}

CStubOptionDlg::~CStubOptionDlg()
{
}

void CStubOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER, m_nInstallProductFor);
}


BEGIN_MESSAGE_MAP(CStubOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_STUB_OPTION_DESTINATION_FOLDER_OPEN, &CStubOptionDlg::OnBnClickedBtnStubOptionDestinationFolderOpen)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_DESTINATION_FOLDER, &CStubOptionDlg::OnEnChangeEditStubOptionDestinationFolder)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_USER_NAME, &CStubOptionDlg::OnEnChangeEditStubOptionUserName)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_USER_EMAIL, &CStubOptionDlg::OnEnChangeEditStubOptionUserEmail)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_COMPANY, &CStubOptionDlg::OnEnChangeEditStubOptionCompany)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_CUSTOM_FIELD, &CStubOptionDlg::OnEnChangeEditStubOptionCustomField)
	ON_EN_CHANGE(IDC_EDIT_STUB_OPTION_SERIAL_NUMBER, &CStubOptionDlg::OnEnChangeEditStubOptionSerialNumber)
	ON_BN_CLICKED(IDOK, &CStubOptionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER, &CStubOptionDlg::OnBnClickedRadioInstallProductForCurrentUser)
	ON_BN_CLICKED(IDC_RADIO_INSTALL_PRODUCT_FOR_ALL_USERS, &CStubOptionDlg::OnBnClickedRadioInstallProductForAllUsers)
END_MESSAGE_MAP()


// CStubOptionDlg message handlers


void CStubOptionDlg::OnBnClickedBtnStubOptionDestinationFolderOpen()
{
	// TODO: Add your control notification handler code here
	CFolderPickerDialog folderpickerdlg(NULL, OFN_READONLY | /*OFN_OVERWRITEPROMPT |*/ OFN_ALLOWMULTISELECT, this);
	folderpickerdlg.m_ofn.lpstrTitle  = _T("Select Install Folder");
	if (folderpickerdlg.DoModal() == IDOK)
	{
		//...
		g_strInstDir = folderpickerdlg.GetPathName();
		SetDlgItemText(IDC_EDIT_STUB_OPTION_DESTINATION_FOLDER, g_strInstDir);
	}
}


void CStubOptionDlg::OnEnChangeEditStubOptionDestinationFolder()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_INSTALL_DIR, g_strInstDir);
}


BOOL CStubOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	// set install product for //
	m_nInstallProductFor = g_globaldata.general_sh1.params.nInstallProduct4 % 2;
	UpdateData(FALSE);

	if (g_globaldata.general_sh1.params.nInstallProduct4 < 2)
	{
		GetDlgItem(IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_INSTALL_PRODUCT_FOR_ALL_USERS)->EnableWindow(FALSE);
	}

	//set dir//
	SetDlgItemText(IDC_EDIT_STUB_OPTION_DESTINATION_FOLDER, g_strInstDir);

	//display required free space//
	CString strTxt;
	UINT64	nReqSpace = g_globaldata.install_sh2.advanced.nRequiredSpace;
	strTxt = L"Required Free Space : ";
	if (nReqSpace < 1024)
		strTxt.AppendFormat(L"%d bytes", nReqSpace);
	else if(nReqSpace < 1024 * 1024)
		strTxt.AppendFormat(L"%.2f KB", (float)nReqSpace / 1024.0);
	else if (nReqSpace < 1024 * 1024 *1024)
		strTxt.AppendFormat(L"%.2f MB", (float)nReqSpace / (1024.0 * 1024.0));
	else
		strTxt.AppendFormat(L"%.2f GB", (float)nReqSpace / (1024.0 * 1024.0 * 1024.0));
	GetDlgItem(IDC_STATIC_TUB_OPTION_REQUIRED_FREE_SPACE)->SetWindowText(strTxt);

	//display available free space//
	DWORD dwSecsPerCluster = 0;
	DWORD dwBytesPerSector = 0;
	DWORD dwFreeClusters = 0;
	DWORD dwTotalClusters = 0;
	CString strDrive = g_strInstDir.Left(g_strInstDir.Find(L"\\"));
	if(GetDiskFreeSpace(strDrive, &dwSecsPerCluster, &dwBytesPerSector, &dwFreeClusters, &dwTotalClusters))
	{
		UINT64 freespace = 0;
		freespace = (UINT64)dwSecsPerCluster * (UINT64)dwBytesPerSector * (UINT64)dwFreeClusters;	
		strTxt = L"Available Free Space : ";
		if (freespace < 1024)
			strTxt.AppendFormat(L"%d bytes", freespace);
		else if(freespace < 1024 * 1024)
			strTxt.AppendFormat(L"%.2f KB", (float)freespace / 1024.0);
		else if (freespace < 1024 * 1024 *1024)
			strTxt.AppendFormat(L"%.2f MB", (float)freespace / (1024.0 * 1024.0));
		else
			strTxt.AppendFormat(L"%.2f GB", (float)freespace / (1024.0 * 1024.0 * 1024.0));
		GetDlgItem(IDC_STATIC_STUB_OPTION_AVAILABLE_FREE_SPACE)->SetWindowText(strTxt);
	}

	SetUserInformation();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CStubOptionDlg::SetUserInformation()
{
	//user information//
	if (!g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_USER_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STUB_OPTION_USER_EMAIL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STUB_OPTION_COMPANY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STUB_OPTION_CUSTOM_FIELD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_STUB_OPTION_SERIAL_NUMBER)->EnableWindow(FALSE);
		return;
	}

	if (!g_globaldata.interface_sh3.userinfo.chkUserName)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_USER_NAME)->EnableWindow(FALSE);
	}
	else
	{
		char szbuffer[MAX_PATH];
		DWORD len = MAX_PATH;
		GetUserNameA(szbuffer, &len);
		CString str(szbuffer);
		GetDlgItem(IDC_EDIT_STUB_OPTION_USER_NAME)->SetWindowText(str);
		if (strUserName.IsEmpty())
		{
			strUserName = str;
		}
	}
	

	if (!g_globaldata.interface_sh3.userinfo.chkUserEmail)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_USER_EMAIL)->EnableWindow(FALSE);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkCompanyName)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_COMPANY)->EnableWindow(FALSE);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkCustomFiled)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_CUSTOM_FIELD)->EnableWindow(FALSE);
	}
	else
	{
		CString strText;
		strText.Format(L"%s :*", g_globaldata.interface_sh3.userinfo.wszFieldText);
		GetDlgItem(IDC_STATIC_STUB_OPTION_CUSTOM_FIELD)->SetWindowText(strText);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkSerialNumber)
	{
		GetDlgItem(IDC_EDIT_STUB_OPTION_SERIAL_NUMBER)->EnableWindow(FALSE);
	}

	SetDlgItemText(IDC_EDIT_STUB_OPTION_USER_NAME, strUserName);
	SetDlgItemText(IDC_EDIT_STUB_OPTION_USER_EMAIL, strUserEmail);
	SetDlgItemText(IDC_EDIT_STUB_OPTION_COMPANY, strCompany);
	SetDlgItemText(IDC_EDIT_STUB_OPTION_CUSTOM_FIELD, strFieldText);
	SetDlgItemText(IDC_EDIT_STUB_OPTION_SERIAL_NUMBER, strSN);
}

void CStubOptionDlg::OnEnChangeEditStubOptionUserName()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStubOptionDlg::OnEnChangeEditStubOptionUserEmail()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStubOptionDlg::OnEnChangeEditStubOptionCompany()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStubOptionDlg::OnEnChangeEditStubOptionCustomField()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStubOptionDlg::OnEnChangeEditStubOptionSerialNumber()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CStubOptionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_STUB_OPTION_USER_NAME, strUserName);
	GetDlgItemText(IDC_EDIT_STUB_OPTION_USER_EMAIL, strUserEmail);
	GetDlgItemText(IDC_EDIT_STUB_OPTION_COMPANY, strCompany);
	GetDlgItemText(IDC_EDIT_STUB_OPTION_CUSTOM_FIELD, strFieldText);
	GetDlgItemText(IDC_EDIT_STUB_OPTION_SERIAL_NUMBER, strSN);
	
	g_mapDirList[L"#INSTALL_DIR"] = g_strInstDir;

	CDialogEx::OnOK();
}

void CStubOptionDlg::OnBnClickedRadioInstallProductForCurrentUser()
{
	// TODO: Add your control notification handler code here
	m_nInstallProductFor = 0;
	g_globaldata.general_sh1.params.nInstallProduct4 = m_nInstallProductFor;
	if (g_globaldata.general_sh1.params.nInstallProduct4 == 0 || g_globaldata.general_sh1.params.nInstallProduct4 == 2)
	{
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#APP_DATA#\\#APPLICATION_NAME#");
	}
	else if (g_globaldata.general_sh1.params.nInstallProduct4 == 1 || g_globaldata.general_sh1.params.nInstallProduct4 == 3)
	{
		if (g_globaldata.install_sh2.filesandfolders.nChk64BitApplication)
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES#\\#APPLICATION_NAME#");
		}
		else
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES_X86#\\#APPLICATION_NAME#");
		}
	}

	g_strInstDir = GetInstallDirectory();

	SetDlgItemText(IDC_EDIT_STUB_OPTION_DESTINATION_FOLDER, g_strInstDir);
}


void CStubOptionDlg::OnBnClickedRadioInstallProductForAllUsers()
{
	// TODO: Add your control notification handler code here
	m_nInstallProductFor = 1;
	g_globaldata.general_sh1.params.nInstallProduct4 = m_nInstallProductFor;
	if (g_globaldata.general_sh1.params.nInstallProduct4 == 0 || g_globaldata.general_sh1.params.nInstallProduct4 == 2)
	{
		wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#APP_DATA#\\#APPLICATION_NAME#");
	}
	else if (g_globaldata.general_sh1.params.nInstallProduct4 == 1 || g_globaldata.general_sh1.params.nInstallProduct4 == 3)
	{
		if (g_globaldata.install_sh2.filesandfolders.nChk64BitApplication)
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES#\\#APPLICATION_NAME#");
		}
		else
		{
			wsprintf(g_globaldata.install_sh2.filesandfolders.wszInstallationDirectory, L"#PROGRAM_FILES_X86#\\#APPLICATION_NAME#");
		}
	}

	g_strInstDir = GetInstallDirectory();

	SetDlgItemText(IDC_EDIT_STUB_OPTION_DESTINATION_FOLDER, g_strInstDir);
}
