
#include "stdafx.h"
#include "TestWizardCompletionPage.h"
#include "../../common/common.h"

LRESULT CTestWizardCompletionPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	this->InitializeControls();
	this->InitializeValues();
	
	return 1;
}

void CTestWizardCompletionPage::InitializeFont(void)
{

}

void CTestWizardCompletionPage::InitializeControls(void)
{
	CFontHandle fontExteriorPageTitleFont(baseClass::GetExteriorPageTitleFont());

	CWindow title = this->GetDlgItem(IDC_WIZ97_EXTERIOR_TITLE);
	title.SetFont(fontExteriorPageTitleFont);

	//m_editSummary = this->GetDlgItem(IDC_WIZ97_SUMMARY);
	CString strTxt;

	if (g_nErr != SINST_NO_ERROR)
	{
		strTxt.Format(L"%s Installation Uncompleted!", g_globaldata.general_sh1.info.wszProductName);
		title.SetWindowText(strTxt);

		strTxt.Format(L"%s %s hasn't been installed on your computer.",
			g_globaldata.general_sh1.info.wszProductName,
			g_globaldata.general_sh1.info.wszProductVersion);
	}
	else
	{
		if (nAlreadyInstalled)
		{			
			if (bRepair)
			{
				strTxt.Format(L"%s Repaired!", g_globaldata.general_sh1.info.wszProductName);
				title.SetWindowText(strTxt);
				strTxt.Format(L"%s has been repaired.",	g_globaldata.general_sh1.info.wszProductName);
			}
			else
			{
				strTxt.Format(L"%s Removed!", g_globaldata.general_sh1.info.wszProductName);
				title.SetWindowText(strTxt);
				strTxt.Format(L"%s has been removed.", g_globaldata.general_sh1.info.wszProductName);
			}
		}
		else
		{
			strTxt.Format(L"%s Installation Completed!", g_globaldata.general_sh1.info.wszProductName);
			title.SetWindowText(strTxt);
			strTxt.Format(L"%s %s has been installed on your computer.", g_globaldata.general_sh1.info.wszProductName,	g_globaldata.general_sh1.info.wszProductVersion);
		}
	}
	

	//finish tab//
	GetDlgItem(IDC_CHECK_FINISH_LAUNCH_APPLICATION).ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_FINISH_CUSTOM_ACTION).ShowWindow(SW_HIDE);

	if (!nAlreadyInstalled && g_globaldata.interface_sh3.finish.chkLaunchApp && g_nErr != SINST_DOWNLOAD_PKG_FAILED)
	{
		GetDlgItem(IDC_CHECK_FINISH_LAUNCH_APPLICATION).ShowWindow(SW_SHOW);
		CheckDlgButton(IDC_CHECK_FINISH_LAUNCH_APPLICATION, g_globaldata.interface_sh3.finish.chkLaunchAppCheckedByDefault);
	}

	if (!nAlreadyInstalled && g_globaldata.interface_sh3.finish.chkCustomAction && g_nErr != SINST_DOWNLOAD_PKG_FAILED)
	{
		GetDlgItem(IDC_CHECK_FINISH_CUSTOM_ACTION).ShowWindow(SW_SHOW);
		CString str(g_globaldata.interface_sh3.finish.wszCustomActionName);
		GetDlgItem(IDC_CHECK_FINISH_CUSTOM_ACTION).SetWindowText(str);
		CheckDlgButton(IDC_CHECK_FINISH_CUSTOM_ACTION, g_globaldata.interface_sh3.finish.chkCustomActionCheckedByDefault);
	}
	
	GetDlgItem(IDC_WIZ97_EXTERIOR_DESC_COMPLETE).SetWindowText(strTxt);

	this->InitializeFont();
}

void CTestWizardCompletionPage::InitializeValues(void)
{
	
}

void CTestWizardCompletionPage::UpdateSummary(void)
{
	CString path = m_pTestWizardInfo->GetPath();
	bool recurse = m_pTestWizardInfo->GetRecurse();
	CString filter = m_pTestWizardInfo->GetFilter();
	TestWizardOutputType outputType = m_pTestWizardInfo->GetOutputType();

	CString text;
	/*text.Format(
		_T("Test Wizard: \r\n")
		_T("· Find files in the directory:\r\n")
		_T("\t%s\r\n")
		_T("· %s\r\n")
		_T("· Find files matching the filter '%s'\r\n"),
		path,
		recurse ? _T("Also search sub-directories") : _T("Only search that directory"),
		filter);*/
	text = _T("Setup is completed!");
	//m_editSummary.SetWindowText(text);

	CString outputDescription;
	switch(outputType)
	{
	case eOutput_SendEMail:
		outputDescription = 
			_T("· Send the file list in an e-mail\r\n")
			_T("  (using the default mail client)\r\n");
		break;
	case eOutput_SaveToFile:
		{
			CString outputFileName = m_pTestWizardInfo->GetOutputFileName();
			TestWizardOutputFileEncoding outputFileEncoding = m_pTestWizardInfo->GetOutputFileEncoding();

			outputDescription.Format(
				_T("· Save the file list to the file:\r\n")
				_T("\t%s\r\n"),
				outputFileName);
			switch(outputFileEncoding)
			{
			case eEncoding_ASCII:
				outputDescription += _T("  with ASCII encoding\r\n");
				break;
			case eEncoding_UCS2:
				outputDescription += _T("  with Unicode (UCS-2) encoding\r\n");
				break;
			case eEncoding_UTF8:
				outputDescription += _T("  with Unicode (UTF-8) encoding\r\n");
				break;
			}
		}
		break;
	case eOutput_Clipboard:
	default:
		outputDescription = _T("· Copy the file list to the clipboard\r\n");
		break;
	}
	//m_editSummary.AppendText(outputDescription);
}

int CTestWizardCompletionPage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_FINISH);
	::ShowWindow(::GetDlgItem(GetParent().m_hWnd, 0x02), SW_HIDE); // cancel hide //
	::ShowWindow(::GetDlgItem(GetParent().m_hWnd, 0x09), SW_HIDE); // help hide //
	::ShowWindow(::GetDlgItem(GetParent().m_hWnd, 0x3023), SW_HIDE); // back hide //
	
	// Don't remember any previous updates to the summary,
	//  and just regenerate the whole summary
	//this->UpdateSummary();

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	if (!g_globaldata.interface_sh3.main.chkDialogs2ShowFinish)
	{
		::PostMessage(GetParent().m_hWnd, WM_COMMAND, 0x00003025, (LPARAM)(::GetParent(GetParent().m_hWnd)));
		/*if(g_pLog)	g_pLog->Write(L"auto finish.");*/
	}
	
	return 0;
}

int CTestWizardCompletionPage::OnWizardBack()
{
	// 0  = goto previous page
	// -1 = prevent page change
	// >0 = jump to page by dlg ID
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

INT_PTR CTestWizardCompletionPage::OnWizardFinish()
{
	// We could either do the work here, or in the place that
	// called DoModal on our Sheet (which for this example is CTestWizard).
	// The advantage of doing the work here is that we can prevent
	// the finish, and tell the user to go back and correct something.
	// The advantage of doing it in the caller of DoModal is
	// that the wizard isn't visible while the work is being done.

	// For this example, we'll do the work here (or rather call back into
	// the info class to do the work), and prevent finish if something fails.

	CWaitCursor waitCursor;

	bool success = m_pTestWizardInfo->FinishWizard(m_hWnd);

	g_rfile.Close();

	close_uninstlst();
	close_log();

	g_globaldata.interface_sh3.finish.chkLaunchApp = IsDlgButtonChecked(IDC_CHECK_FINISH_LAUNCH_APPLICATION);
	g_globaldata.interface_sh3.finish.chkCustomAction = IsDlgButtonChecked(IDC_CHECK_FINISH_CUSTOM_ACTION);

	FinishTab();

	// FALSE = allow finish
	// TRUE = prevent finish
	// HWND = prevent finish and set focus to HWND (CommCtrl 5.80 only)
	return success ? FALSE : TRUE;
}

void CTestWizardCompletionPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}
