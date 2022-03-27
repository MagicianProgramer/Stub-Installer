// InterfacePage.cpp : implementation file
//

#include "stdafx.h"
#include "MainDlgSheet.h"
#include "InterfacePage.h"
#include "afxdialogex.h"

#include "../common/common.h"
#include "WizardPropertySheet.h"


// CInterfacePage dialog

IMPLEMENT_DYNAMIC(CInterfacePage, CPropertyPage)

CInterfacePage::CInterfacePage()
	: CPropertyPage(CInterfacePage::IDD)
	, m_bModernInterface(FALSE)
{
	m_imgModernInterface.LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_NO_INTERFACE);
	m_imgWizardInterface.LoadFromResource(AfxGetApp()->m_hInstance, IDB_BITMAP_WIZARD_INTERFACE);

	m_hDestDC = NULL;
}

CInterfacePage::~CInterfacePage()
{
}

void CInterfacePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_WIZARD_NO_INTERFACE, m_bModernInterface);
}


BEGIN_MESSAGE_MAP(CInterfacePage, CPropertyPage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_WIZARD_NO_INTERFACE, &CInterfacePage::OnBnClickedRadioWizardNoInterface)
	ON_BN_CLICKED(IDC_RADIO_WIZARD_CLASSIC_WIZARD, &CInterfacePage::OnBnClickedRadioWizardClassicWizard)
END_MESSAGE_MAP()


// CInterfacePage message handlers
void CInterfacePage::GetParams()
{
	g_globaldata.interface_sh3.main.nGraphicInterfaceMode = 2 - (int)m_bModernInterface;
}

BOOL CInterfacePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	m_hDestDC = GetDlgItem(IDC_STATIC_INTERFACE_VIEW)->GetDC()->GetSafeHdc();
	SetStretchBltMode(m_hDestDC, COLORONCOLOR);

	CRect re;
	GetDlgItem(IDC_STATIC_INTERFACE_VIEW)->GetClientRect(&re);
	m_nDW = re.Width();
	m_nDH = re.Height();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CInterfacePage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CPropertyPage::OnPaint() for painting messages
	if (m_hDestDC)
	{
		if (!m_bModernInterface)
		{
			m_imgModernInterface.Draw(m_hDestDC, 0, 0, m_nDW, m_nDH);
		}
		else
		{
			m_imgWizardInterface.Draw(m_hDestDC, 0, 0, m_nDW, m_nDH);
		}
	}
	
}


void CInterfacePage::OnBnClickedRadioWizardNoInterface()
{
	// TODO: Add your control notification handler code here
	::StretchBlt(m_hDestDC, 0, 0, m_nDW, m_nDH, m_imgModernInterface.GetDC(), 0, 0, m_imgModernInterface.GetWidth(), m_imgModernInterface.GetHeight(), SRCCOPY);
	m_imgModernInterface.ReleaseDC();
	/*m_imgModernInterface.Draw(m_hDestDC, 0, 0, m_imgModernInterface.GetWidth(), m_imgModernInterface.GetHeight());*/
	m_bModernInterface = FALSE;
}


void CInterfacePage::OnBnClickedRadioWizardClassicWizard()
{
	// TODO: Add your control notification handler code here
	m_imgWizardInterface.Draw(m_hDestDC, 0, 0, m_nDW, m_nDH);
	m_bModernInterface = TRUE;
}


BOOL CInterfacePage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	GetParent()->PostMessage(UM_CURRENT_PAGE);

	return CPropertyPage::OnSetActive();
}
