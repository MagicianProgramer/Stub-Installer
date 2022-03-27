
#include "stdafx.h"
#include "CAddInfoPage.h"
#include "../../common/common.h"
#include "../../common/SHA1.h"

LRESULT CAddInfoPage::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CWaitCursor waitCursor;

	this->InitializeControls();
	this->InitializeValues();

	//user information query//
	if (!g_globaldata.interface_sh3.userinfo.chkUserName)
	{
		this->GetDlgItem(IDC_STATIC_USER_NAME).ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_EDIT_USER_NAME).ShowWindow(SW_HIDE);
	}
	else
	{
		char szbuffer[MAX_PATH];
		DWORD len = MAX_PATH;
		GetUserNameA(szbuffer, &len);
		CString str(szbuffer);
		GetDlgItem(IDC_EDIT_USER_NAME).SetWindowText(str);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkUserEmail)
	{
		this->GetDlgItem(IDC_STATIC_USER_EMAIL).ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_EDIT_USER_EMAIL).ShowWindow(SW_HIDE);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkCompanyName)
	{
		this->GetDlgItem(IDC_STATIC_COMPANY).ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_EDIT_COMPANY).ShowWindow(SW_HIDE);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkCustomFiled)
	{
		this->GetDlgItem(IDC_STATIC_CUSTOM_FIELD).ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_EDIT_CUSTOM_FIELD).ShowWindow(SW_HIDE);
	}
	else
	{
		CString strText;
		strText.Format(L"%s :*", g_globaldata.interface_sh3.userinfo.wszFieldText);
		this->GetDlgItem(IDC_STATIC_CUSTOM_FIELD).SetWindowText(strText);
	}

	if (!g_globaldata.interface_sh3.userinfo.chkSerialNumber)
	{
		this->GetDlgItem(IDC_STATIC_SERIAL_NUMBER).ShowWindow(SW_HIDE);
		this->GetDlgItem(IDC_EDIT_SERIAL_NUMBER).ShowWindow(SW_HIDE);
	}

	return 1;
}

LRESULT CAddInfoPage::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// Be sure the base gets the message too
	bHandled = FALSE;

	//this->UninitializeControls();
	return 0;
}


void CAddInfoPage::InitializeControls(void)
{
	
}

void CAddInfoPage::UninitializeControls(void)
{
}

void CAddInfoPage::InitializeValues(void)
{
	SetEnableNextButton();
}

bool CAddInfoPage::StoreValues(void)
{
	

	return true;
}

// Overrides from base class
int CAddInfoPage::OnSetActive()
{
	if (m_bEnable)
		this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	else
		this->SetWizardButtons(PSWIZB_BACK);

	// 0 = allow activate
	// -1 = go back to page that was active
	// page ID = jump to page
	return 0;
}

int CAddInfoPage::CheckUserInfo()
{
	int nRet = 0;

	CString strtxt;
	if (g_globaldata.interface_sh3.userinfo.chkUserNameRequired)
	{
 		GetDlgItem(IDC_EDIT_USER_NAME).GetWindowText(strtxt);
 		if (strtxt.IsEmpty())	return -1;
	}

	if (g_globaldata.interface_sh3.userinfo.chkUserEmailRequired)
	{
		GetDlgItem(IDC_EDIT_USER_EMAIL).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	return -2;

		int k1, k2;
		k1 = strtxt.Find(L"@");
		k2 = strtxt.ReverseFind(L'.');
		if (k1 <= 0 || k2 <= 0 || k1 >= k2)		return -2;
	}

	if (g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired)
	{
		GetDlgItem(IDC_EDIT_COMPANY).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	return -3;
	}

	if (g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired)
	{
		GetDlgItem(IDC_EDIT_CUSTOM_FIELD).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	return -4;
	}

	if (g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired)
	{
		GetDlgItem(IDC_EDIT_SERIAL_NUMBER).GetWindowText(strtxt);
		CStringA strA(strtxt);

		//get hash value//
		CSHA1 sha1;
		sha1.Reset();
		sha1.Update((UINT_8 *)strA.GetBuffer(), strA.GetLength());
		sha1.Final();
		BYTE hashbuf[HASHLEN];
		sha1.GetHash(hashbuf);

		//compare//
		bool bSNExist = false;
		if (g_globaldata.interface_sh3.userinfo.nSNCount == 0)
		{
			bSNExist = true;
		}
		else
		{
			for (int i = 0; i < g_globaldata.interface_sh3.userinfo.nSNCount; i++)
			{
				if (memcmp((byte*)g_globaldata.interface_sh3.userinfo.wszSNList + i * HASHLEN,
					hashbuf, HASHLEN) == 0)
				{
					bSNExist = true;
					break;
				}
			}
		}

		if (bSNExist)		nRet = 1;
		else				nRet = -5;
	}
		
	return nRet;
}

int CAddInfoPage::OnWizardNext()
{
	int ret = CheckUserInfo();
	if (ret >= 0)
	{
		return m_pTestWizardInfo->FindNextPage(IDD);
	}
	else
	{
		CString strMsg = L"";
		switch(ret)
		{
		case -1:
			strMsg = L"Please input correct user name.";
			break;
		case -2:
			strMsg = L"Please input correct user email.";
			break;
		case -3:
			strMsg = L"Please input correct company name.";
			break;
		case -4:
			strMsg = L"Please input correct custom field.";
			break;
		case -5:
			strMsg = L"Please input correct serial number.";
			break;
		default:
			strMsg = L"Information is not correct or License Key does not match. Please input correct information.";
			break;
		}
		::MessageBox(NULL, strMsg, L"Warnning", MB_OK);
		return -1;
	}
	
}

int CAddInfoPage::OnWizardBack()
{
	return m_pTestWizardInfo->FindPreviousPage(IDD);
}

void CAddInfoPage::OnHelp()
{
	m_pTestWizardInfo->ShowHelp(IDD);
}


void CAddInfoPage::SetEnableNextButton()
{
	m_bEnable = TRUE;
	CString strtxt;
	if (g_globaldata.interface_sh3.userinfo.chkUserNameRequired)
	{
		GetDlgItem(IDC_EDIT_USER_NAME).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	m_bEnable = false;
	}

	if (m_bEnable && g_globaldata.interface_sh3.userinfo.chkUserEmailRequired)
	{
		GetDlgItem(IDC_EDIT_USER_EMAIL).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	m_bEnable = false;
	}

	if (m_bEnable && g_globaldata.interface_sh3.userinfo.chkCompanyNameRequired)
	{
		GetDlgItem(IDC_EDIT_COMPANY).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	m_bEnable = false;
	}

	if (m_bEnable && g_globaldata.interface_sh3.userinfo.chkCustomFiledRequired)
	{
		GetDlgItem(IDC_EDIT_CUSTOM_FIELD).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	m_bEnable = false;
	}

	if (m_bEnable && g_globaldata.interface_sh3.userinfo.chkSerialNumberRequired)
	{
		GetDlgItem(IDC_EDIT_SERIAL_NUMBER).GetWindowText(strtxt);
		if (strtxt.IsEmpty())	m_bEnable = false;
	}

	if (!m_bEnable)
	{
		this->SetWizardButtons(PSWIZB_BACK);
	}
	else
	{
		this->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	}
}



LRESULT CAddInfoPage::OnEnChangedEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetEnableNextButton();
	return 0L;
}