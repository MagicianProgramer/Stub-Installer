#pragma once
#include "afxwin.h"

#include "ReportCtrl.h"
#include <vector>
using namespace std;

typedef struct _RETSTU{
	CString strRegKey;
	CString strName;
	CString strValue;
}RETSTU;
// CreateODBCDlg dialog

class CreateODBCDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreateODBCDlg)

public:
	CreateODBCDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CreateODBCDlg();

// Dialog Data
	enum { IDD = IDD_TOOLS_CREATE_ODBC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CReportCtrl m_ctrlListReg;
	void InitLst();
	void SetLst();	
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboToolsCreateOdbcDataSourceName();
	afx_msg void OnCbnSelchangeComboToolsCreateOdbcDataDriver();
	afx_msg void OnBnClickedOk();
	CComboBox m_comCtrlDataSouceName;
	CComboBox m_comCtrlDrive;
	CComboBox m_comCtrlRegFor;
public:
	vector<RETSTU>	m_vecRet;
	
};
