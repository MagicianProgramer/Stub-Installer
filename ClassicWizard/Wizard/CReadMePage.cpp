
#include "stdafx.h"
#include "CReadMePage.h"
#include "../../common/common.h"

LRESULT CReadMePage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();
	this->InitializeValues();

	return 1;
}

LRESULT CReadMePage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;
	//this->UninitializeControls();
	return 0;
}


void CReadMePage::InitializeControls(void)
{
	m_pRichReadme = this->GetDlgItem(IDC_EDIT_WIZ97_READ_ME);
}

void CReadMePage::UninitializeControls(void)
{
}

void CReadMePage::InitializeValues(void)
{
	if (g_pReadmeBuff)
	{
		CString strFile = g_globaldata.interface_sh3.main.wszOptionsReadmeFile;
		if(strFile.Right(3) == L"rtf")
		{
			m_pRichReadme.SetWindowTextW((LPCTSTR)g_pReadmeBuff);
		}
		else if (strFile.Right(3) == L"txt")
		{
			CStringA str;
			str.Format("%s", (LPCSTR)g_pReadmeBuff);
			CString strReadMe(str);
			m_pRichReadme.SetWindowText(strReadMe);
		}	
	}
}

bool CReadMePage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CReadMePage::OnSetActive()
{
	this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CReadMePage::OnWizardNext()
{
	return m_pTestWizardInfo->FindNextPage(IDD);
}

int CReadMePage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CReadMePage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}
