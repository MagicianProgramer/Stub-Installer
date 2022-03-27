// MyCrumbBar.cpp : implementation file
//

#include "stdafx.h"
#include "BlockCrumbBar.h"

#define EXTRA_WIDTH 10

IMPLEMENT_DYNAMIC(CBlockCrumbBar, MZCrumbBar)

CBlockCrumbBar::CBlockCrumbBar()
{
  m_hBrush1 = ::CreateSolidBrush(RGB(240,240,240));
  // hot
  m_hBrushHot = ::CreateSolidBrush(RGB(255,236,139));

  m_hBrushPre = ::CreateSolidBrush(RGB(216,191,216));
  m_hBrushPreHot = ::CreateSolidBrush(RGB(234,173,234));

  m_bCustomDraw = false;
}

CBlockCrumbBar::~CBlockCrumbBar()
{
  if (m_hBrush1)
    ::DeleteObject(m_hBrush1);

  if (m_hBrushHot)
    ::DeleteObject(m_hBrushHot);

  if (m_hBrushPre)
    ::DeleteObject(m_hBrushPre);
  
}


BEGIN_MESSAGE_MAP(CBlockCrumbBar, MZCrumbBar)
END_MESSAGE_MAP()

BOOL CBlockCrumbBar::Create( DWORD dwExStyle )
{
  MZCrumbBar::Create(dwExStyle);

  LOGFONT lf;
  m_fontHot.GetLogFont(&lf);
  lf.lfUnderline = 0;
  lf.lfWeight = FW_BOLD;
  m_fontHotCustom.DeleteObject();
  m_fontHotCustom.CreateFontIndirect(&lf);

  return TRUE;
}

int CBlockCrumbBar::OnMeasureItem( CDC* pDC, CrumbBarItem* pItem )
{
  if(m_bCustomDraw)
  {
    // since we are using bold font for HOT on custom draw. then we need to measure the size using the bold font
    CFont* pOldFont = pDC->SelectObject(&m_fontHotCustom);
    int nLen = EXTRA_WIDTH + MZCrumbBar::OnMeasureItem(pDC, pItem);
    pDC->SelectObject(pOldFont);
    return nLen;
  }
  else
    return MZCrumbBar::OnMeasureItem(pDC, pItem);
}

int CBlockCrumbBar::OnMeasurePreItem( CDC* pDC )
{
  if(m_bCustomDraw)
  {
    CFont* pOldFont = pDC->SelectObject(&m_fontHotCustom);
    int nLen = EXTRA_WIDTH + pDC->GetTextExtent(_T("<")).cx;
    pDC->SelectObject(pOldFont);
    return nLen;
  }
  else
    return MZCrumbBar::OnMeasurePreItem(pDC);
}

void CBlockCrumbBar::DrawCrumbItem( CrumbBarItem* pItem, CDC* pDC, CRect rc )
{
  if(m_bCustomDraw == false)
  {
    MZCrumbBar::DrawCrumbItem(pItem, pDC, rc);
    return;
  }

  bool bHot = false;
  if(m_DrawState.pHotItem == pItem)
    bHot = true;

  COLORREF crTextColor = m_crColorDefault;
  if(bHot)
    crTextColor = m_crColorHot;

  pDC->SetTextColor(crTextColor);

  CFont* pOldFont = NULL;
  if(bHot)
    pOldFont = pDC->SelectObject(&m_fontHotCustom);


  rc.DeflateRect(0,1,0,1);

  CBrush br;
  if(bHot)
    br.Attach(m_hBrushHot);
  else
    br.Attach(m_hBrush1);

  pDC->FillRect(rc,&br);
  br.Detach();

  pDC->Draw3dRect(rc, RGB(0,0,0), RGB(120,130,125) );
  pDC->DrawText(pItem->DisplayName, - 1,rc, DT_CENTER| DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

  if(pOldFont)
    pDC->SelectObject(pOldFont);

}

void CBlockCrumbBar::DrawPreItem( CrumbBarItem* pPreItem, CDC* pDC, CRect rc )
{
  if(m_bCustomDraw == false)
  {
    MZCrumbBar::DrawPreItem(pPreItem, pDC, rc);
    return;
  }

  bool bHot = false;
  if(m_DrawState.pHotItem == pPreItem)
    bHot = true;

  COLORREF crTextColor = m_crColorDefault;
  if(bHot)
    crTextColor = m_crColorHot;

  pDC->SetTextColor(crTextColor);

  CFont* pOldFont = NULL;
  if(bHot)
    pOldFont = pDC->SelectObject(&m_fontHotCustom);


  rc.DeflateRect(0,1,0,1);

  CBrush br;
  if(bHot)
    br.Attach(m_hBrushPreHot);
  else
    br.Attach(m_hBrushPre);

  pDC->FillRect(rc,&br);
  br.Detach();

  pDC->Draw3dRect(rc, RGB(240,0,0), RGB(255,0,0) );
  pDC->DrawText(_T("<"), - 1,rc, DT_CENTER| DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

  if(pOldFont)
    pDC->SelectObject(pOldFont);

}




