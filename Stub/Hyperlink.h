// -----------------------------------------------------------------------------------------------------
// Header file for the CHyperlink class, derived from CMFCLinkCtrl.
// -----------------------------------------------------------------------------------------------------
// Dennis Dykstra, 08 Oct 2010
// -----------------------------------------------------------------------------------------------------
#pragma once

#include <afxlinkctrl.h>

#define  UM_SHOW_STUB_OPTION	WM_USER + 0x01
#define  UM_SHOW_STUB_LICENSE	WM_USER + 0x02

// CHyperlink

class CHyperlink : public CMFCLinkCtrl
	{
	DECLARE_DYNAMIC(CHyperlink)

public:
	CHyperlink();
	virtual ~CHyperlink();

protected:
	DECLARE_MESSAGE_MAP()

private:
	// Member variables
	CWnd*	 m_pParent;				// Pointer to the parent window.
	CFont	 m_Font;				// Font without underlining.
	CFont	 m_ULFont;				// Same font with underlining.
	BOOL	 m_bHLinkShown;			// TRUE after the hyperlink has been shown once.
	BOOL	 m_bULCreated;			// TRUE after the underlined font has been created.
	BOOL	 m_bMatchParentFont;	// TRUE to use the same font as the parent window.
	COLORREF m_colorNormalLink;		// Foreground color of a normal, previously unvisited link.
	COLORREF m_colorHoverLink;		// Foreground color of a link when the mouse is hovering over it.
	COLORREF m_colorVisitedLink;	// Foreground color of a link that has previously been visited.

	// Private methods
	void	CreateUnderlinedFont(CFont* pFont);
	virtual void OnDraw(CDC* pDC, const CRect& rect, UINT uiState);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	// Public enumerations
	enum HyperlinkStyle
		{
		ThreeDButton,
		FlatButton,
		TextOnly,
		SemiFlatButton
		};

	enum HyperlinkAlignment
		{
		Left,
		Right,
		Center
		};

	// Public methods
	void	SetHyperlinkFont(LPCTSTR strFontName, long nFontHeight, BOOL bItalics = FALSE, BOOL bBold = FALSE);
	void	SetHyperlinkFont(LPCTSTR strFontName, int nPoints, BOOL bItalics = FALSE, BOOL bBold = FALSE);
	void	SetHyperlinkColors(COLORREF colorNormalLink, COLORREF colorHoverLink, COLORREF colorVisitedLink);
	CString GetHyperlinkFontName(void);
	int		GetHyperlinkFontPoints(void);
	BOOL	SetHyperlinkStyle(HyperlinkStyle enumValue);
	BOOL	SetTextAlignment(HyperlinkAlignment enumValue);
	BOOL	IsHyperlinkFontItalic(void);
	BOOL	IsHyperlinkFontBold(void);
	BOOL	GetMatchParentFont(void);
	void	SetMatchParentFont(BOOL);
	void	UnderlineHyperlink(BOOL bUnderline = TRUE);
	void	SetHyperlinkFontToDefault(void);

	// Override of public method inherited from CMFCColorButton
	virtual CSize SizeToContent(BOOL bVCenter = FALSE, BOOL bHCenter = FALSE);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

