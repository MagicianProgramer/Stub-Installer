#include "stdafx.h"
#include "CDownloaderDlg.h"


CDownloaderDlg::CDownloaderDlg()
{
	m_strURL = L"";
	m_bDownloadSuccess = FALSE;
}


CDownloaderDlg::~CDownloaderDlg()
{
}



CDownloaderDlg *pDownloaderDlg = NULL;

LRESULT CDownloaderDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Do some initialization code

	//center//
	HWND   hwndScreen;
	RECT   rectScreen;
	int    PosX;
	int    PosY;
	hwndScreen = GetDesktopWindow();
	::GetWindowRect(hwndScreen, &rectScreen);
	

	RECT rewnd;
	GetWindowRect(&rewnd);

	PosX = (rectScreen.right - (rewnd.right - rewnd.left)) >> 1;
	PosY = (rectScreen.bottom - (rewnd.bottom - rewnd.top)) >> 1;
	::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, PosX, PosY, (rewnd.right - rewnd.left), (rewnd.bottom - rewnd.top), SWP_NOOWNERZORDER);

	//init param//
	m_progWind = this->GetDlgItem(IDC_PROGRESS_DOWNLOADING);
	SendMessage(m_progWind.m_hWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 100));

	m_bPkg = FALSE;
	m_hThread = NULL;
	m_bRunningThread = FALSE;
	m_nWritten = 0;
	m_nTotalSize = 0;

	//set window title//
	CString strTitle;
	strTitle.Format(L"Download <%s>", m_strFileName);
	SetWindowText(strTitle);

	//libcurl//
		
	init_curl();

	if (m_strURL.IsEmpty())
	{
		CloseDlg();
	}
	else
	{
		nDownloaderFlag = 0;
		pDownloaderDlg = this;
		TryDownload();
	}
	
	return 1;
}


LRESULT CDownloaderDlg::OnBnClickedButtonAbortDownload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add your control notification handler code here
	CString strTxt;
	strTxt = L"Do you really want to stop downloading?";
	//if (MessageBox(strTxt, L"Downloading Prerequisies", MB_OKCANCEL | MB_ICONWARNING) == IDOK)
	CloseDlg();

	return 0;
}

void CDownloaderDlg::CloseDlg()
{
	if (m_bRunningThread)
	{
		Sleep(100);
		TerminateThread(m_hThread, 0);
	}

	finish_curl();

	EndDialog(0);
}

void CDownloaderDlg::SetURL(wchar_t* wszURL)
{
	m_strURL.Format(L"%s", wszURL);
	m_strOldURL = m_strURL;
	m_strFileName = m_strURL.Right(m_strURL.GetLength() - m_strURL.ReverseFind(L'/') - 1);
}

LRESULT CDownloaderDlg::OnCancel(WORD, WORD, HWND, BOOL&)
{
	CloseDlg();
	return 0L;
}


void CDownloaderDlg::SetSaveFileFullPathName(CString strPathName, BOOL bpkg)
{
	m_strSavePathName = strPathName;
	m_bPkg = bpkg;
}



void CDownloaderDlg::TryDownload()
{
	SendMessage(m_progWind.m_hWnd, PBM_SETPOS, 0, 0);

	CString strSts;
	//m_strFileName = m_strURL.Right(m_strURL.GetLength() - m_strURL.ReverseFind(L'/') - 1);
	strSts = L"<" + m_strFileName + L"> Download Starting";
	SetDlgItemText(IDC_STATIC_DOWN_PREREQUISITE, strSts);

	//start thread//
	DWORD dwThID;
	m_hThread = CreateThread(NULL, 0, thStartDownloading, this, 0, &dwThID);
}

DWORD WINAPI thStartDownloading(LPVOID lParam)
{
	CDownloaderDlg *pDlg = (CDownloaderDlg*)lParam;
	pDlg->Downloading();
	return 0;
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *fp)
{
	if (nDownloaderFlag == 1)
	{
		nDownloaderFlag = 2;
		CStringA str;
		memset((BYTE*)ptr + nmemb, 0, 1);
		str.Format("%s", (char*)ptr);
		str = str.Right(str.GetLength() - str.Find("href=\"") - 6);
		str = str.Left(str.Find("\">"));

		char* szbuff = new char[str.GetLength() + 1];
		memcpy(szbuff, str.GetBuffer(), str.GetLength() + 1);
		if (pDownloaderDlg)
		{
			pDownloaderDlg->PostMessage(UM_RETRY_DOWNLOAD, (WPARAM)szbuff);
		}

		return 0;
	}

	size_t written = 0;
	written = fwrite(ptr, size, nmemb, fp);

	pDownloaderDlg->m_nWritten += (int)(written);
	if (pDownloaderDlg->m_nTotalSize)
	{
		//pDownloaderDlg->m_progCtrl.SetPos(100 * pDownloaderDlg->m_nWritten / pDownloaderDlg->m_nTotalSize);
		SendMessage(pDownloaderDlg->m_progWind.m_hWnd, PBM_SETPOS, 100 * pDownloaderDlg->m_nWritten / pDownloaderDlg->m_nTotalSize, 0);
	}


	CString strTxt;
	strTxt.Format(L"<%s> Downloading ", pDownloaderDlg->m_strFileName);
	if (pDownloaderDlg->m_nWritten < 1024)
		strTxt.AppendFormat(L"(%d bytes", pDownloaderDlg->m_nWritten);
	else if (pDownloaderDlg->m_nWritten < 1024 * 1024)
		strTxt.AppendFormat(L"(%.2f KB", (float)pDownloaderDlg->m_nWritten / 1024.0);
	else
		strTxt.AppendFormat(L"(%.2f MB", (float)pDownloaderDlg->m_nWritten / (1024.0 * 1024.0));

	if (pDownloaderDlg->m_nTotalSize < 1024)
		strTxt.AppendFormat(L"/%d bytes)", pDownloaderDlg->m_nTotalSize);
	else if (pDownloaderDlg->m_nTotalSize < 1024 * 1024)
		strTxt.AppendFormat(L"/%.2f KB)", (float)pDownloaderDlg->m_nTotalSize / 1024.0);
	else
		strTxt.AppendFormat(L"/%.2f MB)", (float)pDownloaderDlg->m_nTotalSize / (1024.0 * 1024.0));

	pDownloaderDlg->SetDlgItemText(IDC_STATIC_DOWN_PREREQUISITE, strTxt);

	return written;
}

void CDownloaderDlg::Downloading()
{
	m_bDownloadSuccess = FALSE;
	//get tmp file full path name//
	CString strTmpDir = L"";

	if (m_strSavePathName.IsEmpty())
	{
		TRegistry reg(HKEY_CURRENT_USER);
		if (reg.OpenKey(REGSTR_SHELLFOLDERS))
		{
			CString strFile;

			strFile.Format(L"%s", L"Desktop");

			CString strBuf = reg.GetStr(strFile, L"");
			strTmpDir = strBuf;
			strTmpDir = strTmpDir.Left(strTmpDir.ReverseFind(L'\\'));
			strTmpDir = strTmpDir + L"\\Downloads";
		}

		if (strTmpDir.IsEmpty())	return;
		if (!PathFileExists(strTmpDir))		SHCreateDirectory(NULL, strTmpDir);
		m_strTmpFilePathName = strTmpDir + L"\\" + m_strFileName;
	}
	else
	{
		m_strTmpFilePathName = m_strSavePathName;
	}

	//start down//
	m_bRunningThread = TRUE;

	CStringA fname(m_strTmpFilePathName);
	CStringA url(m_strURL);


	//get downfile size//
	m_nWritten = 0;
	m_nTotalSize = get_download_size(url);

	CURLcode res = CURLE_FAILED_INIT;
	if (m_nTotalSize > 0)
	{
		//down file//
		CURL *curl;
		FILE *fp = NULL;
		double filesize = 0.0;

		fp = fopen(fname, "wb");

		if (fp)
		{
			curl = curl_easy_init();
			if (curl)
			{
				/* set URL to get here */
				curl_easy_setopt(curl, CURLOPT_URL, url);

				/* Switch on full protocol/debug output while testing */
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);//////////

															/* disable progress meter, set to 0L to enable it */
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

				//skip peer verification//
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

				//skip hostname verification//
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

				/* write the page body to this file handle */
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

				/* send all data to this function  */
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

				//perform//
				res = curl_easy_perform(curl);

				curl_easy_cleanup(curl);
				fclose(fp);
			}
		}

		//down complete//
		m_bRunningThread = FALSE;

		if (res == CURLE_OK/*CURLE_RECV_ERROR*/)
		{
			if (m_nTotalSize > m_nWritten)
			{
				CString strMsg;
				strMsg = L"The server replies that you don't have permissions to download this file.\n\n";
				strMsg += L"Details:\nHTTP/1.1 403 Forbidden";
				MessageBox(strMsg, m_strFileName, MB_OK | MB_ICONWARNING);
				ShellExecute(NULL, L"open", m_strOldURL, NULL, NULL, SW_SHOWNORMAL);//open web browser
				CloseDlg();
			}
			else
			{
				CString strTxt = L"<" + m_strFileName + L"> Downloading Done!";
				SetDlgItemText(IDC_STATIC_DOWN_PREREQUISITE, strTxt);
				//m_progCtrl.SetPos(100);
				SendMessage(pDownloaderDlg->m_progWind.m_hWnd, PBM_SETPOS, 100, 0);
				m_bDownloadSuccess = TRUE;

				//MessageBox(strTxt, L"Downloading Prerequisies", MB_OK|MB_ICONINFORMATION);
				if (!m_bPkg)
				{
					ShellExecute(NULL, L"open", m_strTmpFilePathName, NULL, NULL, SW_SHOWNORMAL);//execute downloaded file//
				}
				CloseDlg();
			}
		}
		else
		{
			CString strTxt = L"<" + m_strFileName + L"> Downloading Failed!";
			SetDlgItemText(IDC_STATIC_DOWN_PREREQUISITE, strTxt);
			strTxt = strTxt + L"\r\nDo you want to retry?";
			if (MessageBox(strTxt, L"Downloading Prerequisies", MB_RETRYCANCEL | MB_ICONWARNING) != IDRETRY)
				CloseDlg();
			else
				PostMessage(UM_RETRY_DOWNLOAD);
		}
	}
	else if (nDownloaderFlag == 0)
	{
		nDownloaderFlag = 1;

		//down file//
		CURL *curl;
		double filesize = 0.0;
		curl = curl_easy_init();
		if (curl) {
			curl_easy_setopt(curl, CURLOPT_URL, url);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
			res = curl_easy_perform(curl);
			curl_easy_cleanup(curl);
		}
	}
	else
	{
		CString strTxt = L"<" + m_strFileName + L"> Downloading Failed!";
		SetDlgItemText(IDC_STATIC_DOWN_PREREQUISITE, strTxt);
		strTxt = strTxt + L"\r\nDo you want to retry?";
		if (MessageBox(strTxt, L"Downloading Prerequisies", MB_RETRYCANCEL | MB_ICONWARNING) != IDRETRY)
			CloseDlg();
		else
		{
			nDownloaderFlag = 0;
			PostMessage(UM_RETRY_DOWNLOAD);
		}
	}

}

LRESULT CDownloaderDlg::OnRetryDownload(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	Sleep(50);

	if (wParam)
	{
		char* szurl = (char*)wParam;

		CString strurl(szurl);
		m_strURL = strurl;
		m_strFileName = m_strURL.Left(m_strURL.Find(L"?"));
		m_strFileName = m_strFileName.Right(m_strFileName.GetLength() - m_strFileName.ReverseFind(L'/') - 1);

		delete[] szurl;
	}	

	TryDownload();

	return 0L;
}
