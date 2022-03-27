// -----------------------------------------------------------------------------------------------------
// Hyperlink.cpp : implementation file
// -----------------------------------------------------------------------------------------------------
// Dennis Dykstra, 08 Oct 2010
// -----------------------------------------------------------------------------------------------------
// The purpose of this class is to overcome limitations in the CMFCLinkCtrl class from which it is
// derived.
// -----------------------------------------------------------------------------------------------------
// LICENSE
//      This software is licensed under the Code Project Open License (CPOL). You are free to use the
// software in any way you like, except that you may not sell the source code. The software is provided
// "as is" with no expressed or implied warranty. I accept no liability for any damage or loss of
// business that this software might cause.
//      Please note that this software code is derived from software owned and copyrighted by Microsoft®
// as part of the Microsoft Foundation Classes for Visual C++®. As such it is a derivative work that
// could be subject to more restrictive licensing policies under terms promulgated by Microsoft®.
// -----------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "Hyperlink.h"

// CHyperlink
IMPLEMENT_DYNAMIC(CHyperlink, CMFCLinkCtrl)

// -----------------------------------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------------------------------
CHyperlink::CHyperlink()
	{
	m_bHLinkShown = FALSE;		// Becomes TRUE after the control has been shown the first time.
	m_pParent = NULL;			// Becomes non-NULL after the parent window exists.
	m_bMatchParentFont = TRUE;	// Assume the user wants to initially match the parent font.
	m_nAlignStyle = ALIGN_LEFT;	// Assume the user wants to align the text on the left.

	// -------------------------------------
	// Set default colors for the hyperlink.
	// -------------------------------------
	m_colorNormalLink  = afxGlobalData.clrHotLinkNormalText;
	m_colorHoverLink   = afxGlobalData.clrHotLinkHoveredText;
	m_colorVisitedLink = afxGlobalData.clrHotLinkVisitedText;

	// ----------------------------------------------------------------------------------------
	// Initially set the default underlined GUI font as the hyperlink font. This normally uses
	// a Microsoft "logical" font, MS Shell Dlg. The first time OnDraw() is called we'll change
	// this to the parent window's font but we don't know it yet so we can't do that now.
	// ----------------------------------------------------------------------------------------
	LOGFONT lf;
	afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
	BOOL bBold = lf.lfWeight > FW_NORMAL ? TRUE : FALSE;
	SetHyperlinkFont(lf.lfFaceName, lf.lfHeight, lf.lfItalic, bBold);

	// ---------------------------------------------------------------------------------------
	// These properties are set by CMFCLinkCtrl but are shown here for documentation purposes.
	// ---------------------------------------------------------------------------------------
	//m_nFlatStyle = BUTTONSTYLE_NOBORDERS;
	//m_sizePushOffset = CSize(0, 0);
	//m_bAlwaysUnderlineText = TRUE;// Assume we always want underlining.
	//m_bMultilineText = FALSE;		// Assume single-line hyperlink text.
	}

// ----------------------------------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------------------------------
CHyperlink::~CHyperlink()
	{
	m_Font.DeleteObject();		// Delete the normal font.
	m_ULFont.DeleteObject();	// Delete the underlined font.
	}

BEGIN_MESSAGE_MAP(CHyperlink, CMFCLinkCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// ----------------------------------------------------------------------------------------------------
// CHyperlink message handlers
// ----------------------------------------------------------------------------------------------------
// What to do whenever the hyperlink control is drawn.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::OnDraw(CDC* pDC, const CRect& rect, UINT uiState)
	{
	ASSERT_VALID(pDC);
	m_pParent = GetParent();

	// ---------------------------------------------------------------------------
	// Font initialization--do this exactly once, the first time OnDraw is called.
	// ---------------------------------------------------------------------------
	if (!m_bHLinkShown)
		{
		// ----------------------------------------------------------------------------
		// If m_bMatchParentFont has been set, inherit the font from the parent window.
		// ----------------------------------------------------------------------------
		if (m_bMatchParentFont)
			{
			if (m_pParent != NULL)
				{
				LOGFONT lf;
				m_pParent->GetFont()->GetLogFont(&lf);		// Get the dialog font.

				BOOL bBold = lf.lfWeight > FW_NORMAL ? TRUE : FALSE;

				SetHyperlinkFont(			// Set the font for the hyperlink:
					lf.lfFaceName,			//   ... Name of the dialog font.
					lf.lfHeight,			//   ... Font height from the parent.
					(BOOL)lf.lfItalic,		//   ... Italics if used in parent.
					bBold);					//   ... Bold if parent has bold.
				}
			}
		m_bHLinkShown = TRUE;	// Don't do this again.
		}

	// ------------------------
	// Set the font to be used.
	// ------------------------
	CFont* pOldFont = NULL;
	if (m_bAlwaysUnderlineText || m_bHover)
		{
		if (&m_Font != NULL)
			{
			// ----------------------------------------------------------
			// Use the font set by the user with underlining added to it.
			// ----------------------------------------------------------
			if (!m_bULCreated)
				{
				CreateUnderlinedFont(&m_Font);
				}
			pOldFont = pDC->SelectObject(&m_ULFont);
			}
		else
			{
			// ------------------------------------------------------------
			// No font set by the user, so use the default underlined font.
			// ------------------------------------------------------------
			pOldFont = pDC->SelectObject(&afxGlobalData.fontDefaultGUIUnderline);
			}
		}
	else
		{
		// --------------------
		// Not using underline.
		// --------------------
		if (&m_Font != NULL)
			{
			// -----------------------------
			// Use the font set by the user.
			// -----------------------------
			pOldFont = pDC->SelectObject(&m_Font);
			}
		else
			{
			// --------------------------------------------------------
			// No font set by the user, so use the default button font.
			// --------------------------------------------------------
			pOldFont = CMFCButton::SelectFont(pDC);
			}
		}

	// -------------------------------
	// Set the hyperlink's text color.
	// -------------------------------
	pDC->SetTextColor(m_bHover ? m_colorHoverLink : (m_bVisited ? m_colorVisitedLink : m_colorNormalLink));

	// -----------------------------------------------------------------------------------
	// Set the background mode to TRANSPARENT so the background will show behind the text.
	// -----------------------------------------------------------------------------------
	pDC->SetBkMode(TRANSPARENT);

	// -----------------------------
	// Get the text for the control.
	// -----------------------------
	CString strLabel;
	GetWindowText(strLabel);

	// --------------
	// Draw the text.
	// --------------
	CRect rectText = rect;
	if (m_bMultilineText)
		{
		pDC->DrawText(strLabel, rectText, DT_WORDBREAK | DT_TOP);
		}
	else
		{
		UINT nFormat = DT_SINGLELINE | DT_WORD_ELLIPSIS | DT_VCENTER;
		switch(m_nAlignStyle)
			{
			case ALIGN_CENTER: nFormat |= DT_CENTER; break;
			case ALIGN_RIGHT:  nFormat |= DT_RIGHT;  break;
			case ALIGN_LEFT:
			default:
				nFormat |= DT_LEFT;
				break;
			}
		pDC->DrawText(strLabel, rectText, nFormat);
		}

	pDC->SelectObject(pOldFont);
	}

// ----------------------------------------------------------------------------------------------------
// Set the font to be used for the hyperlink.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::SetHyperlinkFont(LPCTSTR strFontName, long nFontHeight, BOOL bItalics, BOOL bBold)
	{
	LOGFONT lf;											// Declare a logfont object.
	memset(&lf, 0, sizeof(LOGFONT));					// Clear the logfont structure.
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, strFontName);	// Font name passed by the user.
	lf.lfHeight = nFontHeight;							// Font height passed by the user.
	lf.lfItalic = (BYTE)bItalics;						// TRUE if italics.
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;			// Set weight as either bold or normal.
	m_Font.Detach();									// Detach the old font before creating a new one.
	VERIFY(m_Font.CreateFontIndirect(&lf));				// Create the font without underlining.
	m_bULCreated = false;

	if (m_pParent != NULL)
		{
		Invalidate();
		}
	}

// ----------------------------------------------------------------------------------------------------
// Overload for creating a font of a specified point size.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::SetHyperlinkFont(LPCTSTR strFontName, int nPoints, BOOL bItalics, BOOL bBold)
	{
	LOGFONT lf;											// Declare a logfont object.
	memset(&lf, 0, sizeof(LOGFONT));					// Clear the logfont structure.
	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, strFontName);	// Font name passed by the user.
	lf.lfHeight = nPoints * 10;							// lfHeight is in 10ths of points.
	lf.lfItalic = (BYTE)bItalics;						// TRUE if italics.
	lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;			// Set weight for bold or normal.
	CClientDC dc(this);									// Get a device context for the hyperlink.
	m_Font.Detach();									// Detach the old font before creating a new one.
	VERIFY(m_Font.CreatePointFontIndirect(&lf, &dc));	// Create the font without underlining.
	m_bULCreated = false;

	if (m_pParent != NULL)
		{
		Invalidate();
		}
	}

// ----------------------------------------------------------------------------------------------------
// Get the current hyperlink font's height in points.
// ----------------------------------------------------------------------------------------------------
int CHyperlink::GetHyperlinkFontPoints()
	{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	if (&m_Font != NULL)
		{
		m_Font.GetLogFont(&lf);
		}
	else
		{
		afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
		}
	CDC* pDC = this->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	int nPoints = abs(MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY)));
	return nPoints;
	}

// ----------------------------------------------------------------------------------------------------
// Get the name of the current hyperlink font.
// ----------------------------------------------------------------------------------------------------
CString CHyperlink::GetHyperlinkFontName()
	{
	CString strLogName;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	if (&m_Font != NULL)
		{
		m_Font.GetLogFont(&lf);
		}
	else
		{
		afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
		}
	strLogName = lf.lfFaceName;
	if ((strLogName == _T("MS Shell Dlg")) ||
		(strLogName == _T("MS Shell Dlg 2")))
		{
		// -------------------------------------------------
		// The font is one of Microsoft's two logical fonts,
		// so get the actual font name from the registry.
		// -------------------------------------------------
		CRegKey regKey;
		CString strKey =
			_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\FontSubstitutes\\");
		if (regKey.Open(HKEY_LOCAL_MACHINE, strKey, KEY_QUERY_VALUE) == ERROR_SUCCESS)
			{
			TCHAR szName[256];		// Should be plenty long enough to hold the name.
			ULONG nChars = 256;		// The actual number of chars will be returned.
			if (regKey.QueryStringValue((LPCTSTR)strLogName, szName, &nChars) == ERROR_SUCCESS)
				{
				int nLen = _tcslen(szName);
				// -----------------------------------------------------------------
				// Make sure the number of chars returned matches the string length.
				// -----------------------------------------------------------------
				if (nChars == (ULONG)(nLen + 1))
					{
					regKey.Close();
					return szName;
					}
				// ----------------------------------------------------------------------
				// Wrong number of chars returned; drop through to avoid security issues.
				// ----------------------------------------------------------------------
				}
			// --------------------------------
			// Couldn't query the string value.
			// --------------------------------
			}
		// -------------------------------
		// Couldn't open the registry key.
		// -------------------------------
		regKey.Close();
		}
	// ------------------------------------------------------------------------------------
	// Not a logical font, or we couldn't get the substitute, so just return the font name.
	// ------------------------------------------------------------------------------------
	return strLogName;
	}

// ----------------------------------------------------------------------------------------------------
// Return an indication of whether the current font is italic.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::IsHyperlinkFontItalic()
	{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	if (&m_Font != NULL)
		{
		m_Font.GetLogFont(&lf);
		}
	else
		{
		afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
		}
	return (BOOL)lf.lfItalic;
	}

// ----------------------------------------------------------------------------------------------------
// Return an indication of whether the current font is bold.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::IsHyperlinkFontBold()
	{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	if (&m_Font != NULL)
		{
		m_Font.GetLogFont(&lf);
		}
	else
		{
		afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
		}
	if (lf.lfWeight >= FW_BOLD)
		return TRUE;
	else
		return FALSE;
	}

// ----------------------------------------------------------------------------------------------------
// Set the button style for the hyperlink.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::SetHyperlinkStyle(HyperlinkStyle enumValue)
	{
	if (enumValue == ThreeDButton)
		{
		CMFCButton::EnableWindowsTheming(TRUE);
		m_nFlatStyle = BUTTONSTYLE_3D;
		Invalidate();
		}
	else if (enumValue == FlatButton)
		{
		CMFCButton::EnableWindowsTheming(FALSE);
		m_nFlatStyle = BUTTONSTYLE_FLAT;
		Invalidate();
		}
	else if (enumValue == TextOnly)
		{
		CMFCButton::EnableWindowsTheming(TRUE);
		m_nFlatStyle = BUTTONSTYLE_NOBORDERS;
		Invalidate();
		}
	else if (enumValue == SemiFlatButton)
		{
		CMFCButton::EnableWindowsTheming(FALSE);
		m_nFlatStyle = BUTTONSTYLE_SEMIFLAT;
		Invalidate();
		}
	else
		{
		return FALSE;
		}

	return TRUE;
	}

// ----------------------------------------------------------------------------------------------------
// Set the alignment for the hyperlink.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::SetTextAlignment(HyperlinkAlignment enumValue)
	{
	if (enumValue == Left)
		{
		m_nAlignStyle = ALIGN_LEFT;
		SizeToContent(TRUE, FALSE);
		Invalidate();
		}
	else if (enumValue == Right)
		{
		m_nAlignStyle = ALIGN_RIGHT;
		SizeToContent(TRUE, FALSE);
		Invalidate();
		}
	else if (enumValue == Center)
		{
		m_nAlignStyle = ALIGN_CENTER;
		SizeToContent(TRUE, TRUE);
		Invalidate();
		}
	else
		{
		return FALSE;
		}

	return TRUE;
	}

// ----------------------------------------------------------------------------------------------------
// Get a value indicating whether to match the font of the parent window.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::GetMatchParentFont()
	{
	return m_bMatchParentFont;
	}

// ----------------------------------------------------------------------------------------------------
// Set a value indicating whether to match the font of the parent window.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::SetMatchParentFont(BOOL bMatch)
	{
	m_bMatchParentFont = bMatch;
	}

// ----------------------------------------------------------------------------------------------------
// Change the colors used with the hyperlink font.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::SetHyperlinkColors(COLORREF colorNormal, COLORREF colorHover, COLORREF colorVisited)
	{
	m_colorNormalLink  = colorNormal;
	m_colorHoverLink   = colorHover;
	m_colorVisitedLink = colorVisited;
	}

// ----------------------------------------------------------------------------------------------------
// Set a flag to indicate whether or not the hyperlink should always be underlined.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::UnderlineHyperlink(BOOL bUnderline)
	{
	m_bAlwaysUnderlineText = bUnderline;
	}

// ----------------------------------------------------------------------------------------------------
// Set the Windows default underlined GUI font as the current hyperlink font.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::SetHyperlinkFontToDefault(void)
	{
	LOGFONT lf;
	afxGlobalData.fontDefaultGUIUnderline.GetLogFont(&lf);
	BOOL bBold = lf.lfWeight > FW_NORMAL ? TRUE : FALSE;
	SetHyperlinkFont(lf.lfFaceName, lf.lfHeight, lf.lfItalic, bBold);
	}

// ----------------------------------------------------------------------------------------------------
// Add underlining to the font.
// ----------------------------------------------------------------------------------------------------
void CHyperlink::CreateUnderlinedFont(CFont* pFont)
	{
	LOGFONT lf;									// Declare a logfont object.
	memset(&lf, 0, sizeof(LOGFONT));			// Clear the logfont structure.
	pFont->GetLogFont(&lf);						// Fill the logfont with data.
	lf.lfUnderline = TRUE;						// Add underlining.
	m_ULFont.Detach();							// Detach the old font before creating a new one.
	VERIFY(m_ULFont.CreateFontIndirect(&lf));	// Create the underlined font.
	m_bULCreated = true;

	if (m_pParent != NULL)
		{
		Invalidate();
		}
	}

// ----------------------------------------------------------------------------------------------------
// Size the hyperlink focus rectangle to the text of the hyperlink.
// ----------------------------------------------------------------------------------------------------
CSize CHyperlink::SizeToContent(BOOL bVCenter, BOOL bHCenter)
	{
	// ----------------------------------------------------------------------
	// If the content is an image, just pass the job to the CMFCButton class.
	// ----------------------------------------------------------------------
	if (m_sizeImage != CSize(0,0))
		{
		return CMFCButton::SizeToContent();
		}

	// ----------------------------------------------------------------
	// If multiline text has been enabled, do as CMFCLinkCtrl would do.
	// ----------------------------------------------------------------
	if (m_bMultilineText)
		{
		return CMFCLinkCtrl::SizeToContent(bVCenter, bHCenter);
		}

	// -------------------------------------------------------------
	// The content is single-line text so we need to do the resizing
	// ourselves in order to take account of the current font.
	// -------------------------------------------------------------

	ASSERT_VALID(this);
	ENSURE(GetSafeHwnd() != NULL);

	// -------------------------------------------------
	// Set the font to be used in getting the text size.
	// -------------------------------------------------
	CClientDC dc(this);
	CFont* pOldFont;
	if (&m_Font != NULL)
		{
		pOldFont = dc.SelectObject(&m_Font);
		}
	else
		{
		pOldFont = dc.SelectObject(&afxGlobalData.fontDefaultGUIUnderline);
		}
	ENSURE(pOldFont != NULL);

	// -----------------------
	// Get the hyperlink text.
	// -----------------------
	CString strLink;
	GetWindowText(strLink);

	// -----------------------------------------------------------------------------
	// Calculate the space for the hyperlink and inflate it for the focus rectangle.
	// -----------------------------------------------------------------------------
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectText = rectClient;
	dc.DrawText(strLink, rectText, DT_SINGLELINE | DT_CALCRECT);
	rectText.InflateRect(6, 6, 6, 6);

	if (bVCenter || bHCenter)
		{
		// ------------------------------------------------
		// Adjust for centering within the focus rectangle.
		// ------------------------------------------------
		ASSERT(GetParent()->GetSafeHwnd() != NULL);	// Get the parent window.
		MapWindowPoints(GetParent(), rectClient);	// Map the text rectangle to the parent window.

		// ---------------------------------------------------
		// Calculate x and y offsets to account for centering.
		// ---------------------------------------------------
		int dx = bHCenter ? (rectClient.Width()  - rectText.Width())  / 2 : 0;
		int dy = bVCenter ? (rectClient.Height() - rectText.Height()) / 2 : 0;

		// ------------------------------------------
		// Set the new size of the drawing rectangle.
		// ------------------------------------------
		SetWindowPos(NULL, rectClient.left + dx, rectClient.top + dy, rectText.Width(), rectText.Height(),
					 SWP_NOZORDER | SWP_NOACTIVATE);
		}
	else
		{
		// -------------------------------------------------------------------------------
		// No centering, so just set the size of the drawing rectangle as the text extent.
		// -------------------------------------------------------------------------------
		SetWindowPos(NULL, -1, -1, rectText.Width(), rectText.Height(),
					 SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}

	dc.SelectObject(pOldFont);
	return rectText.Size();
	}

// ----------------------------------------------------------------------------------------------------
// ACCESSIBILITY: Implementation to enable pressing the spacebar as a way to 'click' the hyperlink.
// ----------------------------------------------------------------------------------------------------
BOOL CHyperlink::PreTranslateMessage(MSG* pMsg)
	{
	if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
		if (pMsg->wParam == VK_SPACE)
			{
			// -------------------------------------------------------------------
			// Just convert VK_SPACE to VK_RETURN, since the handler for VK_RETURN
			// in CMFCLinkCtrl does exactly what we want.
			// -------------------------------------------------------------------
			pMsg->wParam = VK_RETURN;
			// ----------------------------------------------
			// We could alternatively have done this instead:
			// ----------------------------------------------
			// SendMessage(WM_COMMAND, MAKEWPARAM(this->GetDlgCtrlID(), BN_CLICKED), (LPARAM)this->m_hWnd);
			// return TRUE;
			}
		}

	return CMFCLinkCtrl::PreTranslateMessage(pMsg);
	}


void CHyperlink::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_strURL == L"option")
	{
		GetParent()->PostMessageW(UM_SHOW_STUB_OPTION);
		return;
	}
	else if (m_strURL == L"license")
	{
		GetParent()->PostMessageW(UM_SHOW_STUB_LICENSE);
		return;
	}

	CMFCLinkCtrl::OnLButtonDown(nFlags, point);
}
