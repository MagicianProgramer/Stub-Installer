#pragma once

#include "BlankSheet.h"

// CBlankMainDlg dialog

class CBlankMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBlankMainDlg)

public:
	CBlankMainDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CBlankMainDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BLANK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	CBlankPropertySheet m_blankSheet;
public:
	
};
