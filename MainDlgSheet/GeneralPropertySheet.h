#pragma once
#include "afxdlgs.h"
#include "GeneralInformationDlg.h"
#include "GeneralParametersDlg.h"
#include "GeneralRequirementsDlg.h"
#include "GeneralPrequisitesDlg.h"
#include "GeneralOutputDlg.h"

#include <vector>
using namespace std;

class CGeneralPropertySheet :
	public CPropertySheet
{
	DECLARE_DYNAMIC(CGeneralPropertySheet)
public:
	CGeneralPropertySheet(LPCTSTR pszCaption = L"", CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CGeneralPropertySheet(void);

	void SetActive1stPage(BOOL bNewGuid = FALSE);
private:
	CGeneralInformationDlg m_GeneralInformationDlg;
	CGeneralParametersDlg m_GeneralParameterDlg;
	CGeneralRequirementsDlg m_GeneralRequirementsDlg;
	CGeneralPrequisitesDlg m_GeneralPrequisitesDlg;
	CGeneralOutputDlg m_GeneralOutputDlg;
	
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	void NextPage();
	void PrevPage();
	void SetPage(INT idx);
	CString GetTitle();
private:
	INT m_nTotalDlgs;
	vector<CString> m_vecStrTitle;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

