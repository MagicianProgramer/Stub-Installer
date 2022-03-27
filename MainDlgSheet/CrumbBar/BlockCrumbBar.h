#pragma once

#include "MZCrumbBar.h"

class CBlockCrumbBar : public MZCrumbBar
{
	DECLARE_DYNAMIC(CBlockCrumbBar)

public:
	CBlockCrumbBar();
	virtual ~CBlockCrumbBar();

  void CustomDraw(bool b){m_bCustomDraw = b;}
protected:
  virtual BOOL Create( DWORD dwExStyle = 0 );

  // Return width of item.
  virtual int OnMeasureItem(CDC* pDC, CrumbBarItem* pItem);
  // Return width if the PreItem that is shown if not all items fits in the controller
  virtual int OnMeasurePreItem(CDC* pDC);

  // Draw CrumbBar Item
  virtual void DrawCrumbItem(CrumbBarItem* pItem, CDC* pDC, CRect rc);
  virtual void DrawPreItem(CrumbBarItem* pItem, CDC* pDC, CRect rc);

	DECLARE_MESSAGE_MAP()
private:
  HBRUSH	m_hBrush1;
  HBRUSH  m_hBrushHot;
  HBRUSH  m_hBrushPre;
  HBRUSH  m_hBrushPreHot;
  bool m_bCustomDraw;
  CFont   m_fontHotCustom;
};


