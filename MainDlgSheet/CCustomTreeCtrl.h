#if !defined(AFX_CCUSTOMTREECTRL_H__188A2C09_D1BE_42B3_8353_267E9307B3C5__INCLUDED_)
#define AFX_CCUSTOMTREECTRL_H__188A2C09_D1BE_42B3_8353_267E9307B3C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCustomTreeCtrl.h : header file
//


#define COMPANY			1
#define PROJ			2
#define PROJMANAGER		3
#define PROJLEAD		4
#define PROJMEMBER		5
#define TRAINEE			6

class CCustomTreeCtrl;						 

typedef struct __LOOPINFO
{
	CCustomTreeCtrl* pTree;
	HTREEITEM hItem;
	CWnd*	  pParent;
	void*	  param;	
}LOOPINFO,*PLOOPINFO;

typedef void (*ScanCallBackFunc) (LOOPINFO* pLoopInfo );
void FindItemsInRect( LOOPINFO* pLoopInfo );
void SelectItems( LOOPINFO* pLoopInfo );

//*************************************************************************
typedef struct __CUSTOMITEMDATA
{
	__CUSTOMITEMDATA()
	{
		m_iNormalImage = -1;
		m_iDragImage = -1;
		m_iExpandedImage = -1;
		m_iState = 0;
		m_bEditable = true;
	}

	int m_iNormalImage;		//Image given when the item is in normal state
	int	m_iDragImage;		//Image given when the item is dragged
	int m_iExpandedImage;	//Image Given when the item is expanded
	int m_iState;			//COMPANY/PROJECT/MANAGER/LEAD/MEMBER
	bool m_bEditable;

}CUSTOMITEMDATA,*PCUSTOMITEMDATA;
//*************************************************************************
typedef struct __CUSTOMINSERTSTRUCT
{
	__CUSTOMINSERTSTRUCT()
	{
		m_pCustomData = NULL;
	}

	TVINSERTSTRUCT		m_tvIS;
	PCUSTOMITEMDATA		m_pCustomData;
	
}CUSTOMINSERTSTRUCT,*PCUSTOMINSERTSTRUCT;
//*************************************************************************

/////////////////////////////////////////////////////////////////////////////
// CCustomTreeCtrl window

class CCustomTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CCustomTreeCtrl();

	HTREEITEM m_hSelectedItem,m_hPrevItem,m_hDragSourceItem,m_hDragTargetItem;
	
	bool	  m_bExpandItem;
	bool	  m_bExpandFlyBy;
	bool      m_bIsDragging;
	bool      m_bIsBandingON;
	CPoint    m_startPt,m_endPt;

	HCURSOR   m_defaultCursor,m_noCursor;

	CArray<HTREEITEM,HTREEITEM>	m_aItemsInRect;
	CArray<HTREEITEM,HTREEITEM>	m_aSelectedItems;
	bool	m_bOldItemSelected;

	CImageList* m_pDragImageList;


// Attributes
public:
		//To add the line to linking the child with its parent.
		//If flag true means line is added,  otherwise removed
		void AddLineStyle(bool flag = true);

		//To add the line to linking the child with its root.
		//If flag true means line is added,  otherwise removed
		void AddLineAtRoot( bool flag = true );

		//To add the button [ +/ - ] to show the expanded or collpased state;
		//If flag true means button is added,  otherwise removed
		void AddButton( bool flag = true );

		//To allow the editing of the label of an item
		//If flag true editiing is enabled,  otherwise disabled
		void EnableEditLabel( bool flag = true );

		//To set the background color for the tree control
		void SetBackgroundColor( COLORREF bkColor );	//[in]	-	New background color to be set

		//To set the line color
		void SetLineColor(COLORREF lineColor);	//[in]	-	New line color to be set

		//To Show the item information
		void ShowItemInfo(HTREEITEM hItem );

		//To set the bold font for the label
		void SetBoldFont( HTREEITEM hItem );	//[in]	-	Item to which the font to be set as bold

		//To expand the entire tree
		void ExpandTree(HTREEITEM hItem,bool Expand = true);	//[in]	- Item to be expanded

		//Set Expand flag
		void SetExpand( bool flag ) { m_bExpandItem = flag; }

		//Set Expand Flyby
		void SetExpandFlyby( bool flag ) { m_bExpandFlyBy = flag; }

		//To insert a item 
		HTREEITEM InsertItem(PCUSTOMINSERTSTRUCT lpInsertStruct );
		HTREEITEM InsertItem(CString strText, int nImg, int nSelectedImg, HTREEITEM hParent = NULL, HTREEITEM hAfterItem = NULL);
		
		//Copy item to another parent
		void CopyItem(HTREEITEM hSourceItem,HTREEITEM hTargetItem );

		//To a delete an item recurisvely
		void DeleteItem( HTREEITEM hTreeItem );

		//To iterate all the items and call the callback function for each item
		void IterateItems(  ScanCallBackFunc func,		//[in]	-	Call back function to be called on each item
							HTREEITEM hStart = NULL,	//[in]	-	Start node for the iteration
							HTREEITEM hEnd = NULL,		//[in]	-	End node to stop the iteration
							void* pInfo = NULL			//[in]	-	User parameter
						  );		

		//To iterate all the items and call the callback function for each item
		void ScanItems(		ScanCallBackFunc func,		//[in]	-	Call back function to be called on each item
							HTREEITEM hStart = NULL,	//[in]	-	Start node for the iteration
							HTREEITEM hEnd = NULL,		//[in]	-	End node to stop the iteration
							void* pInfo = NULL			//[in]	-	User parameter
					   );

		//To Draw the banding rectangle
		void	DrawBandingRect();

		//To invert the rectangle being drawn
		void	InvertRectangle( CDC* pDC,			//[in]	-	Device context used for drawing
								 CPoint startPt,	//[in]	-	Start point of the rectangle
								 CPoint endPt );	//[in]	-	End point of the rectangle

		//To find the items inside the given rectangle
		bool	ItemsInRect(CRect rect);	//[in]	-	Bounding rectangle

		//To get the list of selected items
		void	CollectSelectedItems();

		//To get the first selected item
		HTREEITEM CCustomTreeCtrl::GetFirstSelectedItem() const ;

		//To get the next selected item
		HTREEITEM CCustomTreeCtrl::GetNextSelectedItem(HTREEITEM hItem) const ;

		//To generate the imagelist for the dragging
		CImageList* GetImageListForDrag();


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickLButton(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	friend void FindItemsInRect( HTREEITEM hItem,CWnd* pParent );

private:
	CArray<HTREEITEM, HTREEITEM> m_aOldItems;
	HTREEITEM m_hFirstSelectedItem;
	HTREEITEM m_hLastSelectedItem;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCUSTOMTREECTRL_H__188A2C09_D1BE_42B3_8353_267E9307B3C5__INCLUDED_)
