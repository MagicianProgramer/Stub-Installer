
// StubDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "Hyperlink.h"
#include "StubOptionDlg.h"
#include "StubLicenseDlg.h"

// CStubDlg dialog
class CStubDlg : public CDialogEx
{
// Construction
public:
	CStubDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_STUB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnStartInstall();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CFile m_rfile;
	BOOL m_bOpened;
	unsigned char* m_CprPtr;
	unsigned char* m_DataPtr;

	
	void StartMainInstall();
	void CloseFunction();
	BOOL GetGlobalOptionParam();

	///draw back////
	//rect//
	CRect m_wndRect;
	CRect m_reLogo;
	CRect m_reHdr;
	CRect m_reText;
	CRect m_reButton;
	CRect m_reOption;
	CRect m_reCopyright;
	CRect m_reLink;
	void GetRects();

	CString m_strText;
	CString m_strHeader;
	CString m_strBtn;
	void DrawWindowComponents(BOOL bLinkChange = TRUE);
	void DrawLogo();
	void DrawHeader();
	void DrawText();
	
	CFont m_Fontbtn;
	CFont m_Fontlnk;
	void DrawButton();
	void DrawLicenseAndCopyright(BOOL bLinkChange = FALSE);
	void DisplayText(HDC dc, wchar_t* wsztext, RECT r, wchar_t* wszFontFaceName, int fontsize, COLORREF clr, int dwStyle = 0, int bTop = 0);

	//proc(unzip)//
	BOOL ExtractZipFiles();
	CString GetSaveFileFullPathName(wchar_t* wszName, CString& strFullDir, BOOL bDir = FALSE);

	CProgressCtrl m_ctrlPogExtract;
	void SetStatusText(CString strSts);

	//////////////////////////////
	void CreateShortcuts();
	void DeleteShortcuts();
	void CreateLink(CString strLocation, CString strName, CString strTarget);

	/////////////////////////////
	void RegisterValues();
	void UnregisterValues();

	CString UnZipFile(wchar_t* wszFileFullPath, BOOL bPkg = FALSE);
	byte* UnZipBuffer(BOOL btext = FALSE);

	void GetBitmaps();
	void GetLicAndReadme();
	byte* m_pLogoBitmapBuff;
	byte* m_pLicBuff;
	byte* m_pReadmeBuff;
	void DrawBackground();

	void ExtractUninstaller();
	void ExtractUpdater();

	void ExtractTempFiles();
public:
	//unistall and repair//
	void UninstallApp();
	CString RepairFile(wchar_t* wszDestPathName);
	void RepairApp();
public:
	HANDLE m_hExtractThread;
	void UnzipProc(BOOL bRepair = FALSE);
	BOOL m_bRun;
	BOOL m_bComplete;
	BOOL m_bPause;


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CButton m_btnInstall;
	CHyperlink m_lnkOption;
	CHyperlink m_lnkLicense;
	CHyperlink m_lnkCompany;
	afx_msg void OnBnClickedSyslinkStubLicenseLink();
	afx_msg void OnBnClickedSyslinkStubOption();
	afx_msg void OnBnClickedSyslinkStubCopyrightLink();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
private:
	int CheckUserInfo();
public:
	
};

DWORD WINAPI thStartUnzipProc(LPVOID lParam);
DWORD WINAPI thRepairing(LPVOID lParam);
DWORD WINAPI thUninstalling(LPVOID lParam);