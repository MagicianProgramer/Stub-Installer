#pragma once


// CDownloaderDlg1 dialog

class CDownloaderDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDownloaderDlg1)

public:
	CDownloaderDlg1(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDownloaderDlg1();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DOWNLOADER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonAbortDownload();
	CProgressCtrl m_progCtrl;
	afx_msg void OnClose();

	void SetURL(wchar_t* wszURL);

	//package//
	BOOL m_bPkg;
	CString m_strSavePathName;
	void SetSaveFileFullPathName(CString strPathName, BOOL bpkg);
public:
	CString m_strURL;
	CString m_strOldURL;
	void Downloading();
	CString m_strFileName;
	CString m_strTmpFilePathName;
	HANDLE m_hThread;
	BOOL m_bRunningThread;

	int m_nWritten;
	int m_nTotalSize;

	BOOL m_bDownloadSuccess;
public:
	void TryDownload();
};

DWORD WINAPI thStartDownloading(LPVOID lParam);
