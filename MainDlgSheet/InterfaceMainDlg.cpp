// InterfaceMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfaceMainDlg.h"
#include "afxdialogex.h"
#include "../common/common.h"
#include "TemplateEditDlg.h"

enum INTERFACE_KIND{
	NOINTERFACE = 0,
	CLASSICWIZARD, 
	MODERN,
};
// CInterfaceMainDlg dialog

IMPLEMENT_DYNAMIC(CInterfaceMainDlg, CPropertyPage)

CInterfaceMainDlg::CInterfaceMainDlg()
	: CPropertyPage(CInterfaceMainDlg::IDD)
{
	m_bRefresh = TRUE;
}

CInterfaceMainDlg::~CInterfaceMainDlg()
{
}

void CInterfaceMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_LOGO_BITMAP, m_strLogoBitmap);
	DDX_Text(pDX, IDC_EDIT_OPTIONS_LICFILE, m_strLicFile);
	DDX_Text(pDX, IDC_EDIT_OPTIONS_READMEFILE, m_strReadMeFile);
	DDX_Text(pDX, IDC_EDIT_GRAPHIC_WIZARDBITMAP, m_strWizardBmp);
	DDX_Text(pDX, IDC_EDIT_GRAPHIC_HEADERBITMAP, m_strHdrBmp);
	DDX_Control(pDX, IDC_COMBO_INTERFACE, m_ctrlInterfaceCmb);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_WELCOME, m_ctrlShowWelcome);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_LICENSE_AGREEMENT, m_ctrlShowLicense);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_README, m_ctrlShowReadme);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_USER_INFORMATION, m_ctrlShowUser);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_DESTINATION_FOLDER, m_ctrlShowDest);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_ADDITIONAL_TASKS, m_ctrlShowTask);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_READY_TO_INSTALL, m_ctrlShowReady);
	DDX_Control(pDX, IDC_CHECK_DIALOGS_TO_SHOW_FINISH, m_ctrlShowFinish);
	DDX_Control(pDX, IDC_CHECK_OPTIONS_SHOW_ACCEPT_AGREE, m_ctrlOptAccept);
	DDX_Control(pDX, IDC_CHECK_SHOW_COPYRIGHT_AT_WELCOME_DIALOG, m_ctrlOptCopy);
	DDX_Control(pDX, IDC_CHECK_HIDE_FILE_NAMES_DURING_COPY_PROCESS, m_ctrlOptHide);
	DDX_Control(pDX, IDC_COMBO_LOGO_BITMAP, m_ctrlLogoBitmapCmb);
	DDX_Control(pDX, IDC_BTN_LOGO_BITMAP, m_ctrlLogoBitmapBtn);
	DDX_Control(pDX, IDC_EDIT_OPTIONS_LICFILE, m_InterfaceOptionsLicFileEditCtrl);
	DDX_Control(pDX, IDC_EDIT_OPTIONS_READMEFILE, m_InterfaceOptionsReadmeFileEditCtrl);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_HEADERBITMAP, m_GraphicHeaderBmpEditCtrl);
	DDX_Control(pDX, IDC_EDIT_GRAPHIC_WIZARDBITMAP, m_GraphicWzdBmpEditCtrl);
}


BEGIN_MESSAGE_MAP(CInterfaceMainDlg, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_WELCOME, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowWelcome)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_DESTINATION_FOLDER, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowDestinationFolder)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_LICENSE_AGREEMENT, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowLicenseAgreement)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_ADDITIONAL_TASKS, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowAdditionalTasks)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_README, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowReadme)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_READY_TO_INSTALL, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowReadyToInstall)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_USER_INFORMATION, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowUserInformation)
	ON_BN_CLICKED(IDC_CHECK_DIALOGS_TO_SHOW_FINISH, &CInterfaceMainDlg::OnBnClickedCheckDialogsToShowFinish)
	ON_BN_CLICKED(IDC_CHECK_SHOW_COPYRIGHT_AT_WELCOME_DIALOG, &CInterfaceMainDlg::OnBnClickedCheckShowCopyrightAtWelcomeDialog)
	ON_BN_CLICKED(IDC_CHECK_HIDE_FILE_NAMES_DURING_COPY_PROCESS, &CInterfaceMainDlg::OnBnClickedCheckHideFileNamesDuringCopyProcess)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, &CInterfaceMainDlg::OnCbnSelchangeComboInterface)
	ON_BN_CLICKED(IDC_BTN_LOGO_BITMAP, &CInterfaceMainDlg::OnBnClickedBtnLogoBitmap)
	ON_CBN_SELCHANGE(IDC_COMBO_LOGO_BITMAP, &CInterfaceMainDlg::OnCbnSelchangeComboLogoBitmap)
	ON_CBN_EDITCHANGE(IDC_COMBO_LOGO_BITMAP, &CInterfaceMainDlg::OnCbnEditchangeComboLogoBitmap)
	ON_BN_CLICKED(IDC_BTN_OPTIONS_LICFILE_OPEN, &CInterfaceMainDlg::OnBnClickedBtnOptionsLicfileOpen)
	ON_BN_CLICKED(IDC_BTN_OPTIONS_README_OPEN, &CInterfaceMainDlg::OnBnClickedBtnOptionsReadmeOpen)
	ON_EN_CHANGE(IDC_EDIT_OPTIONS_LICFILE, &CInterfaceMainDlg::OnEnChangeEditOptionsLicfile)
	ON_EN_CHANGE(IDC_EDIT_OPTIONS_READMEFILE, &CInterfaceMainDlg::OnEnChangeEditOptionsReadmefile)
	ON_BN_CLICKED(IDC_CHECK_OPTIONS_SHOW_ACCEPT_AGREE, &CInterfaceMainDlg::OnBnClickedCheckOptionsShowAcceptAgree)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_WIZARDBITMAP, &CInterfaceMainDlg::OnEnChangeEditGraphicWizardbitmap)
	ON_EN_CHANGE(IDC_EDIT_GRAPHIC_HEADERBITMAP, &CInterfaceMainDlg::OnEnChangeEditGraphicHeaderbitmap)
	ON_BN_CLICKED(IDC_BTN_WIZARDBITMAP_OPEN, &CInterfaceMainDlg::OnBnClickedBtnWizardbitmapOpen)
	ON_BN_CLICKED(IDC_BTN_GRAPHIC_HEADERBITMAP_OPEN, &CInterfaceMainDlg::OnBnClickedBtnGraphicHeaderbitmapOpen)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_VIEW_SAMPLE, &CInterfaceMainDlg::OnNMClickSyslinkViewSample)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_USE_DEFAULT_BITMAPS, &CInterfaceMainDlg::OnNMClickSyslinkUseDefaultBitmaps)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_TEMPLATE_EDITOR, &CInterfaceMainDlg::OnNMClickSyslinkTemplateEditor)
END_MESSAGE_MAP()


// CInterfaceMainDlg message handlers


BOOL CInterfaceMainDlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_InterfaceOptionsLicFileEditCtrl.EnableFileBrowseButton();
	m_InterfaceOptionsReadmeFileEditCtrl.EnableFileBrowseButton();

	m_GraphicHeaderBmpEditCtrl.EnableFileBrowseButton();
	m_GraphicWzdBmpEditCtrl.EnableFileBrowseButton();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowWelcome()
{
	m_bWelcom = m_ctrlShowWelcome.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome = m_bWelcom;
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowDestinationFolder()
{
	m_bDestinationFolder = m_ctrlShowDest.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder = m_bDestinationFolder;
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowLicenseAgreement()
{
	m_bLicenseAgreement = m_ctrlShowLicense.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement = m_bLicenseAgreement;
	GetDlgItem(IDC_EDIT_OPTIONS_LICFILE)->EnableWindow(m_bLicenseAgreement);
	m_ctrlOptAccept.EnableWindow(m_bLicenseAgreement);
	m_ctrlOptAccept.SetCheck(m_bLicenseAgreement);
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowAdditionalTasks()
{
	m_bAdditionalTasks = m_ctrlShowTask.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks = m_bAdditionalTasks;
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowReadme()
{
	m_bReadme = m_ctrlShowReadme.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowReadme = m_bReadme;

	GetDlgItem(IDC_EDIT_OPTIONS_READMEFILE)->EnableWindow(m_bReadme);
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowReadyToInstall()
{
	m_bReadyInstall = m_ctrlShowReady.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install = m_bReadyInstall;
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowUserInformation()
{
	m_bUserInfo = m_ctrlShowUser.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation = m_bUserInfo;
	
	CWnd *pParentWnd = GetParent();
	if (pParentWnd)			pParentWnd->PostMessage(UM_INTERFACE_USERINFO_REFRESH);
}


void CInterfaceMainDlg::OnBnClickedCheckDialogsToShowFinish()
{
	m_bFinish = m_ctrlShowFinish.GetCheck();
	g_globaldata.interface_sh3.main.chkDialogs2ShowFinish = m_bFinish;
}


void CInterfaceMainDlg::OnBnClickedCheckShowCopyrightAtWelcomeDialog()
{
	m_bShowCopyright = m_ctrlOptCopy.GetCheck();
	g_globaldata.interface_sh3.main.chkOptionsShowCopyright = m_bShowCopyright;
}


void CInterfaceMainDlg::OnBnClickedCheckHideFileNamesDuringCopyProcess()
{
	m_bHidefile = m_ctrlOptHide.GetCheck();
	g_globaldata.interface_sh3.main.chkOptionsHideFileNames = m_bHidefile;
}


void CInterfaceMainDlg::OnCbnSelchangeComboInterface()
{
	INT k = m_ctrlInterfaceCmb.GetCurSel();
	if (m_nInterface != k)
	{
		m_nInterface = k;
		g_globaldata.interface_sh3.main.nGraphicInterfaceMode = m_nInterface;
		SetComponentFromInterfaceSel(m_nInterface);
	}
}


void CInterfaceMainDlg::OnBnClickedBtnLogoBitmap()
{	
	CString filetype = L"All Files(*.*)|*.*|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		//load//
		m_strLogoBitmap = dlg.GetPathName();
		wcscpy(g_globaldata.interface_sh3.main.wszGraphicLogoBitmap, m_strLogoBitmap);
		m_ctrlLogoBitmapCmb.SetWindowTextW(m_strLogoBitmap);
	}
}


void CInterfaceMainDlg::OnCbnSelchangeComboLogoBitmap()
{
	m_ctrlLogoBitmapCmb.GetWindowTextW(m_strLogoBitmap);
	wcscpy(g_globaldata.interface_sh3.main.wszGraphicLogoBitmap, m_strLogoBitmap);
}


void CInterfaceMainDlg::OnCbnEditchangeComboLogoBitmap()
{
	m_ctrlLogoBitmapCmb.GetWindowTextW(m_strLogoBitmap);
	wcscpy(g_globaldata.interface_sh3.main.wszGraphicLogoBitmap, m_strLogoBitmap);
}


BOOL CInterfaceMainDlg::OnSetActive()
{
	if (m_bRefresh)		
	{
		m_bWelcom = g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome;
		m_bLicenseAgreement = g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement;
		m_bReadme = g_globaldata.interface_sh3.main.chkDialogs2ShowReadme;
		m_bUserInfo = g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation;
		m_bDestinationFolder = g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder;
		m_bAdditionalTasks = g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks;
		m_bReadyInstall = g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install;
		m_bFinish = g_globaldata.interface_sh3.main.chkDialogs2ShowFinish;
		m_bShowCopyright = g_globaldata.interface_sh3.main.chkOptionsShowCopyright;
		m_bHidefile = g_globaldata.interface_sh3.main.chkOptionsHideFileNames;
		m_bRunAsAdmin = g_globaldata.interface_sh3.main.chkOptionsRunAsAdmin;
		m_nInterface = g_globaldata.interface_sh3.main.nGraphicInterfaceMode;
		m_strLogoBitmap = g_globaldata.interface_sh3.main.wszGraphicLogoBitmap;
		m_nChkOptionsShowAgree = g_globaldata.interface_sh3.main.chkOptionsShowAgreement;
		m_strLicFile = g_globaldata.interface_sh3.main.wszOptionsLicenseFile;
		m_strReadMeFile = g_globaldata.interface_sh3.main.wszOptionsReadmeFile;
		m_strWizardBmp = g_globaldata.interface_sh3.main.wszGraphicWizardBmp;
		m_strHdrBmp = g_globaldata.interface_sh3.main.wszGraphicHdrBmp;

		m_nInterface = g_globaldata.interface_sh3.main.nGraphicInterfaceMode;
		m_ctrlInterfaceCmb.SetCurSel(m_nInterface);

		SetComponentFromInterfaceSel(m_nInterface);

		m_bRefresh = FALSE;
	}
		
	
	return CPropertyPage::OnSetActive();
}


void CInterfaceMainDlg::SetComponentFromInterfaceSel(INT nSel)
{
	switch (nSel)
	{
	case NOINTERFACE://no interface
		{
			//show and hide//
			m_ctrlLogoBitmapCmb.ShowWindow(SW_HIDE);
			m_ctrlLogoBitmapBtn.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SYSLINK_TEMPLATE_EDITOR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SYSLINK_USE_DEFAULT_BITMAPS)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_EDIT_GRAPHIC_WIZARDBITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_WIZARD_BITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_GRAPHIC_HEADERBITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_HEADER_BITMAP)->ShowWindow(SW_HIDE);
			
			GetDlgItem(IDC_SYSLINK_VIEW_SAMPLE)->ShowWindow(SW_HIDE);

			//check button//
			m_ctrlShowWelcome.SetCheck(BST_UNCHECKED);
			m_ctrlShowLicense.SetCheck(BST_UNCHECKED);
			m_ctrlShowReadme.SetCheck(BST_UNCHECKED);
			m_ctrlShowUser.SetCheck(BST_UNCHECKED);
			m_ctrlShowDest.SetCheck(BST_UNCHECKED);
			m_ctrlShowTask.SetCheck(BST_UNCHECKED);
			m_ctrlShowReady.SetCheck(BST_UNCHECKED);
			m_ctrlShowFinish.SetCheck(BST_UNCHECKED);
			
			m_ctrlOptAccept.SetCheck(BST_UNCHECKED);
			m_ctrlOptCopy.SetCheck(BST_UNCHECKED);
			m_ctrlOptHide.SetCheck(BST_UNCHECKED);

			g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowReadme = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install = FALSE;
			g_globaldata.interface_sh3.main.chkDialogs2ShowFinish = FALSE;
			g_globaldata.interface_sh3.main.chkOptionsShowCopyright = FALSE;
			g_globaldata.interface_sh3.main.chkOptionsHideFileNames = FALSE;
			g_globaldata.interface_sh3.main.chkOptionsRunAsAdmin = FALSE;
			g_globaldata.interface_sh3.main.chkOptionsShowAgreement = FALSE;
						
			//disable//
			m_ctrlShowWelcome.EnableWindow(FALSE);
			m_ctrlShowLicense.EnableWindow(FALSE);
			m_ctrlShowReadme.EnableWindow(FALSE);
			m_ctrlShowUser.EnableWindow(FALSE);
			m_ctrlShowDest.EnableWindow(FALSE);
			m_ctrlShowTask.EnableWindow(FALSE);
			m_ctrlShowReady.EnableWindow(FALSE);
			m_ctrlShowFinish.EnableWindow(FALSE);

			m_ctrlOptAccept.EnableWindow(FALSE);
			m_ctrlOptCopy.EnableWindow(FALSE);
			m_ctrlOptHide.EnableWindow(FALSE);
			
			GetDlgItem(IDC_EDIT_OPTIONS_LICFILE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OPTIONS_READMEFILE)->EnableWindow(FALSE);
		}
		break;
	case CLASSICWIZARD://classic wizard
		{
			//show and hide//
			m_ctrlLogoBitmapCmb.ShowWindow(SW_HIDE);
			m_ctrlLogoBitmapBtn.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_LOGO_BITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SYSLINK_TEMPLATE_EDITOR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SYSLINK_USE_DEFAULT_BITMAPS)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_GRAPHIC_WIZARDBITMAP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_WIZARD_BITMAP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_EDIT_GRAPHIC_HEADERBITMAP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_HEADER_BITMAP)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_SYSLINK_VIEW_SAMPLE)->ShowWindow(SW_SHOW);

			//check button//
			if (m_bRefresh)
			{
				m_ctrlShowWelcome.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome);
				m_ctrlShowLicense.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement);
				m_ctrlShowReadme.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowReadme);
				m_ctrlShowUser.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation);
				m_ctrlShowDest.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder);
				m_ctrlShowTask.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks);
				m_ctrlShowReady.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install);
				m_ctrlShowFinish.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowFinish);

				m_ctrlOptAccept.SetCheck(g_globaldata.interface_sh3.main.chkOptionsShowAgreement);
				m_ctrlOptCopy.SetCheck(g_globaldata.interface_sh3.main.chkOptionsShowCopyright);
				m_ctrlOptHide.SetCheck(g_globaldata.interface_sh3.main.chkOptionsHideFileNames);
			}
			else
			{
				m_ctrlShowWelcome.SetCheck(BST_CHECKED);
				m_ctrlShowLicense.SetCheck(BST_UNCHECKED);
				m_ctrlShowReadme.SetCheck(BST_UNCHECKED);
				m_ctrlShowUser.SetCheck(BST_UNCHECKED);
				m_ctrlShowDest.SetCheck(BST_CHECKED);
				m_ctrlShowTask.SetCheck(BST_CHECKED);
				m_ctrlShowReady.SetCheck(BST_CHECKED);
				m_ctrlShowFinish.SetCheck(BST_CHECKED);

				m_ctrlOptAccept.SetCheck(BST_UNCHECKED);
				m_ctrlOptCopy.SetCheck(BST_CHECKED);
				m_ctrlOptHide.SetCheck(BST_UNCHECKED);
				
				g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowReadme = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowFinish = TRUE;
				g_globaldata.interface_sh3.main.chkOptionsShowCopyright = TRUE;
				g_globaldata.interface_sh3.main.chkOptionsHideFileNames = FALSE;
				g_globaldata.interface_sh3.main.chkOptionsRunAsAdmin = FALSE;
				g_globaldata.interface_sh3.main.chkOptionsShowAgreement = FALSE;
			}

			//disable//
			m_ctrlShowWelcome.EnableWindow(TRUE);
			m_ctrlShowLicense.EnableWindow(TRUE);
			m_ctrlShowReadme.EnableWindow(TRUE);
			m_ctrlShowUser.EnableWindow(TRUE);
			m_ctrlShowDest.EnableWindow(TRUE);
			m_ctrlShowTask.EnableWindow(TRUE);
			m_ctrlShowReady.EnableWindow(TRUE);
			m_ctrlShowFinish.EnableWindow(TRUE);

			m_ctrlOptAccept.EnableWindow(FALSE);
			m_ctrlOptCopy.EnableWindow(TRUE);
			m_ctrlOptHide.EnableWindow(TRUE);
			
			GetDlgItem(IDC_EDIT_OPTIONS_LICFILE)->EnableWindow(g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement);
			GetDlgItem(IDC_EDIT_OPTIONS_READMEFILE)->EnableWindow(g_globaldata.interface_sh3.main.chkDialogs2ShowReadme);
		}
		break;
	case MODERN://modern
		{
			//show and hide//
			m_ctrlLogoBitmapCmb.ShowWindow(SW_SHOW);
			m_ctrlLogoBitmapBtn.ShowWindow(SW_SHOW);
			GetDlgItem(IDC_STATIC_LOGO_BITMAP)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_SYSLINK_TEMPLATE_EDITOR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_SYSLINK_USE_DEFAULT_BITMAPS)->ShowWindow(SW_SHOW);

			GetDlgItem(IDC_EDIT_GRAPHIC_WIZARDBITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_WIZARD_BITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_EDIT_GRAPHIC_HEADERBITMAP)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_HEADER_BITMAP)->ShowWindow(SW_HIDE);

			GetDlgItem(IDC_SYSLINK_VIEW_SAMPLE)->ShowWindow(SW_SHOW);

			//check button//
			if (m_bRefresh)
			{
				m_ctrlShowWelcome.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome);
				m_ctrlShowLicense.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement);
				m_ctrlShowReadme.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowReadme);
				m_ctrlShowUser.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation);
				m_ctrlShowDest.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder);
				m_ctrlShowTask.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks);
				m_ctrlShowReady.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install);
				m_ctrlShowFinish.SetCheck(g_globaldata.interface_sh3.main.chkDialogs2ShowFinish);

				m_ctrlOptAccept.SetCheck(g_globaldata.interface_sh3.main.chkOptionsShowAgreement);
				m_ctrlOptCopy.SetCheck(g_globaldata.interface_sh3.main.chkOptionsShowCopyright);
				m_ctrlOptHide.SetCheck(g_globaldata.interface_sh3.main.chkOptionsHideFileNames);
			}
			else
			{
				m_ctrlShowWelcome.SetCheck(BST_CHECKED);
				m_ctrlShowLicense.SetCheck(BST_UNCHECKED);
				m_ctrlShowReadme.SetCheck(BST_UNCHECKED);
				m_ctrlShowUser.SetCheck(BST_UNCHECKED);
				m_ctrlShowDest.SetCheck(BST_CHECKED);
				m_ctrlShowTask.SetCheck(BST_CHECKED);
				m_ctrlShowReady.SetCheck(BST_UNCHECKED);
				m_ctrlShowFinish.SetCheck(BST_CHECKED);

				m_ctrlOptAccept.SetCheck(BST_UNCHECKED);
				m_ctrlOptCopy.SetCheck(BST_CHECKED);
				m_ctrlOptHide.SetCheck(BST_CHECKED);
				
				g_globaldata.interface_sh3.main.chkDialogs2ShowWelcome = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowReadme = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowUserInformation = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowDestinationFolder = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowAdditionalTasks = TRUE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowReady2Install = FALSE;
				g_globaldata.interface_sh3.main.chkDialogs2ShowFinish = TRUE;
				g_globaldata.interface_sh3.main.chkOptionsShowCopyright = TRUE;
				g_globaldata.interface_sh3.main.chkOptionsHideFileNames = TRUE;
				g_globaldata.interface_sh3.main.chkOptionsRunAsAdmin = FALSE;
				g_globaldata.interface_sh3.main.chkOptionsShowAgreement = FALSE;
			}
			

			//disable//
			m_ctrlShowWelcome.EnableWindow(FALSE);
			m_ctrlShowLicense.EnableWindow(TRUE);
			m_ctrlShowReadme.EnableWindow(FALSE);
			m_ctrlShowUser.EnableWindow(TRUE);
			m_ctrlShowDest.EnableWindow(FALSE);
			m_ctrlShowTask.EnableWindow(FALSE);
			m_ctrlShowReady.EnableWindow(FALSE);
			m_ctrlShowFinish.EnableWindow(FALSE);

			m_ctrlOptAccept.EnableWindow(FALSE);
			m_ctrlOptCopy.EnableWindow(TRUE);
			m_ctrlOptHide.EnableWindow(FALSE);
			
			GetDlgItem(IDC_EDIT_OPTIONS_LICFILE)->EnableWindow(g_globaldata.interface_sh3.main.chkDialogs2ShowLicenseAgreement);
			GetDlgItem(IDC_EDIT_OPTIONS_READMEFILE)->EnableWindow(g_globaldata.interface_sh3.main.chkDialogs2ShowReadme);
		}
		break;
	}
}

/////////////////////////////////////////////////

void CInterfaceMainDlg::OnBnClickedBtnOptionsLicfileOpen()
{
	// TODO: Add your control notification handler code here
	CString filetype = L"Text Files(*.txt)|*.txt|Rich Text Files(*.rtf)|*.rtf|All Files(*.*)|*.*|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		//load//
		m_strLicFile = dlg.GetPathName();
		wsprintf(g_globaldata.interface_sh3.main.wszOptionsLicenseFile, m_strLicFile.GetBuffer());
		SetDlgItemText(IDC_EDIT_OPTIONS_LICFILE, m_strLicFile);
	}
}


void CInterfaceMainDlg::OnBnClickedBtnOptionsReadmeOpen()
{
	// TODO: Add your control notification handler code here
	CString filetype = L"Text Files(*.txt)|*.txt|Rich Text Files(*.rtf)|*.rtf|All Files(*.*)|*.*|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		//load//
		m_strReadMeFile = dlg.GetPathName();
		wsprintf(g_globaldata.interface_sh3.main.wszOptionsReadmeFile, m_strReadMeFile.GetBuffer());
		SetDlgItemText(IDC_EDIT_OPTIONS_READMEFILE, m_strReadMeFile);
	}
}


void CInterfaceMainDlg::OnEnChangeEditOptionsLicfile()
{
	GetDlgItemText(IDC_EDIT_OPTIONS_LICFILE, m_strLicFile);
	wsprintf(g_globaldata.interface_sh3.main.wszOptionsLicenseFile, m_strLicFile.GetBuffer());
}


void CInterfaceMainDlg::OnEnChangeEditOptionsReadmefile()
{
	GetDlgItemText(IDC_EDIT_OPTIONS_READMEFILE, m_strReadMeFile);
	wsprintf(g_globaldata.interface_sh3.main.wszOptionsReadmeFile, m_strReadMeFile.GetBuffer());
}


void CInterfaceMainDlg::OnBnClickedCheckOptionsShowAcceptAgree()
{
	m_nChkOptionsShowAgree = IsDlgButtonChecked(IDC_CHECK_OPTIONS_SHOW_ACCEPT_AGREE);
	g_globaldata.interface_sh3.main.chkOptionsShowAgreement = m_nChkOptionsShowAgree;
}


void CInterfaceMainDlg::OnEnChangeEditGraphicWizardbitmap()
{
	GetDlgItemText(IDC_EDIT_GRAPHIC_WIZARDBITMAP, m_strWizardBmp);
	wsprintf(g_globaldata.interface_sh3.main.wszGraphicWizardBmp, m_strWizardBmp.GetBuffer());
}


void CInterfaceMainDlg::OnEnChangeEditGraphicHeaderbitmap()
{
	GetDlgItemText(IDC_EDIT_GRAPHIC_HEADERBITMAP, m_strHdrBmp);
	wsprintf(g_globaldata.interface_sh3.main.wszGraphicHdrBmp, m_strHdrBmp.GetBuffer());
}


void CInterfaceMainDlg::OnBnClickedBtnWizardbitmapOpen()
{
	CString filetype = L"Bitmap Files(*.bmp)|*.bmp|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		//load//
		m_strWizardBmp = dlg.GetPathName();
		wsprintf(g_globaldata.interface_sh3.main.wszGraphicWizardBmp, m_strWizardBmp.GetBuffer());
		SetDlgItemText(IDC_EDIT_GRAPHIC_WIZARDBITMAP, m_strWizardBmp);
	}
}


void CInterfaceMainDlg::OnBnClickedBtnGraphicHeaderbitmapOpen()
{
	CString filetype = L"Bitmap Files(*.bmp)|*.bmp|";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, filetype);
	if (dlg.DoModal() == IDOK)
	{
		//load//
		m_strHdrBmp = dlg.GetPathName();
		wsprintf(g_globaldata.interface_sh3.main.wszGraphicHdrBmp, m_strHdrBmp.GetBuffer());
		SetDlgItemText(IDC_EDIT_GRAPHIC_HEADERBITMAP, m_strHdrBmp);
	}
}


void CInterfaceMainDlg::OnNMClickSyslinkViewSample(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CString strPath = g_wszModulePathName;
	strPath = strPath.Left(strPath.ReverseFind(L'\\'));
	CString strSampleFilePath;
	if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)
	{
		/*strSampleFilePath.Format(L"%s\\%s\\NoInterface.bmp", strPath, STR_BITMAPS);*/
	}
	else if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 1)
	{
		strSampleFilePath.Format(L"%s\\%s\\Wizard.bmp", strPath, STR_BITMAPS);
	}
	else if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 2)
	{
		strSampleFilePath.Format(L"%s\\%s\\Modern.bmp", strPath, STR_BITMAPS);
	}

	if (PathFileExists(strSampleFilePath))
	{
		ShellExecute(NULL, L"open", strSampleFilePath, NULL, NULL, SW_NORMAL);
	}
	else
	{
		CString strMsg;
		strMsg.Format(L"There is no sample image.\n'%s'", strSampleFilePath);
		MessageBox(strMsg, L"Error", MB_OK | MB_ICONERROR);
	}
	
}


void CInterfaceMainDlg::OnNMClickSyslinkUseDefaultBitmaps(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;	
	CString str1, str2;
	if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 0)
	{
		//str1.Format(L"%s\\%s\\NoInterface.bmp", g_wszModulePath, STR_BITMAPS);
	}
	else if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 1)
	{
		str1.Format(L"#INSTALLER_DEFAULT_DIR#\\%s\\Wizard\\01.bmp", STR_BITMAPS);
		SetDlgItemText(IDC_EDIT_GRAPHIC_WIZARDBITMAP, str1);
		str2.Format(L"#INSTALLER_DEFAULT_DIR#\\%s\\Header\\01.bmp", STR_BITMAPS);
		SetDlgItemText(IDC_EDIT_GRAPHIC_HEADERBITMAP, str2);

		wcscpy(g_globaldata.interface_sh3.main.wszGraphicWizardBmp, str1);
		wcscpy(g_globaldata.interface_sh3.main.wszGraphicHdrBmp, str2);
	}
	else if (g_globaldata.interface_sh3.main.nGraphicInterfaceMode == 2)
	{
		str1.Format(L"#INSTALLER_DEFAULT_DIR#\\%s\\Logo\\01.bmp", STR_BITMAPS);
		SetDlgItemText(IDC_COMBO_LOGO_BITMAP, str1);

		wcscpy(g_globaldata.interface_sh3.main.wszGraphicLogoBitmap, str1);
	}
}


void CInterfaceMainDlg::OnNMClickSyslinkTemplateEditor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CTemplateEditDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}

	*pResult = 0;
}
