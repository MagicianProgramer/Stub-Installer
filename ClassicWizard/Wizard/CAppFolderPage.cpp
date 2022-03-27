
#include "stdafx.h"
#include "CAppFolderPage.h"

#include "../../common/other.h"
#include "../../common/common.h"

#include "FolderDialogStatusText.h"

LRESULT CAppFolderPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();
	this->InitializeValues();

	return 1;
}

LRESULT CAppFolderPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;

	//this->UninitializeControls();
	return 0;
}


void CAppFolderPage::InitializeControls(void)
{

}

void CAppFolderPage::UninitializeControls(void)
{
}

void CAppFolderPage::InitializeValues(void)
{
	// set install product for //
	m_nInstallProductFor = g_globaldata.general_sh1.params.nInstallProduct4 % 2;
	if (m_nInstallProductFor == 0)
		CheckDlgButton(IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER, BST_CHECKED);
	else if (m_nInstallProductFor == 1)
		CheckDlgButton(IDC_RADIO_INSTALL_PRODUCT_FOR_ALL_USERS, BST_CHECKED);

	if (g_globaldata.general_sh1.params.nInstallProduct4 < 2)
	{
		GetDlgItem(IDC_RADIO_INSTALL_PRODUCT_FOR_CURRENT_USER).EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_INSTALL_PRODUCT_FOR_ALL_USERS).EnableWindow(FALSE);
	}


	//set dir//
	SetDlgItemText(IDC_EDIT_DEST_FOLDER, g_strInstDir);

	CString strTxt;
	if (!g_globaldata.interface_sh3.custom.chkComboBox)
	{
		GetDlgItem(IDC_LABEL_CUSTOM_COMBOBOX).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_COMBO_CUSTOM_COMBOBOX).ShowWindow(SW_HIDE);
	}
	else
	{
		strTxt = g_globaldata.interface_sh3.custom.wszComboBoxText;
		GetDlgItem(IDC_LABEL_CUSTOM_COMBOBOX).SetWindowText(strTxt);
		
		CComboBox box = GetDlgItem(IDC_COMBO_CUSTOM_COMBOBOX);
		strTxt = g_globaldata.interface_sh3.custom.wszComboBoxItems;
		int k = strTxt.Find(L"\r\n");
		while(k > 0)
		{
			CString str;
			str = strTxt.Left(k);
			box.InsertString(box.GetCount(), str);
			strTxt = strTxt.Right(strTxt.GetLength() - k - 2);
			k = strTxt.Find(L"\r\n");

			if (k < 0 && !strTxt.IsEmpty())
			{
				box.InsertString(box.GetCount(), strTxt);
			}
		}

		if (box.GetCount() > 0)
			box.SetCurSel(g_globaldata.interface_sh3.custom.nComboBoxDefaultIdx % box.GetCount());
	}

	if (!g_globaldata.interface_sh3.custom.chkSelectComponents)
	{
		GetDlgItem(IDC_LABEL_SEL_COMPONENTS).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LABEL_SEL_COMPONENTS_DESCRIPTION).ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LIST_SELECT_COMPONENTS).ShowWindow(SW_HIDE);
	}
	else
	{
		strTxt = g_globaldata.interface_sh3.custom.wszSelComponentsName;
		GetDlgItem(IDC_LABEL_SEL_COMPONENTS).SetWindowText(strTxt);
		strTxt = g_globaldata.interface_sh3.custom.wszSelComponentsDescription;
		GetDlgItem(IDC_LABEL_SEL_COMPONENTS_DESCRIPTION).SetWindowText(strTxt);
	}

	//display required free space//
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
	GetDlgItem(IDC_STATIC_REQUIRED_FREE_SPACE).SetWindowText(strTxt);

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
		GetDlgItem(IDC_STATIC_AVAILABLE_SPACE).SetWindowText(strTxt);
	}
}

bool CAppFolderPage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CAppFolderPage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CAppFolderPage::OnWizardNext()
{
	if (g_globaldata.interface_sh3.custom.chkComboBox)
	{
		CString strKey;
		GetDlgItem(IDC_LABEL_CUSTOM_COMBOBOX).GetWindowText(strKey);

		CString strValue;
		CComboBox box = GetDlgItem(IDC_COMBO_CUSTOM_COMBOBOX);
		box.GetWindowText(strValue);

		SetRegKeyValue(1, strKey, 0, strValue);
	}

	//not installed but install folder exists//
	if (nAlreadyInstalled == 0 && PathFileExists(g_mapDirList[L"#INSTALL_DIR#"]))
	{
		CString strtxt;
		strtxt.Format(L"The installation folder already exists, would you like to continue installing?");
		if (::MessageBox(NULL, strtxt, L"Warning", MB_YESNO | MB_ICONWARNING) != IDYES)
		{
			return -1;
		}
	}

	return m_pTestWizardInfo->FindNextPage(IDD);
}

int CAppFolderPage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CAppFolderPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}


LRESULT CAppFolderPage::OnBnClickedBtnFolderBrowse(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CFolderDialogStatusText dialog(
		m_hWnd, _T("Root directory:"),
		(/*BIF_EDITBOX |*/ BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT));

	if (IDOK == dialog.DoModal())
	{
		g_strInstDir = dialog.GetFolderPath();
		SetDlgItemText(IDC_EDIT_DEST_FOLDER, g_strInstDir);
	}

	return 0;
}


LRESULT CAppFolderPage::OnEnChangeEditDestFolder(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	GetDlgItemText(IDC_EDIT_DEST_FOLDER, g_strInstDir);
	g_mapDirList[L"#INSTALL_DIR"] = g_strInstDir;
	return 0;
}


LRESULT CAppFolderPage::OnBnClickedRadioInstallProductForCurrentUser(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

	SetDlgItemText(IDC_EDIT_DEST_FOLDER, g_strInstDir);
	g_mapDirList[L"#INSTALL_DIR"] = g_strInstDir;

	return 0;
}


LRESULT CAppFolderPage::OnBnClickedRadioInstallProductForAllUsers(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

	SetDlgItemText(IDC_EDIT_DEST_FOLDER, g_strInstDir);
	g_mapDirList[L"#INSTALL_DIR"] = g_strInstDir;

	return 0;
}
