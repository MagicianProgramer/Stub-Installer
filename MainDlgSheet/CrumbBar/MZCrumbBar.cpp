/* MZCrumbBar.cpp

	MZCrumbBar Ctrl - Created by Mathias Svensson (C) Copyright 2009-2010 - ( http://result42.com/projects/MZCrumbBar )

  History 


  v1.2  2010-04-15  Added improvments/fixes provided by Iain Clarke
                      * 3 more BuildCrumbbar(...) function for special situations.
                      * Refactored how a path is parsed.
                      * Added CRU_LASTITEMNODELIMITER. If style is set the Delimiter will not be shown for last crumbbar item.
                      * Style can now be set using styleEx.
                      * Change Default hot color to COLOR_HOTLIGHT
                      * Added possibility to store custom data in a Crumbar item.
                      * Added SetItemData/GetItemData of custom data to crumbbar items.
                    Fixed so that if double delimiter now works again. Double delimiter should be part of the crumbpath name, not a splitter.
                    Change so that items are added to m_vCrumbItems via the virtual function AddCrumbItem(...)
                    Fixed so that empty path parts are not added to the path.

  v1.1  2010-03-17  First Public Version, Fixed minor draw issue
  v1.0  2009-10-10  First Version

  All rights reserved.

  Copyright / Usage Details:

  You are allowed to include this source code in any product (commercial, shareware, freeware or otherwise) 
  when your product is released in binary form. You are allowed to modify the source code in any way you want 
  except you cannot modify the copyright details at the top of each module. If you want to distribute source 
  code with your application, then you are only allowed to distribute versions released by the author. This is 
  to maintain a single distribution point for the source code. If you fix any bug or add enhachment please send
  the changes to the author so they may be included in the source.

*/

#include "stdafx.h"
#include "MZCrumbBar.h"

#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

#define MZCRUMBBARCTRL_CLASSNAME _T("MZCrumbBarCtrl")

IMPLEMENT_DYNAMIC(MZCrumbBar, CWnd)

namespace
{
	class CMemoryDC: public CDC
	{
	public:
		CMemoryDC(CDC* pDC) : CDC()
		{
			ASSERT(pDC != NULL);

			m_pDC = pDC;
			m_pOldBitmap = NULL;
			m_bMemDC = !pDC->IsPrinting();

			if (m_bMemDC)    // Create a Memory DC
			{
				pDC->GetClipBox(&m_rect);
				CreateCompatibleDC(pDC);
				m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
				m_pOldBitmap = SelectObject(&m_bitmap);
				SetWindowOrg(m_rect.left, m_rect.top);
				FillSolidRect(m_rect, pDC->GetBkColor());
			}
			else        
			{
				// Make a copy of the relevant parts of the current DC for printing
				m_hDC       = pDC->m_hDC;
				m_hAttribDC = pDC->m_hAttribDC;
			}

		}

		~CMemoryDC()
		{
			if (m_bMemDC)
			{
				// Copy the offscreen bitmap onto the screen.
				m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), this, m_rect.left, m_rect.top, SRCCOPY);

				//Swap back the original bitmap.
				SelectObject(m_pOldBitmap);
			} 
			else 
			{
				m_hDC = m_hAttribDC = NULL;
			}
		}

		CMemoryDC* operator->() { return this;  }
		operator CMemoryDC*()   { return this;  }

	private:
		CBitmap  m_bitmap;      // Offscreen bitmap
		CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
		CDC*     m_pDC;         // Saves CDC passed in constructor
		CRect    m_rect;        // Rectangle of drawing area.
		BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
	};

	class MZCrumbBar_InplaceEditCtrl : public CEdit
	{
		// Construction
	public:
		MZCrumbBar_InplaceEditCtrl(const CString& strText);
		virtual ~MZCrumbBar_InplaceEditCtrl(){};

		virtual BOOL PreTranslateMessage(MSG* pMsg);


	private:	
		CString m_strInitText;

		bool  m_bVK_ESCAPE;
		bool  m_bVK_RETURN;

		// Generated message map functions
	protected:
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg void OnNcDestroy();
		afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);	

		DECLARE_MESSAGE_MAP()
	};

	//////////////////////////////////////////////////////////////////////////
	// CListEditCtrl
	BEGIN_MESSAGE_MAP(MZCrumbBar_InplaceEditCtrl, CEdit)
		ON_WM_KILLFOCUS()
		ON_WM_NCDESTROY()
		ON_WM_CHAR()
		ON_WM_CREATE()
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CListEditCtrl message handlers

	MZCrumbBar_InplaceEditCtrl::MZCrumbBar_InplaceEditCtrl(const CString& strText)
	: m_strInitText(strText)
	{	
		m_bVK_ESCAPE = false;
		m_bVK_RETURN = false;
	}

	int MZCrumbBar_InplaceEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CEdit::OnCreate(lpCreateStruct) == -1)		
			return -1;	

		CFont* font = GetParent()->GetFont();
		SetFont(font);
		SetWindowText(m_strInitText);
		SetFocus();	
		SetSel(0, 0);
		return 0;
	}

	BOOL MZCrumbBar_InplaceEditCtrl::PreTranslateMessage(MSG* pMsg)
	{
		if( pMsg->message == WM_KEYDOWN )	
		{		
			// If MZCrumbBar is placed on a dialog and user are editing the CrumbBar and press return key we will close dialog
			// We do not want that
			// So while we are editing. do not send any keydown messages down to the dialog
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}

		return CEdit::PreTranslateMessage(pMsg);
	}


	void MZCrumbBar_InplaceEditCtrl::OnKillFocus(CWnd* pNewWnd)
	{	
		CEdit::OnKillFocus(pNewWnd);

		CString str;	
		GetWindowText(str);

		// Send Notification to parent (the MZCrumbBar Ctrl)
		LV_DISPINFO lvDispInfo;
		lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
		lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
		lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
		lvDispInfo.item.mask = LVIF_TEXT;	
		lvDispInfo.item.iItem = 0;
		lvDispInfo.item.iSubItem = 0;
		lvDispInfo.item.pszText = m_bVK_RETURN ? LPTSTR((LPCTSTR)str) : NULL;
		lvDispInfo.item.cchTextMax = str.GetLength();
		GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
		DestroyWindow();
	}


	void MZCrumbBar_InplaceEditCtrl::OnNcDestroy()
	{
		CEdit::OnNcDestroy();	
		delete this;
	}


	void MZCrumbBar_InplaceEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if( nChar == VK_ESCAPE || nChar == VK_RETURN)	
		{		
			if(nChar == VK_ESCAPE)
			{
				m_bVK_ESCAPE = true;
			}
			else
			{
				m_bVK_RETURN = true;
			}

			GetParent()->SetFocus();
			return;	
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);	
	}
}

//////////////////////////////////////////////////////////////////////////

MZCrumbBar::MZCrumbBar()
{
	WNDCLASS wndclass;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if(!(::GetClassInfo(hInst, MZCRUMBBARCTRL_CLASSNAME, &wndclass)))
	{
		wndclass.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc = ::DefWindowProc;
		wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
		wndclass.hInstance = hInst;
		wndclass.hIcon = NULL;
		wndclass.hCursor = LoadCursor(hInst, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = MZCRUMBBARCTRL_CLASSNAME;

		if (!AfxRegisterClass(&wndclass))
			AfxThrowResourceException();
	}

	m_bCreateForSubclassing = false;
	m_strDelimiter = _T('\\');
	m_nDelimiterMode = DM_RIGHT;

	m_DrawState.Clear();

	m_bDrawStateDirty = true;
	m_bMouseOver = false;
	m_pNotifyTarget = NULL;
	m_bDblClk = false;
	m_dwStyleEx = 0;
	m_bGradientBackground = false;
	m_hBackBrush = NULL;
	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	m_PreItem.nWidth = -1;
	m_PreItem.DisplayName = _T("[<<]");
	m_nMargin = 0;
	m_nItemSpacing = 0;

	m_crColorDefault = GetSysColor(COLOR_WINDOWTEXT);
	m_crColorHot = GetSysColor (COLOR_HOTLIGHT);
	m_crLoColor = GetSysColor(COLOR_WINDOWTEXT);
	m_crHiColor = GetSysColor(COLOR_WINDOWTEXT);
	m_crColorSelected = GetSysColor(COLOR_HIGHLIGHT);
}

MZCrumbBar::~MZCrumbBar()
{
	EmptyCrumbItems();

	if (m_hBackBrush)
		::DeleteObject(m_hBackBrush);

	if (m_hwndBrush)
		::DeleteObject(m_hwndBrush);
}

BEGIN_MESSAGE_MAP(MZCrumbBar, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_WM_SYSCOLORCHANGE()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_GETFONT, OnGetFont)
END_MESSAGE_MAP()


int MZCrumbBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void MZCrumbBar::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();

	if(!m_bCreateForSubclassing)
	{
		if(!Create( GetExStyle () ) )
			AfxThrowMemoryException();
	}
}

BOOL MZCrumbBar::Create( DWORD dwStyle, DWORD dwExStyle , const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /*= NULL*/ )
{
	m_bCreateForSubclassing = true;
	if(!CWnd::Create(MZCRUMBBARCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID, pContext))
		return FALSE;

	return Create( dwExStyle );
}

BOOL MZCrumbBar::Create( DWORD dwExStyle )
{
	// Create Internal resources font, bitmap and so on
	m_dwStyleEx = dwExStyle;

	// Create Default Font here.
	CWnd* pWnd = GetParent();
	if(pWnd)
	{
		CFont* pFont = pWnd->GetFont();
		if(pFont)
		{
			LOGFONT lf;
			pFont->GetLogFont(&lf);

			m_fontDefault.DeleteObject();
			m_fontDefault.CreateFontIndirect(&lf);

			lf.lfUnderline = 1;
			m_fontHot.DeleteObject();
			m_fontHot.CreateFontIndirect(&lf);
		}
	}

	return TRUE;
}

bool MZCrumbBar::GetPath(CString& pre, CString& hot, CString& tail, CrumbBarItem* pStartItem)
{
	CString strPath;
	CrumbBarItem* pItem;
	bool bFoundStartItem = false;
	bool bHotFound = false;

	if(pStartItem == NULL)
		bFoundStartItem = true;

	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{
		pItem = (*it);
		if(bFoundStartItem == false && pItem == pStartItem)
			bFoundStartItem = true;

		if(!bFoundStartItem)
			continue;

		if(m_DrawState.pHotItem == pItem)
		{
			pre = strPath;
			strPath = _T("");
		}

		strPath += pItem->DisplayName;

		if(m_DrawState.pHotItem == pItem)
		{
			hot = strPath;
			strPath = _T("");
			bHotFound = true;
		}

	}

	if(bHotFound)
	{
		tail = strPath;
	}
	else 
		pre = strPath;

	return true;
}

int MZCrumbBar::OnMeasureItem(CDC* pDC, CrumbBarItem* pItem)
{
	return pDC->GetTextExtent(pItem->DisplayName).cx;
}

int MZCrumbBar::OnMeasurePreItem(CDC* pDC)
{
	return OnMeasureItem(pDC, &m_PreItem);
}

MZCrumbBar::CrumbBarItem* MZCrumbBar::GetCrumbBarPreItem()
{
	CrumbBarItem* pPreItem = NULL; // Item before the start Item
	if(m_DrawState.bPreItem && m_DrawState.pStartItem != *m_vCrumbItems.begin())
	{
		for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
		{
			if((*it) == m_DrawState.pStartItem)
				return pPreItem;

			pPreItem = (*it);
		}

	}
	return NULL;
}

MZCrumbBar::CrumbBarItem* MZCrumbBar::FindFirstValidItem(int nMaxWidth)
{
	CrumbBarItem* pItem = NULL;

	int nSize = m_nMargin;
	int nSpacing = m_nItemSpacing;
	for(std::vector<CrumbBarItem*>::reverse_iterator it = m_vCrumbItems.rbegin(); it != m_vCrumbItems.rend(); ++it)
	{
		pItem = (*it);

		if(pItem == *m_vCrumbItems.begin())
			nSpacing = 0;

		if(nSize + nSpacing + pItem->nWidth > nMaxWidth)
		{
			// Get Previous item unless 'it' is the first ('last' since we going in revers)
			if(it != m_vCrumbItems.rbegin())
			{
				--it;
				pItem = (*it);

			}
			return pItem;
		}

		nSize += nSpacing;
		nSize += pItem->nWidth;
	}

	return pItem; // If we get there we returns the very first item in the list.
}

void MZCrumbBar::PrepareDraw(CDC* pDC, const CRect& rcClient)
{
	// Only recalculate everything if draws state is dirty
	if(!m_bDrawStateDirty)
		return;

	// Calculate the size of every item if needed
	CrumbBarItem* pItem;
	int nTotalSize = 0;
	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{
		if(it != m_vCrumbItems.begin())
			nTotalSize += m_nItemSpacing;

		pItem = (*it);
		if(pItem->nWidth == -1)
			pItem->nWidth = OnMeasureItem(pDC, pItem);

		nTotalSize += pItem->nWidth; 
	}

	if(nTotalSize + m_nMargin > rcClient.Width())
	{
		// Path is to long to be shown. so a "Show/Goto Previous" item is needed at the beginning of the crumbbar
		m_DrawState.bPreItem = true;
		if(m_PreItem.nWidth == -1)
			m_PreItem.nWidth = OnMeasurePreItem(pDC);
	}
	else
	{
		m_DrawState.bPreItem = false;
	}

	m_DrawState.pStartItem = FindFirstValidItem(rcClient.Width());

	// Not dirty anymore
	m_bDrawStateDirty = false;
}

void MZCrumbBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CMemoryDC MemDC(&dc);
	Draw(&MemDC);
}

// "borrowed" from CLabel - found at CodeProject ( http://www.codeproject.com/KB/static/clabel.aspx )
void MZCrumbBar::DrawGradientBackground(CDC* pDC, const CRect& rc, COLORREF crStart, COLORREF crEnd, int nSegments)
{
	// Get the starting RGB values and calculate the incremental
	// changes to be applied.

	COLORREF cr;
	int nR = GetRValue(crStart);
	int nG = GetGValue(crStart);
	int nB = GetBValue(crStart);

	int neB = GetBValue(crEnd);
	int neG = GetGValue(crEnd);
	int neR = GetRValue(crEnd);

	if(nSegments > rc.Width())
		nSegments = rc.Width();

	int nDiffR = (neR - nR);
	int nDiffG = (neG - nG);
	int nDiffB = (neB - nB);

	int ndR = 256 * (nDiffR) / (max(nSegments,1));
	int ndG = 256 * (nDiffG) / (max(nSegments,1));
	int ndB = 256 * (nDiffB) / (max(nSegments,1));

	nR *= 256;
	nG *= 256;
	nB *= 256;

	neR *= 256;
	neG *= 256;
	neB *= 256;

	int nCX = rc.Width() / max(nSegments,1), nLeft = rc.left, nRight;
	pDC->SelectStockObject(NULL_PEN);

	for (int i = 0; i < nSegments; i++, nR += ndR, nG += ndG, nB += ndB)
	{
		// Use special code for the last segment to avoid any problems
		// with integer division.

		if (i == (nSegments - 1))
			nRight = rc.right;
		else
			nRight = nLeft + nCX;

		cr = RGB(nR / 256, nG / 256, nB / 256);

		{
			CBrush br(cr);
			CBrush* pbrOld = pDC->SelectObject(&br);
			pDC->Rectangle(nLeft, rc.top, nRight + 1, rc.bottom);
			pDC->SelectObject(pbrOld);
		}

		// Reset the left side of the drawing rectangle.

		nLeft = nRight;
	}
}
/*
// Did not get it to work using ::GradientFill.. using code above instead..
//
void MZCrumbBar::DrawGradientBackground(CDC* pDC, const CRect& rc, COLORREF cr1, COLORREF cr2)
{
  TRIVERTEX        vert[2] ;
  GRADIENT_RECT    gRect;
  vert[0].x      = rc.left;
  vert[0].y      = rc.top;
  vert[0].Red    = GetRValue(cr1); 
  vert[0].Green  = GetGValue(cr1); 
  vert[0].Blue   = GetBValue(cr1); 
  vert[0].Alpha  = 0x0000;

  vert[1].x      = rc.right;
  vert[1].y      = rc.bottom; 
  vert[1].Red    = GetRValue(cr2);
  vert[1].Green  = GetGValue(cr2);
  vert[1].Blue   = GetBValue(cr2);
  vert[1].Alpha  = 0x0000;

  gRect.UpperLeft  = 0;
  gRect.LowerRight = 1;
  GradientFill(pDC->GetSafeHdc(), vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);
}
*/
void MZCrumbBar::DrawBackground(CDC* pDC, const CRect& rc)
{
	if(m_bGradientBackground)
	{
		DrawGradientBackground(pDC, rc, m_crLoColor, m_crHiColor, 100);
	}
	else
	{
		CBrush br;

		if (m_hBackBrush != NULL)
			br.Attach(m_hBackBrush);
		else
			br.Attach(m_hwndBrush);

		pDC->FillRect(rc,&br);

		br.Detach();
	}
}

void MZCrumbBar::Draw(CDC* pDC)
{
	CFont *pOldFont = pDC->SelectObject(&m_fontDefault);

//   CRect rectClip;
//   if (pDC->GetClipBox(&rectClip) == ERROR)
//     return;

	CRect rcClientRect;
	GetClientRect( &rcClientRect );

	//////////////////////////////////////////////////////////////////////////
	// Draw Background 
	DrawBackground(pDC, rcClientRect);

	// Draw items
	rcClientRect.left += m_nMargin;
	PrepareDraw(pDC, rcClientRect);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_crColorDefault);

	DrawCrumbItems(pDC, rcClientRect);

	// restore font
	pDC->SelectObject(pOldFont);
}

void MZCrumbBar::DrawPreItem(CDC* pDC, CRect& rcItem)
{
	if(m_DrawState.bPreItem && m_PreItem.nWidth > 0)
	{
		// Make sure that m_DrawState.pStartItem is not he first item
		ASSERT(m_DrawState.pStartItem != *m_vCrumbItems.begin());

		CrumbBarItem* pItem = GetCrumbBarPreItem();

		// draw pre-item
		rcItem.right = rcItem.left + m_PreItem.nWidth;
		DrawPreItem(pItem, pDC, rcItem);

		rcItem.left = rcItem.right;
		rcItem.left += m_nItemSpacing;
	}
}

void MZCrumbBar::DrawCrumbItems(CDC* pDC, CRect rc)
{
	CRect rcItem = rc;

	DrawPreItem(pDC, rcItem);

	bool bFoundFirstItem = false;
	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{
		CrumbBarItem* pItem = (*it);
		if(pItem == m_DrawState.pStartItem)
			bFoundFirstItem = true;

		if(!bFoundFirstItem)
			continue;
		
		if(pItem != m_DrawState.pStartItem)
			rcItem.left += m_nItemSpacing;

		rcItem.right = rcItem.left + pItem->nWidth;
		
		DrawCrumbItem(pItem, pDC, rcItem);

		rcItem.left += pItem->nWidth;
	}
}

void MZCrumbBar::DrawPreItem(CrumbBarItem* pPreItem, CDC* pDC, CRect rc)
{
	bool bHot = false;
	if(m_DrawState.pHotItem == pPreItem)
		bHot = true;

	if(bHot)
		pDC->SetTextColor(m_crColorHot);
	else
		pDC->SetTextColor(m_crColorDefault);

	CFont* pOldFont = NULL;

	if(bHot)
		pOldFont = pDC->SelectObject(&m_fontHot);

	pDC->DrawText(m_PreItem.DisplayName, - 1,rc, DT_LEFT| DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

void MZCrumbBar::DrawCrumbItem(CrumbBarItem* pItem, CDC* pDC, CRect rc)
{
	bool bHot = false;
	if(m_DrawState.pHotItem == pItem)
		bHot = true;

	COLORREF crTextColor = m_crColorDefault;
	if(bHot)	crTextColor = m_crColorHot;
	if (pItem->bSelected)		crTextColor = m_crColorSelected;

	pDC->SetTextColor(crTextColor);

	CFont* pOldFont = NULL;
	if(bHot)	pOldFont = pDC->SelectObject(&m_fontHot);


	pDC->DrawText(pItem->DisplayName, - 1,rc, DT_LEFT| DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_EXTERNALLEADING);

	if(pOldFont)		pDC->SelectObject(pOldFont);
}

// override to change the name to display
CString MZCrumbBar::GetDisplayName(DWORD /*nIdx*/, const CString pathPart, DWORD nFlags)
{
	CString displayName;

	if(nFlags == DM_LEFT)
	{
		displayName = m_strDelimiter;
		displayName += pathPart;
	}
	else if(nFlags == DM_RIGHT)
	{
		displayName = pathPart;
		displayName += m_strDelimiter;
	}
	else
		displayName = pathPart;

	return displayName;
}

bool MZCrumbBar::AddCrumbItem(DWORD nIdx, const CString& pathPart, const CString& displayName, DWORD_PTR dwCustomData)
{
  CrumbBarItem* pCrumbItem = new CrumbBarItem(pathPart, displayName);
  pCrumbItem->nPos = nIdx;
  pCrumbItem->dwCustomData = dwCustomData;
 
  m_vCrumbItems.push_back(pCrumbItem);  

  return true;
}

bool MZCrumbBar::BuildCrumbbar(int selidx)
{
	nsel = selidx;
	return BuildCrumbbar(Path());
}

int MZCrumbBar::FindNextCrumBarPos(const CString& path, int iStart)
{
  int nFind = path.Find(m_strDelimiter, iStart);
  
  // Check if we have a double delimiter. we should then not break the string at this position.
  if (path.GetLength () >= (nFind + 2 * m_strDelimiter.GetLength()))
  {
    if (path.Mid(nFind + m_strDelimiter.GetLength(), m_strDelimiter.GetLength()).Compare (m_strDelimiter) == 0)
      return FindNextCrumBarPos(path, nFind + 2 * m_strDelimiter.GetLength());
  }

  return nFind;
}

void MZCrumbBar::StripDoubleDelimiter(CString& pathPart)
{
  CString find = m_strDelimiter;
  find += m_strDelimiter;
  pathPart.Replace(find, m_strDelimiter);
}

bool MZCrumbBar::BuildCrumbbar(CString path)
{
  CStringArray saPath;

  int nFind = -1;

  CString pathPart;
  while (!path.IsEmpty())
  {
    nFind = FindNextCrumBarPos(path, 0);

    if (nFind < 0)
    {
      StripDoubleDelimiter(path);
      saPath.Add(path);
      break;
    }
    else
    {
      pathPart = path.Left(nFind);
      path = path.Mid(nFind + m_strDelimiter.GetLength());

      StripDoubleDelimiter(pathPart);
      if(!pathPart.IsEmpty())
        saPath.Add(pathPart);
    }

  }

  return BuildCrumbbar (saPath, TRUE, NULL);
}

bool MZCrumbBar::BuildCrumbbar(CStringArray const &saPath, BOOL bAddSeparator, CArray<DWORD_PTR, DWORD_PTR &> const *dwa)
{
	CStringArray saDisplay;
	saDisplay.SetSize (saPath.GetSize ());

	for(int n = 0; n < saPath.GetSize (); ++n)
	{
		if(!bAddSeparator || ((n == saPath.GetUpperBound()) && (m_dwStyleEx & CRU_LASTITEMNODELIMITER)))
			saDisplay[n] = saPath[n];
		else
			saDisplay[n] = GetDisplayName(n, saPath[n], m_nDelimiterMode);
	}

	return BuildCrumbbar(saPath, saDisplay, dwa);
}

bool MZCrumbBar::BuildCrumbbar(CStringArray const &saPath, CStringArray const &saDisplay, CArray<DWORD_PTR, DWORD_PTR &> const *dwa)
{
	ASSERT(saPath.GetSize () == saDisplay.GetSize ());
	if (saPath.GetSize () != saDisplay.GetSize ())
		return false;

	ASSERT(!dwa || (saPath.GetSize () == dwa->GetSize ()));
	if (dwa && (saPath.GetSize () != dwa->GetSize ()) )
		return false;

	EmptyCrumbItems();

	for(int n = 0; n < saPath.GetSize(); ++n)
	{
    DWORD_PTR dwCustomData = 0;
    if(dwa)
      dwCustomData = (*dwa)[n];

    AddCrumbItem(n, saPath[n], saDisplay[n], dwCustomData);
	}

	//set selected//
	if (nsel >= 0)
	{
		for (int i = 0; i < m_vCrumbItems.size(); i++)
		{
			if (i == nsel)
				m_vCrumbItems[i]->bSelected = 1;
			else
				m_vCrumbItems[i]->bSelected = 0;
		}
	}
	

	m_bDrawStateDirty = true;
	InvalidateRect(NULL);
	return true;
}

DWORD_PTR MZCrumbBar::GetItemData (int nItem) const
{
	if ((nItem < 0) || (nItem >= (int)m_vCrumbItems.size ()) || (m_vCrumbItems [nItem] == NULL) )
	{
		ASSERT(FALSE);
		return 0;
	}

	return m_vCrumbItems [nItem]->dwCustomData;
}
BOOL MZCrumbBar::SetItemData (int nItem, DWORD_PTR dwItem)
{
	if ( (nItem < 0) || (nItem >= (int)m_vCrumbItems.size ()) || (m_vCrumbItems [nItem] == NULL) )
	{
		ASSERT(FALSE);
		return FALSE;
	}

	m_vCrumbItems[nItem]->dwCustomData = dwItem;

	return TRUE;
}


CString MZCrumbBar::GetPath(CrumbBarItem* pItem)
{
	CString strPath;
	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{
		strPath += (*it)->Name;
		strPath += m_strDelimiter;

		if((*it) == pItem)
			break;
	}

	return strPath;
}

CFont* MZCrumbBar::FontDefault()
{
	return &m_fontDefault;
}

void MZCrumbBar::FontDefault(CFont* pVal)
{
	LOGFONT lf;
	pVal->GetLogFont(&lf);

	m_fontDefault.DeleteObject();
	m_fontDefault.CreateFontIndirect(&lf);
}

void MZCrumbBar::FontDefault(LOGFONT* lf)
{
	m_fontDefault.DeleteObject();
	m_fontDefault.CreateFontIndirect(lf);
}

CFont* MZCrumbBar::FontHot()
{
	return &m_fontHot;
}

void MZCrumbBar::FontHot(CFont* pVal)
{
	LOGFONT lf;
	pVal->GetLogFont(&lf);

	m_fontHot.DeleteObject();
	m_fontHot.CreateFontIndirect(&lf);

}

void MZCrumbBar::FontHot(LOGFONT* lf)
{
	m_fontHot.DeleteObject();
	m_fontHot.CreateFontIndirect(lf);
}

void MZCrumbBar::BackgroundColor(COLORREF crBkgnd, COLORREF crBkgndHigh, bool bGradient)
{
	m_crLoColor = crBkgnd;
	m_crHiColor = crBkgndHigh;

	m_bGradientBackground = bGradient;

	if (m_hBackBrush)
		::DeleteObject(m_hBackBrush);

	if (m_bGradientBackground == false)
	{
		m_hBackBrush = ::CreateSolidBrush(crBkgnd);
	}

	InvalidateRect(NULL);
}

void MZCrumbBar::EmptyCrumbItems()
{
	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{
		delete (*it);
	}
	m_vCrumbItems.clear();
}

MZCrumbBar::CrumbBarItem* MZCrumbBar::GetLastItem()
{
	std::vector<CrumbBarItem*>::reverse_iterator it = m_vCrumbItems.rbegin();
	if(it != m_vCrumbItems.rend())
		return (*it);

	return NULL;
}

MZCrumbBar::CrumbBarItem* MZCrumbBar::HitTestItem(const CPoint& point)
{
	bool bFoundStartItem = false;
	if(m_DrawState.pStartItem == NULL)
		bFoundStartItem = true;

	int nSize = m_nMargin;

	if(point.x < nSize)
		return NULL;

	if(m_DrawState.bPreItem && m_PreItem.nWidth > 0)
	{
		nSize += m_PreItem.nWidth;
		if(point.x < nSize)
			return GetCrumbBarPreItem();
	}

	for(std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
	{

		if(!bFoundStartItem)
		{
			if((*it) == m_DrawState.pStartItem)
				bFoundStartItem = true;
		}

		CrumbBarItem* pItem = (*it);

		if(bFoundStartItem)
		{
			int nSpacing = 0;
			if(pItem != m_DrawState.pStartItem)
				nSpacing = m_nItemSpacing;

			// nSize is the end length of previous item.
			if(nSpacing && nSize + nSpacing > point.x)
				return NULL;

			// if pos is less then the end if current item then it is a HIT
			if(nSize + nSpacing + pItem->nWidth > point.x)
				return pItem;

			nSize += nSpacing;
			nSize += pItem->nWidth;
		}

	}

	return NULL;
}

void MZCrumbBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bMouseOver)
	{
		m_bMouseOver = true;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);
		Invalidate();
	}

	CrumbBarItem* oldHotItem = m_DrawState.pHotItem;
	m_DrawState.pHotItem = HitTestItem(point);
	if(m_dwStyleEx & CRU_LASTITEMCOLD && m_DrawState.pHotItem == GetLastItem())
		m_DrawState.pHotItem = NULL;

	if(oldHotItem != m_DrawState.pHotItem)
	{
		m_bDrawStateDirty = true;
	}

	InvalidateRect(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

BOOL MZCrumbBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
LRESULT MZCrumbBar::SendNotification(CrumbBarItem* pItem, DWORD dwNotifyMsg, const CPoint& pt)
{
	if( !IsWindow(m_hWnd) )
		return 0;

	NM_CRUMBBARITEM nmItem;
	nmItem.pCrumbBarItem = pItem;
	nmItem.pt = pt;

	if(pItem)
		nmItem.strCrumbPath = GetPath(pItem);
	else
		nmItem.strCrumbPath = Path();

	nmItem.hdr.hwndFrom = m_hWnd;
	nmItem.hdr.idFrom = GetDlgCtrlID();
	nmItem.hdr.code = dwNotifyMsg;

	CWnd* pTarget = GetParent();
	if(m_pNotifyTarget)
		pTarget = m_pNotifyTarget;

	return pTarget->SendMessage(WM_NOTIFY, nmItem.hdr.idFrom, (LPARAM)&nmItem);
}
//////////////////////////////////////////////////////////////////////////
bool MZCrumbBar::EditOnClick(DWORD dwClickStyle, bool bClkOnBackground)
{
	if(m_dwStyleEx & dwClickStyle)
	{
		if(m_dwStyleEx & CRU_EDITOUTSIDE )
		{
			if(bClkOnBackground)
				return true;

			return false;
		}
		return true;
	}
	return false;
}

void MZCrumbBar::OnLButtonClick(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONCLK_L, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	if (pItem)
	{
		for (std::vector<CrumbBarItem*>::iterator it = m_vCrumbItems.begin(); it != m_vCrumbItems.end(); ++it)
		{
			CrumbBarItem* pItem1 = (*it);
			pItem1->bSelected = 0;
		}

		pItem->bSelected = 1;
	}
		

	SendNotification(pItem, NMCB_LBCLICK, pt);
}

void MZCrumbBar::OnRButtonClick(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONCLK_R, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	SendNotification(pItem, NMCB_RBCLICK, pt);
}

void MZCrumbBar::OnMButtonClick(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONCLK_M, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	SendNotification(pItem, NMCB_MBCLICK, pt);
}

void MZCrumbBar::OnLButtonDblClk(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONDBL_L, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	SendNotification(pItem, NMCB_LBDBLCLICK, pt);
}

void MZCrumbBar::OnRButtonDblClk(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONDBL_R, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	SendNotification(pItem, NMCB_RBDBLCLICK, pt);
}

void MZCrumbBar::OnMButtonDblClk(CrumbBarItem* pItem, const CPoint& pt)
{
	bool bEditLabel = EditOnClick(CRU_EDITONDBL_M, pItem == NULL );

	if(bEditLabel)
		EditLabel();

	SendNotification(pItem, NMCB_MBDBLCLICK, pt);
}
//////////////////////////////////////////////////////////////////////////
// Called from OnXButtonDblClk
// Override this and change if you have none click areas in the CrumbItems. 
// or some other click areas on them or something.
void MZCrumbBar::OnLButtonClick(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	if(pCrumbItem && m_dwStyleEx & CRU_LASTITEMNOCLK && pCrumbItem == GetLastItem())
		return;
	OnLButtonClick(pCrumbItem, pt);
}

void MZCrumbBar::OnRButtonClick(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	if(pCrumbItem && m_dwStyleEx & CRU_LASTITEMNOCLK && pCrumbItem == GetLastItem())
		return;
	OnRButtonClick(pCrumbItem, pt);
}

void MZCrumbBar::OnMButtonClick(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	if(pCrumbItem && m_dwStyleEx & CRU_LASTITEMNOCLK && pCrumbItem == GetLastItem())
		return;
	OnMButtonClick(pCrumbItem, pt);
}

void MZCrumbBar::OnLButtonDblClk(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	OnLButtonDblClk(pCrumbItem, pt);
}

void MZCrumbBar::OnRButtonDblClk(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	OnRButtonDblClk(pCrumbItem, pt);
}

void MZCrumbBar::OnMButtonDblClk(const CPoint& pt)
{
	CrumbBarItem* pCrumbItem = HitTestItem(pt);
	OnMButtonDblClk(pCrumbItem, pt);
}

//////////////////////////////////////////////////////////////////////////
// WM_xBUTTONxxxx()
void MZCrumbBar::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDblClk = true;
	CWnd::OnMButtonDblClk(nFlags, point);
}

void MZCrumbBar::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_bDblClk = false;
	CWnd::OnMButtonDown(nFlags, point);
}

void MZCrumbBar::OnMButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDblClk)
	{
		OnMButtonDblClk(point);
		m_bDblClk = false;
	}
	else
	{
		OnMButtonClick(point);
	}

	CWnd::OnMButtonUp(nFlags, point);
}

void MZCrumbBar::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDblClk = true;
	CWnd::OnRButtonDblClk(nFlags, point);
}

void MZCrumbBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bDblClk = false;
	CWnd::OnRButtonDown(nFlags, point);
}

void MZCrumbBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDblClk)
	{
		OnRButtonDblClk(point);
		m_bDblClk = false;
	}
	else
	{
		OnRButtonClick(point);
	}

	CWnd::OnRButtonUp(nFlags, point);
}


void MZCrumbBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_bDblClk = true;
	CWnd::OnLButtonDblClk(nFlags, point);
}

void MZCrumbBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bDblClk = false;
	CWnd::OnLButtonDown(nFlags, point);
}

void MZCrumbBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDblClk)
	{
		OnLButtonDblClk(point);
		m_bDblClk = false;
	}
	else
	{
		OnLButtonClick(point);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

// override this to handle context menu over specific crumbbar item
void MZCrumbBar::OnContextMenu(CrumbBarItem* /*pCrumbItem*/)
{

}

void MZCrumbBar::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CrumbBarItem* pCrumbItem = HitTestItem(point);
	if(pCrumbItem)
		OnContextMenu(pCrumbItem);
}

//////////////////////////////////////////////////////////////////////////
LRESULT MZCrumbBar::OnMouseLeave(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (m_bMouseOver)
	{
		m_bMouseOver = false;
		m_DrawState.pHotItem = NULL;
		m_bDrawStateDirty = true;
		Invalidate();
	}
	return 0;
}

CWnd* MZCrumbBar::CreateInplaceEditCtrl(const CString& text)
{
	DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_LEFT;

	CRect rc;
	GetClientRect(&rc);
	CEdit *pEdit = new MZCrumbBar_InplaceEditCtrl( text );
	pEdit->Create(dwStyle, rc, this, 0x1233);	
	pEdit->SetFont(&m_fontDefault, FALSE);
	pEdit->SetSel( 0 , -1 );
	return pEdit;
}

void MZCrumbBar::EditLabel()
{
	CreateInplaceEditCtrl(m_Path);
}

void MZCrumbBar::OnEndLabelEdit(const CString& newText)
{
	SetFocus();

	if(Path().Compare(newText) != 0)
	{
		Path(newText);
		BuildCrumbbar();

		m_bDrawStateDirty = true;
		InvalidateRect(NULL);

		CPoint pt;
		SendNotification(NULL, NMCB_EDITCHANGE, pt);
	}

}

LRESULT MZCrumbBar::OnGetFont (WPARAM, LPARAM)
{
	return (LRESULT)m_fontDefault.m_hObject;
}

void MZCrumbBar::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* lvDispInfo = (LV_DISPINFO*)pNMHDR;
	if(lvDispInfo->item.pszText)
	{
		OnEndLabelEdit(lvDispInfo->item.pszText);
	}
	else
		GetParent()->SetFocus();

	*pResult = 0;
}

void MZCrumbBar::OnSysColorChange()
{
	CWnd::OnSysColorChange();

	if (m_hwndBrush)
		::DeleteObject(m_hwndBrush);

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	InvalidateRect(NULL);
}

BOOL MZCrumbBar::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/)
{
	// Why is not the default cursor of this window used ?? If another cursor is set when entering this window
	// the cursor is not change. so force it to change
	static HCURSOR  hcArrorCursor = ::LoadCursor(NULL, IDC_ARROW);

	HCURSOR hc = GetCursor();
	if( hc != hcArrorCursor  )
		SetCursor( hcArrorCursor  );

	return TRUE;
	// return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
