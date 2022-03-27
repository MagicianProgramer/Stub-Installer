/* MZCrumbBar.h

MZCrumbBar Ctrl - Created by Mathias Svensson (c) Copyright 2009-2010 - http://result42.com

See top of MZCrumbBar.cpp for more information and History


License. 
--------
You are allowed to include this source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. If you fix any bug or add enhachment please send
the changes to the author so they may be included in the source.

*/

#pragma once

#include <vector>

//////////////////////////////////////////////////////////////////////////
// Draw styles
//
#define CRU_EDITOUTSIDE   0x00000010  // Click must be outside of crumb items.
#define CRU_EDITONCLK_L   0x00000100  // Enter Edit mode on Left Click
#define CRU_EDITONDBL_L   0x00000200  // Enter edit mode on Left DblClick
#define CRU_EDITONCLK_R   0x00000400  // Enter Edit mode on Right Click
#define CRU_EDITONDBL_R   0x00000800  // Enter edit mode on Right DblClick
#define CRU_EDITONCLK_M   0x00001000  // Enter Edit mode on Middle Click
#define CRU_EDITONDBL_M   0x00002000  // Enter edit mode on Middle DblClick
#define CRU_LASTITEMCOLD  0x00010000  // Show no hover action for the last item
#define CRU_LASTITEMNOCLK 0x00020000  // Do not generate click action for the last item
#define CRU_LASTITEMNODELIMITER 0x00040000  // Do not draw a delimiter for the last item

//////////////////////////////////////////////////////////////////////////
// Flags for DelimiterMode(...)
// 
#define DM_STRIP    0 // Strip the delimiter char
#define DM_LEFT     1 // Show Delimiter in the Left part 
#define DM_RIGHT    2 // Show Delimiter in the right part 

//////////////////////////////////////////////////////////////////////////
// Notification messages
#define NMCB_LBCLICK        1000	// User clicked with Left Mouse Button
#define NMCB_RBCLICK        1001  // User clicked with Right Mouse Button
#define NMCB_MBCLICK        1002  // User clicked with Middle Mouse Button
#define NMCB_LBDBLCLICK     1003  // User double clicked with Left Mouse Button
#define NMCB_RBDBLCLICK     1004  // User double clicked with Right Mouse Button
#define NMCB_MBDBLCLICK     1005  // User double clicked with Middle Mouse Button
#define NMCB_EDITCHANGE     1010  // User changed path by editing it

//////////////////////////////////////////////////////////////////////////
/*

Example on how to using the Notification..
-------------------------------

// In the .cpp file for the dialog

BEGIN_MESSAGE_MAP(CCrumbBarDemoDlg, CDialog)
ON_NOTIFY(NMCB_LBCLICK	  , IDC_CRUMBBAR, OnCrumbBarItemLeftClick )
ON_NOTIFY(NMCB_RBCLICK	  , IDC_CRUMBBAR, OnCrumbBarItemRightClick )
ON_NOTIFY(NMCB_MBCLICK	  , IDC_CRUMBBAR, OnCrumbBarItemMiddleClick )
ON_NOTIFY(NMCB_LBDBLCLICK	, IDC_CRUMBBAR, OnCrumbBarItemLeftDubbleClick )
ON_NOTIFY(NMCB_RBDBLCLICK	, IDC_CRUMBBAR, OnCrumbBarItemRightDubbleClick )
ON_NOTIFY(NMCB_MBDBLCLICK	, IDC_CRUMBBAR, OnCrumbBarItemMiddleDubbleClick )
ON_NOTIFY(NMCB_EDITCHANGE	, IDC_CRUMBBAR, OnCrumbBarEditChange )
END_MESSAGE_MAP()

void CCrumbBarDemoDlg::OnCrumbBarItemLeftClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarItemRightClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarItemMiddleClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarItemLeftDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarItemRightDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarItemMiddleDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}
void CCrumbBarDemoDlg::OnCrumbBarEditChange(NMHDR *pNotifyStruct, LRESULT* pResult)
{
}

/// .h file
afx_msg void OnCrumbBarItemLeftClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarItemRightClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarItemMiddleClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarItemLeftDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarItemRightDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarItemMiddleDubbleClick(NMHDR *pNotifyStruct, LRESULT* pResult);
afx_msg void OnCrumbBarEditChange(NMHDR *pNotifyStruct, LRESULT* pResult);
*/
//////////////////////////////////////////////////////////////////////////
class MZCrumbBar : public CWnd
{
	DECLARE_DYNAMIC(MZCrumbBar)

public:
	class CrumbBarItem
	{
	public:
		CrumbBarItem()
			: nWidth(-1)
			, dwCustomData(0)
			, nPos(-1)
			, bSelected(0)
		{}

		CrumbBarItem(const CString& s, const CString& d) 
			: Name(s)
			, DisplayName(d)
			, nWidth(-1)
			, dwCustomData(0)
			, nPos(-1)
			, bSelected(0)
		{}

		// Both Name and Display name must be set even if they are the same.
		CString Name;         // path part Without the delimiter.  ( eg: "C:" )
		CString DisplayName;  // DisplayName			( Eg "System Drive (C:)" )

		int     nWidth;   // Width of the item. Set by PrepareDraw when calculating. 
		int     nPos;

		DWORD_PTR	dwCustomData;	// Use to store custom parameters of your choice

		int		bSelected;
	};

public:
	MZCrumbBar();
	virtual ~MZCrumbBar();

	virtual BOOL Create(DWORD dwStyle, DWORD dwExStyle , const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	DWORD Style() const { return m_dwStyleEx; }
	void Style(DWORD val) { m_dwStyleEx = val; }

	CString Path() const  { return m_Path; }
	void Path(CString val) { m_Path = val; }

	CString Delimiter() const { return m_strDelimiter; }
	void Delimiter(CString val) { m_strDelimiter = val; }

	int DelimiterMode() const { return m_nDelimiterMode; }
	void DelimiterMode(int val) { m_nDelimiterMode = val; }

  int FindNextCrumBarPos(const CString& path, int iStart = 0);
  void StripDoubleDelimiter(CString& pathPart);

	bool BuildCrumbbar(int selidx = -1);
	int  nsel;
	// Method added by Iain - essentially 3 arrays that make up the necessary bits of the crumb vectors - two strings and option dwCustomData
	bool BuildCrumbbar(CStringArray const &saPath, CStringArray const &saDisplay, CArray<DWORD_PTR, DWORD_PTR &> const *dwa = NULL);// IAIN
	// And if you only need one set of strings...  bAddSeparator uses the DM_STRIP etc functionality
	bool BuildCrumbbar(CStringArray const &sa, BOOL bAddSeparator, CArray<DWORD_PTR, DWORD_PTR &> const *dwa = NULL);
	// Lastly, a cut-out-path-function version
	bool BuildCrumbbar(CString path);

	// Access the dwCustomData
	DWORD_PTR GetItemData (int nItem) const;
	// Set the dwCustomData
	BOOL SetItemData (int nItem, DWORD_PTR dwItem);

	CString GetPath(CrumbBarItem* pItem);

	CFont* FontDefault();
	void FontDefault(CFont* pVal);
	void FontDefault(LOGFONT* lf);

	CFont* FontHot();
	void FontHot(CFont* pVal);
	void FontHot(LOGFONT* lf);

	COLORREF ColorDefault() const { return m_crColorDefault; }
	void ColorDefault(COLORREF val) { m_crColorDefault = val; }

	COLORREF ColorHot() const { return m_crColorHot; }
	void ColorHot(COLORREF val) { m_crColorHot = val; }

	void ColorSelected(COLORREF val) { m_crColorSelected = val; }

	void BackgroundColor(COLORREF crBkgnd, COLORREF crBkgndHigh = (COLORREF)-1, bool bGradient = false);

	int Margin() const { return m_nMargin; }
	void Margin(int val) { m_nMargin = val; }

	int ItemSpacing() const { return m_nItemSpacing; }
	void ItemSpacing(int val) { m_nItemSpacing = val; }

	void EditLabel();

	void PreItem(MZCrumbBar::CrumbBarItem val) { m_PreItem = val; }

protected:

	virtual BOOL Create( DWORD dwExStyle = 0 );

	void PrepareDraw(CDC* pDC, const CRect& rcClient);
	void Draw(CDC* pDC);

	//void DrawGradientBackground(CDC* pDC, const CRect& rc, COLORREF cr1, COLORREF cr2);
	void DrawGradientBackground(CDC* pDC, const CRect& rc, COLORREF crStart, COLORREF crEnd, int nSegments);
	void DrawPreItem(CDC* pDC, CRect& rcItem);
	void DrawCrumbItems(CDC* pDC, CRect rc);

	//////////////////////////////////////////////////////////////////////////
	void EmptyCrumbItems();

  virtual bool AddCrumbItem(DWORD nIdx,  const CString& pathPart, const CString& displayName, DWORD_PTR dwCustomData);

	virtual CString GetDisplayName(DWORD nIdx, const CString pathPart, DWORD nFlags);
	//////////////////////////////////////////////////////////////////////////
	// Override to change UI

	// Return width of item.
	virtual int OnMeasureItem(CDC* pDC, CrumbBarItem* pItem);
	// Return width if the PreItem that is shown if not all items fits in the controller
	virtual int OnMeasurePreItem(CDC* pDC);

	// draw background of the entire area
	virtual void DrawBackground(CDC* pDC, const CRect& rc);

	// Draw Crumb Item
	virtual void DrawCrumbItem(CrumbBarItem* pItem, CDC* pDC, CRect rc);
	virtual void DrawPreItem(CrumbBarItem* pItem, CDC* pDC, CRect rc);

	// Override to handle if doing special custom Drawing 
	virtual void OnLButtonClick(const CPoint& pt);
	virtual void OnRButtonClick(const CPoint& pt);
	virtual void OnMButtonClick(const CPoint& pt);
	virtual void OnLButtonDblClk(const CPoint& pt);
	virtual void OnRButtonDblClk(const CPoint& pt);
	virtual void OnMButtonDblClk(const CPoint& pt);

	//
	// Override do handle click action. Default a message it sent to parent
	//
	virtual void OnLButtonClick(CrumbBarItem* pItem, const CPoint& pt);
	virtual void OnRButtonClick(CrumbBarItem* pItem, const CPoint& pt);
	virtual void OnMButtonClick(CrumbBarItem* pItem, const CPoint& pt);
	virtual void OnLButtonDblClk(CrumbBarItem* pItem, const CPoint& pt);
	virtual void OnRButtonDblClk(CrumbBarItem* pItem, const CPoint& pt);
	virtual void OnMButtonDblClk(CrumbBarItem* pItem, const CPoint& pt);

	virtual void OnContextMenu(CrumbBarItem* pCrumbItem);
	//////////////////////////////////////////////////////////////////////////

	// Override for custom notification
	virtual LRESULT SendNotification(CrumbBarItem* pItem, DWORD dwNotifyMsg, const CPoint& pt);

	// messages
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnSysColorChange();

	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult); 

	afx_msg LRESULT OnGetFont(WPARAM wParam, LPARAM lParam);

	// Override this for handling editing of crumb-path
	virtual void OnEndLabelEdit(const CString& newText);


	// overrides
	virtual void PreSubclassWindow();

	CrumbBarItem* HitTestItem(const CPoint& point);
	bool GetPath(CString& pre, CString& hot, CString& tail, CrumbBarItem* pStartItem);
	CrumbBarItem* FindFirstValidItem(int nMaxWidth);
	CrumbBarItem* GetCrumbBarPreItem();
	CrumbBarItem* GetLastItem();
	CWnd* CreateInplaceEditCtrl(const CString& text);
	bool EditOnClick(DWORD dwClickStyle, bool bClkIsOutside);


	class CrumbDrawState
	{
	public:
		void Clear()
		{
			pStartItem = NULL;
			pHotItem = NULL;
			bPreItem = false;
		}

		CrumbBarItem* pStartItem;	// Item to start drawing from
		CrumbBarItem* pHotItem;	  // Current HOT ("Hover over") item

		bool					bPreItem;		// Drawing a PreItem is needed.
	};

	// variables
	bool    m_bCreateForSubclassing;
	CString m_Path;
	CString m_strDelimiter;
	int     m_nDelimiterMode;

	CrumbBarItem	m_PreItem;	// PreItem Name,Width and more..

	DWORD   m_dwStyleEx;
	bool    m_bGradientBackground;

	CFont   m_fontDefault;
	CFont   m_fontHot;
	HBRUSH	m_hBackBrush;
	HBRUSH	m_hwndBrush;

	COLORREF  m_crColorDefault;
	COLORREF  m_crColorHot;
	COLORREF  m_crLoColor;
	COLORREF  m_crHiColor; 
	COLORREF  m_crColorSelected;

	int m_nMargin;      // Left margin
	int m_nItemSpacing; // Spacing between items

	std::vector<CrumbBarItem*>  m_vCrumbItems;

	bool		m_bMouseOver;			// use to keep track if we have register event for MouseLeave when we got a MouseOver

	bool     m_bDrawStateDirty;   // if true then a call to PrepareDraw to recalculate items is needed
	CrumbDrawState  m_DrawState;

	CWnd*   m_pNotifyTarget;	// Alternative CWnd to send notifications to instead of GetParent();
	bool  m_bDblClk;
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

//////////////////////////////////////////////////////////////////////////
// Notify data that is send from SendNotification
typedef struct tagNM_CRUMBBARITEM 
{
	NMHDR hdr;
	MZCrumbBar::CrumbBarItem* pCrumbBarItem;
	CString strCrumbPath;
	CPoint pt;
} NM_CRUMBBARITEM;

