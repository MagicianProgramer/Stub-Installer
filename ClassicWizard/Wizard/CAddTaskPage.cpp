
#include "stdafx.h"
#include "CAddTaskPage.h"
#include "../../common/common.h"
#include "../../common/other.h"

BOOL bStartMenu = FALSE;
BOOL bDesktop = FALSE;
CString strStartMenuDir = L"";

LRESULT CAddTaskPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;
	m_nSelProgramGroup = 0;
	this->InitializeControls();
	this->InitializeValues();

	return 1;
}

LRESULT CAddTaskPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;

	//this->UninitializeControls();
	return 0;
}


void CAddTaskPage::InitializeControls(void)
{
	//create start menu shortcut//
	
	for (int i = 0; i < g_globaldata.install_sh2.shortcuts.vecDetail.size(); i++)
	{
		CString str(g_globaldata.install_sh2.shortcuts.vecDetail[i].wszLocation);
		if (str.Find(L"#PROGRAM_GROUP#") >= 0)
		{
			bStartMenu = TRUE;
			break;
		}
	}

	if (bStartMenu)
	{
		//insert combo//
		CComboBox combo = GetDlgItem(IDC_COMBO_PROGRAM_GROUP_NAME);
		combo.AddString(g_globaldata.general_sh1.info.wszProductName);
		CString strPath = g_mapDirList[L"#PROGRAM_GROUP#"];

		//find directory//
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(strPath + L"\\*.*", &ffd);
		if (INVALID_HANDLE_VALUE != hFind) 
		{
			do {
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					if (!_tcscmp(ffd.cFileName, TEXT(".")) || 
						!_tcscmp(ffd.cFileName, TEXT(".."))) continue;
					CString strDir(ffd.cFileName);
					strDir = strDir + L"\\";
					strDir.AppendFormat(L"%s", g_globaldata.general_sh1.info.wszProductName);

					combo.InsertString(-1, strDir);
				}

			} while (FindNextFile(hFind, &ffd) != 0);
		}
		combo.SetCurSel(m_nSelProgramGroup);

		CheckDlgButton(IDC_CHECK_CREATE_SHORTCUTS_IN_START_MENU, BST_CHECKED);
	}
	else
	{
		GetDlgItem(IDC_COMBO_PROGRAM_GROUP_NAME).EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CREATE_SHORTCUTS_IN_START_MENU).EnableWindow(FALSE);		
	}


	//create desktop shortcut//
	
	for (int i = 0; i < g_globaldata.install_sh2.shortcuts.vecDetail.size(); i++)
	{
		CString str(g_globaldata.install_sh2.shortcuts.vecDetail[i].wszLocation);
		if (str.Find(L"#DESKTOP#") >= 0)
		{
			bDesktop = TRUE;
			break;
		}
	}

	if (!bDesktop)
	{
		GetDlgItem(IDC_CHECK_CREATE_SHORTCUTS_ON_DESKTOP).EnableWindow(FALSE);	
	}
	else
	{
		CheckDlgButton(IDC_CHECK_CREATE_SHORTCUTS_ON_DESKTOP, BST_CHECKED);
	}
}

void CAddTaskPage::UninitializeControls(void)
{
}

void CAddTaskPage::InitializeValues(void)
{
	CString strTxt;
	if (!g_globaldata.interface_sh3.custom.chkCheckBox)
	{
		GetDlgItem(IDC_CHECK_CHECK_BOX).ShowWindow(SW_HIDE);
	}
	else
	{
		strTxt = g_globaldata.interface_sh3.custom.wszCheckBoxText;
		GetDlgItem(IDC_CHECK_CHECK_BOX).SetWindowText(strTxt);		
	}
}

bool CAddTaskPage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CAddTaskPage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CAddTaskPage::OnWizardNext()
{
	if (g_globaldata.interface_sh3.custom.chkCheckBox)
	{
		CString strKey;
		GetDlgItem(IDC_CHECK_CHECK_BOX).GetWindowText(strKey);

		int nChecked = IsDlgButtonChecked(IDC_CHECK_CHECK_BOX);

		SetRegKeyValue(0, strKey, nChecked, L"");
	}

	//get shortcut info//
	bStartMenu = IsDlgButtonChecked(IDC_CHECK_CREATE_SHORTCUTS_IN_START_MENU);
	bDesktop = IsDlgButtonChecked(IDC_CHECK_CREATE_SHORTCUTS_ON_DESKTOP);
	CComboBox combo = GetDlgItem(IDC_COMBO_PROGRAM_GROUP_NAME);
	/*combo.SelectString(m_nSelProgramGroup, strStartMenuDir);*/
	combo.GetWindowText(strStartMenuDir);
	

	return m_pTestWizardInfo->FindNextPage(IDD);
}

int CAddTaskPage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CAddTaskPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}
