// CreateODBCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "CreateODBCDlg.h"
#include "afxdialogex.h"

CString strDataSouceName[] = {
	L"dBASE Files",
	L"Excel Files",
	L"MS Access Database"
};

CString strDrive[] = {
	L"Microsoft Access dBASE Driver (*.dbf, *.ndx, *.mdx)",
	L"Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)",
	L"Microsoft Access Driver (*.mdb, *.accdb)"
};

// CreateODBCDlg dialog

IMPLEMENT_DYNAMIC(CreateODBCDlg, CDialogEx)

CreateODBCDlg::CreateODBCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreateODBCDlg::IDD, pParent)
{

}

CreateODBCDlg::~CreateODBCDlg()
{
}

void CreateODBCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TOOLS_CREATE_ODBC_DATA, m_ctrlListReg);
	DDX_Control(pDX, IDC_COMBO_TOOLS_CREATE_ODBC_DATA_SOURCE_NAME, m_comCtrlDataSouceName);
	DDX_Control(pDX, IDC_COMBO_TOOLS_CREATE_ODBC_DATA_DRIVER, m_comCtrlDrive);
	DDX_Control(pDX, IDC_COMBO_TOOLS_CREATE_ODBC_DATA_SOURCE_REGISTER_FOR, m_comCtrlRegFor);
}


BEGIN_MESSAGE_MAP(CreateODBCDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TOOLS_CREATE_ODBC_DATA_SOURCE_NAME, &CreateODBCDlg::OnCbnSelchangeComboToolsCreateOdbcDataSourceName)
	ON_CBN_SELCHANGE(IDC_COMBO_TOOLS_CREATE_ODBC_DATA_DRIVER, &CreateODBCDlg::OnCbnSelchangeComboToolsCreateOdbcDataDriver)
	ON_BN_CLICKED(IDOK, &CreateODBCDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CreateODBCDlg message handlers
void CreateODBCDlg::InitLst()
{
	/*m_ctrlListReg.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP );
	m_ctrlListReg.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);*/
	m_ctrlListReg.InsertColumn(0, L"Name", LVCFMT_LEFT, 150);
	m_ctrlListReg.InsertColumn(1, L"Value", LVCFMT_LEFT,240);
	//m_ctrlListReg.SetColumnHeader(L"Name; Value");
	m_ctrlListReg.SetGridLines(TRUE); // SHow grid lines
	m_ctrlListReg.SetEditable(TRUE); // Allow sub-text edit
}

BOOL CreateODBCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitLst();
	m_comCtrlRegFor.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CreateODBCDlg::OnCbnSelchangeComboToolsCreateOdbcDataSourceName()
{
	// TODO: Add your control notification handler code here
	m_comCtrlDrive.SetCurSel(m_comCtrlDataSouceName.GetCurSel());
	SetLst();
}


void CreateODBCDlg::OnCbnSelchangeComboToolsCreateOdbcDataDriver()
{
	// TODO: Add your control notification handler code here
	SetLst();
}

void CreateODBCDlg::SetLst()
{
	m_ctrlListReg.DeleteAllItems();
	INT nDataSrc = m_comCtrlDataSouceName.GetCurSel();
	INT nDrive = m_comCtrlDrive.GetCurSel();

	m_ctrlListReg.InsertItem(m_ctrlListReg.GetItemCount(), L"UID");
	m_ctrlListReg.SetItemText(m_ctrlListReg.GetItemCount() - 1, 1, L"");

	m_ctrlListReg.InsertItem(m_ctrlListReg.GetItemCount(), L"SafeTransactions");
	m_ctrlListReg.SetItemText(m_ctrlListReg.GetItemCount() - 1, 1, L"0");

	CString str;
	switch (nDataSrc)
	{
	case 0:
		str = L"533";
		break;
	case 1:
		str = L"1046";
		break;
	case 2:
		str = L"25";
		break;
	}

	m_ctrlListReg.InsertItem(m_ctrlListReg.GetItemCount(), L"DriverId");
	m_ctrlListReg.SetItemText(m_ctrlListReg.GetItemCount() - 1, 1, str);

	m_ctrlListReg.InsertItem(m_ctrlListReg.GetItemCount(), L"Driver");
	m_ctrlListReg.SetItemText(m_ctrlListReg.GetItemCount() - 1, 1, L"C:\\PROGRA~2\\COMMON~1\\MICROS~1\\OFFICE14\\ACEODBC.DLL");
}

void CreateODBCDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	RETSTU stu;
	stu.strRegKey = L"HKCU\\Software\\ODBC\\ODBC.INI\\ODBC Data Sources";
	stu.strName = strDataSouceName[m_comCtrlDataSouceName.GetCurSel()];
	stu.strValue = strDrive[m_comCtrlDrive.GetCurSel()];
	m_vecRet.push_back(stu);

	for (INT i = 0; i < m_ctrlListReg.GetItemCount(); i++)
	{
		stu.strRegKey = L"HKCU\\Software\\ODBC\\ODBC.INI\\" + strDataSouceName[m_comCtrlDataSouceName.GetCurSel()];
		stu.strName = m_ctrlListReg.GetItemText(i, 0);
		stu.strValue = m_ctrlListReg.GetItemText(i, 1);
		m_vecRet.push_back(stu);
	}

	CDialogEx::OnOK();
}
