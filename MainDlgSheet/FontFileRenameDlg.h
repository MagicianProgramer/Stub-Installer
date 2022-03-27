#pragma once


// CFontFileRenameDlg dialog

class CFontFileRenameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFontFileRenameDlg)

public:
	CFontFileRenameDlg(CString strFile, CWnd* pParent = NULL);   // standard constructor
	virtual ~CFontFileRenameDlg();

// Dialog Data
	enum { IDD = IDD_FONT_FILE_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
public:
	CString m_strRename;	
};
