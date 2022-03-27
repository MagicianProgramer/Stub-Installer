// GeneralOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "GeneralOutputDlg.h"
#include "afxdialogex.h"

#include "../common/common.h"
#include "../common/other.h"


// CGeneralOutputDlg dialog

IMPLEMENT_DYNAMIC(CGeneralOutputDlg, CPropertyPage)

CGeneralOutputDlg::CGeneralOutputDlg()
	: CPropertyPage(CGeneralOutputDlg::IDD)
{
	m_bLoadImg = FALSE;
}

CGeneralOutputDlg::~CGeneralOutputDlg()
{
}

void CGeneralOutputDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OUTPUT_ICON, m_ctrlPicIcon);
	DDX_Control(pDX, IDC_COMBO_PACKAGE_TYPE, m_ctrlComboPackage);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_FOLDER, m_outputFolderEditCtrl);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_ICON_FILE, m_outputIconFileEditCtrl);
}


BEGIN_MESSAGE_MAP(CGeneralOutputDlg, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_PACKAGE_TYPE, &CGeneralOutputDlg::OnCbnSelchangeComboPackageType)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_DATA_FILE_NAME, &CGeneralOutputDlg::OnEnChangeEditDataFileName)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_FOLDER, &CGeneralOutputDlg::OnEnChangeEditOutputFolder)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_FILE_NAME, &CGeneralOutputDlg::OnEnChangeEditOutputFileName)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT_ICON_FILE, &CGeneralOutputDlg::OnEnChangeEditOutputIconFile)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_FOLDER, &CGeneralOutputDlg::OnBnClickedBtnOutputFolder)
	ON_BN_CLICKED(IDC_BTN_OUTPUT_ICON_FILE, &CGeneralOutputDlg::OnBnClickedBtnOutputIconFile)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGeneralOutputDlg message handlers


void CGeneralOutputDlg::OnCbnSelchangeComboPackageType()
{
	// TODO: Add your control notification handler code here
	g_globaldata.general_sh1.output.nPackageTypeMode = m_ctrlComboPackage.GetCurSel();
	
	CString strOutName;
	if(g_globaldata.general_sh1.output.nPackageTypeMode == 1)
	{
		strOutName.Format(L"%s", INSTALLERPKG_NAME);
	}
	else if (g_globaldata.general_sh1.output.nPackageTypeMode == 2)
	{
		strOutName.Format(L"%s/%s", g_globaldata.general_sh1.info.wszWebSite, INSTALLERPKG_NAME);
	}
	else
	{
		strOutName = L"";
	}

	SetDlgItemText(IDC_EDIT_OUTPUT_DATA_FILE_NAME, strOutName);
	wsprintf(g_globaldata.general_sh1.output.wszDataFileName, strOutName.GetBuffer());
}


void CGeneralOutputDlg::OnEnChangeEditDataFileName()
{
	// TODO:  Add your control notification handler code here
	CString strTemp;
	GetDlgItemTextW(IDC_EDIT_OUTPUT_DATA_FILE_NAME, strTemp);
	wsprintf(g_globaldata.general_sh1.output.wszDataFileName, strTemp.GetBuffer());
}


void CGeneralOutputDlg::OnEnChangeEditOutputFolder()
{
	// TODO:  Add your control notification handler code here
	CString strTemp;
	GetDlgItemTextW(IDC_EDIT_OUTPUT_FOLDER, strTemp);
	wsprintf(g_globaldata.general_sh1.output.wszFolder, strTemp.GetBuffer());
}


void CGeneralOutputDlg::OnEnChangeEditOutputFileName()
{
	// TODO:  Add your control notification handler code here
	CString strTemp;
	GetDlgItemTextW(IDC_EDIT_OUTPUT_FILE_NAME, strTemp);
	wsprintf(g_globaldata.general_sh1.output.wszFileName, strTemp.GetBuffer());
}


void CGeneralOutputDlg::OnEnChangeEditOutputIconFile()
{
	// TODO:  Add your control notification handler code here
	CString strTemp;
	GetDlgItemTextW(IDC_EDIT_OUTPUT_ICON_FILE, strTemp);
	wsprintf(g_globaldata.general_sh1.output.wszIconFile, strTemp.GetBuffer());
}


void CGeneralOutputDlg::OnBnClickedBtnOutputFolder()
{
	// TODO: Add your control notification handler code here
	CFolderPickerDialog dlg(NULL, OFN_READONLY | OFN_OVERWRITEPROMPT , this);
	dlg.m_ofn.lpstrTitle  = _T("Select Output Folder ...");
	if (dlg.DoModal() == IDOK)
	{
		CString strTemp;
		strTemp = dlg.GetFolderPath();
		SetDlgItemText(IDC_EDIT_OUTPUT_FOLDER, strTemp);
		wsprintf(g_globaldata.general_sh1.output.wszFolder, strTemp.GetBuffer());
	}
}


void CGeneralOutputDlg::OnBnClickedBtnOutputIconFile()
{
	// TODO: Add your control notification handler code here
	CString sFilter = L"Icon Files(*.ico)|*.ico|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_READONLY | OFN_ALLOWMULTISELECT, sFilter, this);
	dlg.m_ofn.lpstrTitle = L"Add Image Files ...";
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName;
		strPathName = dlg.GetPathName();

		DrawIcon2PicCtrl(strPathName);
	}
}


BOOL CGeneralOutputDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_outputFolderEditCtrl.EnableFolderBrowseButton();
	m_outputIconFileEditCtrl.EnableFileBrowseButton();

	CString strPathName = g_globaldata.general_sh1.output.wszIconFile;

	if (PathFileExists(strPathName))
	{
		DrawIcon2PicCtrl(strPathName);
	}
	else
	{
		m_img.Create(32, 32, 24);
		Rectangle(m_img.GetDC(), 0, 0, 32, 32);
		m_img.ReleaseDC();

		HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SETUP_DEFAULT);
		DrawIcon(m_img.GetDC(), 0, 0, hIcon);
		m_img.ReleaseDC();

		wsprintf(g_globaldata.general_sh1.output.wszIconFile, L"Default");
		SetDlgItemText(IDC_EDIT_OUTPUT_ICON_FILE, L"Default");
	}
	
	SetDlgItemTextW(IDC_EDIT_OUTPUT_FOLDER, g_globaldata.general_sh1.output.wszFolder);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CGeneralOutputDlg::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	m_ctrlComboPackage.SetCurSel(g_globaldata.general_sh1.output.nPackageTypeMode);
	SetDlgItemTextW(IDC_EDIT_OUTPUT_DATA_FILE_NAME, g_globaldata.general_sh1.output.wszDataFileName);
	SetDlgItemTextW(IDC_EDIT_OUTPUT_FOLDER, g_globaldata.general_sh1.output.wszFolder);
	SetDlgItemTextW(IDC_EDIT_OUTPUT_FILE_NAME, g_globaldata.general_sh1.output.wszFileName);
	SetDlgItemTextW(IDC_EDIT_OUTPUT_ICON_FILE, g_globaldata.general_sh1.output.wszIconFile);

	CString strPathName = g_globaldata.general_sh1.output.wszIconFile;

	if (PathFileExists(strPathName))
	{
		DrawIcon2PicCtrl(strPathName);
	}
	else
	{
		m_img.Destroy();
		m_img.Create(32, 32, 24);
		Rectangle(m_img.GetDC(), 0, 0, 32, 32);
		m_img.ReleaseDC();

		HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SETUP_DEFAULT);
		DrawIcon(m_img.GetDC(), 0, 0, hIcon);
		m_img.ReleaseDC();

		wsprintf(g_globaldata.general_sh1.output.wszIconFile, L"Default");
		SetDlgItemText(IDC_EDIT_OUTPUT_ICON_FILE, L"Default");
	}
	
	return CPropertyPage::OnSetActive();
}


void CGeneralOutputDlg::DrawIcon2PicCtrl(CString strPathName)
{
	if (!strPathName.IsEmpty())
	{
		SetDlgItemText(IDC_EDIT_OUTPUT_ICON_FILE, strPathName);
		wsprintf(g_globaldata.general_sh1.output.wszIconFile, strPathName.GetBuffer());
		m_img.Destroy();
		m_img.Load(g_globaldata.general_sh1.output.wszIconFile);
		Invalidate(TRUE);
	}
}

void CGeneralOutputDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	CClientDC *pDestDC = (CClientDC *)m_ctrlPicIcon.GetDC();

	CRect rectSrc = CRect(0, 0, m_img.GetWidth(), m_img.GetHeight());
	CRect rectDest;
	m_ctrlPicIcon.GetClientRect(&rectDest);
	pDestDC->Rectangle(0, 0, rectDest.Width(), rectDest.Height());
	m_img.Draw(pDestDC->m_hDC, rectDest, rectSrc);
	// Do not call CPropertyPage::OnPaint() for painting messages
}
