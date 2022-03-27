#pragma once

#include "../../common/downloader.h"


class CDownloaderDlg : public CDialogImpl<CDownloaderDlg>
{
public:
	CDownloaderDlg();
	~CDownloaderDlg();
public:
	enum { IDD = IDD_DIALOG_DOWNLOADER };

	BEGIN_MSG_MAP(CDownloaderDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(UM_RETRY_DOWNLOAD, OnRetryDownload)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON_ABORT_DOWNLOAD, BN_CLICKED, OnBnClickedButtonAbortDownload)
	END_MSG_MAP()
	
public:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRetryDownload(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonAbortDownload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD, WORD, HWND, BOOL&);

	void CloseDlg();
	void SetURL(wchar_t* wszURL);

	//package//
	BOOL m_bPkg;
	CString m_strSavePathName;
	void SetSaveFileFullPathName(CString strPathName, BOOL bpkg);
/////////////////////////////////////
public:
	CString m_strURL;
	CString m_strOldURL;
	void Downloading();
	CString m_strFileName;
	CString m_strTmpFilePathName;
	CWindow m_progWind;
	HANDLE m_hThread;
	BOOL m_bRunningThread;

	int m_nWritten;
	int m_nTotalSize;

	BOOL m_bDownloadSuccess;
public:
	void TryDownload();
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

DWORD WINAPI thStartDownloading(LPVOID lParam);


