
#include "stdafx.h"
#include "CLicAgreePage.h"
#include "../../common/common.h"
#include "../../common/SimpleHtml.h"

LRESULT CLicAgreePage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();
	this->InitializeValues();

	return 1;
}

LRESULT CLicAgreePage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;
	//this->UninitializeControls();
	return 0;
}


void CLicAgreePage::InitializeControls(void)
{
	m_buttonAccept = this->GetDlgItem(IDC_CHECK_WIZ97_LIC_AGREEMENT);
	m_richLic = this->GetDlgItem(IDC_EDIT_WIZ97_LICAGREE);
}

void CLicAgreePage::UninitializeControls(void)
{
}


void CLicAgreePage::InitializeValues(void)
{
	m_buttonAccept.SetCheck(BST_UNCHECKED);

	if (g_pLicBuff)
	{
		CString strFile = g_globaldata.interface_sh3.main.wszOptionsLicenseFile;
		if(strFile.Right(4) == L".rtf")
		{
			m_richLic.SetWindowTextW((LPCTSTR)g_pLicBuff);
		}
		else if (strFile.Right(4) == L".txt")
		{
			CStringA str;
			str.Format("%s", (LPCSTR)g_pLicBuff);
			CString strLicense(str);
			m_richLic.SetWindowText(strLicense);
		}
		else if (strFile.Right(5) == L".html")
		{
			CHtmlToRtf html2rtf;			
			LPTSTR wszbuffer;
			LOGFONT lf;
			lf.lfWidth = 5;
			lf.lfHeight = 12;
			lf.lfWeight = 1;
			COLORREF r1,r2;
			
			CString str((LPCSTR)g_pLicBuff);
			CString strLicense = html2rtf.Convert(str, lf, r1, r2);
			
			m_richLic.SetWindowText(strLicense);
		}
	}

}

bool CLicAgreePage::StoreValues(void)
{
	return true;
}

// Overrides from base class
int CLicAgreePage::OnSetActive()
{
	if (m_buttonAccept.GetCheck() == BST_UNCHECKED)
		this->SetWizardButtons(PSWIZB_BACK);
	else
		this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CLicAgreePage::OnWizardNext()
{
	return m_pTestWizardInfo->FindNextPage(IDD);
}

int CLicAgreePage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CLicAgreePage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}


LRESULT CLicAgreePage::OnBnClickedCheckWiz97LicAgreement(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	if (m_buttonAccept.GetCheck() == BST_UNCHECKED)
		this->SetWizardButtons(PSWIZB_BACK);
	else
		this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

	return 0;
}
