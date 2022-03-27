// CCustomTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CCustomTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//BOOL SaveBitmap(HDC hDC,HBITMAP hBitmap,CString szPath);

//***************************************************************************
//This is a callback function called for each item when the items are iterated from a given start
//item to end item.
void FindItemsInRect( LOOPINFO* pLoopInfo )
{
	if( !pLoopInfo->hItem )
		return;

	if( !pLoopInfo->pParent )
		return;

	CCustomTreeCtrl* pCustomTree = pLoopInfo->pTree;

	//Get the rectangle which is passed as the user parameter
	CRect* rc = (CRect*)(pLoopInfo->param);
	
	//Get the rect of the item
	CRect itemRect;
	pCustomTree->GetItemRect( pLoopInfo->hItem ,&itemRect,true);

	CString str = pCustomTree->GetItemText( pLoopInfo->hItem );
	pCustomTree->ClientToScreen( &itemRect );

	CRect resRect;

	bool bFound = false;
	if( itemRect.left >= rc->left && itemRect.left <=rc->right && itemRect.top >= rc->top && itemRect.top <= rc->bottom )
	{
		bFound = true;
	}
	if( !bFound )
	{
		if( itemRect.right >= rc->left && itemRect.right <=rc->right && itemRect.bottom >= rc->top && itemRect.bottom <= rc->bottom )
		bFound = true;
	}
	if( bFound )
		pCustomTree->m_aItemsInRect.Add( pLoopInfo->hItem  );	

}
//***************************************************************************
//This is a call back function to called for each item when the items are iterated from a given start
//item to end item.Based on the pLoopInfo's param the item may be selected or deselected
void SelectItems( LOOPINFO* pLoopInfo )
{
	//Do the validity check on the item and parent
	if( !pLoopInfo->hItem )
		return;

	if( !pLoopInfo->pParent )
		return;

	//Get the tree control from the looping info
	CCustomTreeCtrl* pCustomTree = pLoopInfo->pTree;

	//Just to check the items's label
	CString str;
	str = pCustomTree->GetItemText(pLoopInfo->hItem);

	//Get the flag which gives the selection state that user wants to set
	bool bFlag = false;
	if( pLoopInfo->param )
		bFlag = * ( (bool*)(pLoopInfo->param) );
	
	//Set the item state of hItem as deselected
	if( bFlag )
		pCustomTree->SetItemState( pLoopInfo->hItem,TVIS_SELECTED,TVIS_SELECTED );
	else
		pCustomTree->SetItemState( pLoopInfo->hItem,~TVIS_SELECTED,TVIS_SELECTED );

}
//***************************************************************************



/////////////////////////////////////////////////////////////////////////////
// CCustomTreeCtrl

CCustomTreeCtrl::CCustomTreeCtrl()
{
	m_hSelectedItem = NULL;
	m_hPrevItem     = NULL;
	m_hDragSourceItem = NULL;
	m_hDragTargetItem = NULL;
	m_pDragImageList  = NULL;
	m_bExpandItem   = false;
	m_bExpandFlyBy  = false;
	m_defaultCursor = ::LoadCursor(NULL,IDC_ARROW);
	m_noCursor = ::LoadCursor(NULL,IDC_NO);
	m_bIsBandingON = false;
	m_bIsDragging = false;

	m_hFirstSelectedItem = NULL;
	m_hLastSelectedItem = NULL;
}

CCustomTreeCtrl::~CCustomTreeCtrl()
{

}


BEGIN_MESSAGE_MAP(CCustomTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CCustomTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClickLButton)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomTreeCtrl message handlers

/***************************************************************************
/DESCRIPTION:
/-----------
/			To add the line to linking the child with its parent.
/
/PARAM
/------
/		flag[IN]	-	If flag true means line is added,  otherwise removed
/
/RESULT:
/-------
/		VOID
*/
void CCustomTreeCtrl::AddLineStyle(bool flag /*= true*/ )
{
	DWORD dwStyle = GetStyle();
	if( flag )
		dwStyle |= TVS_HASLINES;
	else
		dwStyle &= ~TVS_HASLINES;

	::SetWindowLong( GetSafeHwnd(),GWL_STYLE,dwStyle); 
}
/***************************************************************************
/***DESCRIPTION:
/***-----------
/			To add the line to linking the child with its root.
/
/***PARAM
/***------
/		flag[IN]	-	If flag true means line is added,  otherwise removed
/
/RESULT:
/-------
/		VOID
****/
void CCustomTreeCtrl::AddLineAtRoot(bool flag /*= true*/ )
{
	DWORD dwStyle = GetStyle();
	if( flag )
		dwStyle |= TVS_LINESATROOT;
	else
		dwStyle &= ~TVS_LINESATROOT;

	::SetWindowLong( GetSafeHwnd(),GWL_STYLE,dwStyle); 
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To add the button [ +/ - ] to show the expanded or collpased state;
/
/PARAM
/------
/		flag[IN]	-	If flag true means line is added,  otherwise removed
/
/RESULT:
/-------
/		VOID
*/
void CCustomTreeCtrl::AddButton( bool flag /*= true*/ )
{
	DWORD dwStyle = GetStyle();
	if( flag )
		dwStyle |= TVS_HASBUTTONS;
	else
		dwStyle &= ~TVS_HASBUTTONS;

	::SetWindowLong( GetSafeHwnd(),GWL_STYLE,dwStyle); 
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To allow the editing of the label of an item
/
/PARAM
/------
/		flag[IN]	-	If flag true editiing is enabled,  otherwise disabled
/
/RESULT:
/-------
/		VOID
*/
void CCustomTreeCtrl::EnableEditLabel( bool flag /*= true*/ )
{
	DWORD dwStyle = GetStyle();
	if( flag )
		dwStyle |= TVS_EDITLABELS;
	else
		dwStyle &= ~TVS_EDITLABELS;

	::SetWindowLong( GetSafeHwnd(),GWL_STYLE,dwStyle); 
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To set the background color for the tree control
/
/PARAM
/------
/		bkColor[IN]	-	New background color to be set
/
/RESULT:
/-------
/		old background color of the control
*/
void CCustomTreeCtrl::SetBackgroundColor( COLORREF bkColor )
{
	//SetBkColor function can also be used
	//	SetBkColor( bkColor );

	//TVM_SETBKCOLOR can also be used by passing the color as the lparam
	//	SendMessage( TVM_SETBKCOLOR,0, (LPARAM)bkColor );

	TreeView_SetBkColor( GetSafeHwnd(),bkColor );    
	
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To set the line color
/
/PARAM
/------
/		bkColor[IN]	-	New line color to be set
/
/RESULT:
/-------
/		old line color of the control
*/
void CCustomTreeCtrl::SetLineColor( COLORREF bkColor )
{
	//TVM_SETLINECOLOR can also be used by passing the color as the lparam
	//SendMessage( TVM_SETLINECOLOR,0, (LPARAM)bkColor );

	
	
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To set the bold font for the label
/
/PARAM
/------
/		hItem[in]	-	Item to which the font to be set as bold
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::SetBoldFont(HTREEITEM hItem )
{
	if( !hItem )
		return;
	//SetItemState( hItem,TVIS_BOLD,TVIS_BOLD );
	Expand(hItem,TVE_EXPAND);
	 
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To expand the given item [ All the items under the given items are expanded]
/
/PARAM
/------
/		hItem[in]	-	Item to be expanded
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::ExpandTree(HTREEITEM hItem,bool bExpand /*= true*/ )
{
	if( !hItem )
		return;

	HTREEITEM hSibling = NULL;

	//If the item is already expanded 
	if( bExpand )
	{
		//Already the item is in expanded state
		if( GetItemState( hItem,TVIS_EXPANDED) == TVIS_EXPANDED  )
			return;
	}
	else
	{
		int state = GetItemState( hItem,TVIS_EXPANDED);
		if(  state && TVIS_EXPANDED  == 0 )
			return;
	}


	//Check whether the given item has children
	if( ItemHasChildren(hItem) )
	{
		if( bExpand )
			Expand( hItem,TVE_EXPAND );
		else
			Expand( hItem,TVE_COLLAPSE );

		//Loop through the childern and expand them
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetChildItem(hItem);

		while (hChildItem != NULL)
		{
			ExpandTree( hChildItem,bExpand );
			hNextItem = GetNextItem(hChildItem, TVGN_NEXT);
			hChildItem = hNextItem;
		}

	}

	hSibling = GetNextSiblingItem( hItem );
	//ExpandTree( hSibling );


}
/***************************************************************************
/DESCRIPTION:
/-----------
/		Called when the user starts its label editing
/
/PARAM
/------
/		pNMHDR[in]		-	Contains the data about the item edited
/		pResult[in/out]	-	Result of the edition
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	//Get the custom data and check whether item is editbale 
	HTREEITEM hItem = pTVDispInfo->item.hItem;

	if( hItem )
	{
		//Get the custom data from the item
		PCUSTOMITEMDATA pCustomData  = (PCUSTOMITEMDATA) GetItemData( hItem );

		if( pCustomData )
		{
			if( !pCustomData->m_bEditable )
			{
				*pResult = 1;
				return;
			}
		}

	}
	
	*pResult = 0;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		Called when the user completes its label editing
/
/PARAM
/------
/		pNMHDR[in]		-	Contains the data about the item edited
/		pResult[in/out]	-	Result of the edition
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here

	//Set the new text received from dispinfo structure to the item
	if( pTVDispInfo->item.pszText != NULL)
		SetItemText( pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
	
	*pResult = 0;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		Called when the user completes its label editing
/
/PARAM
/------
/		pNMHDR[in]		-	Contains the data about the item edited
/		pResult[in/out]	-	Result of the edition
/
/RESULT:
/-------
/		void
*/
HTREEITEM CCustomTreeCtrl::InsertItem(PCUSTOMINSERTSTRUCT lpInsertStruct )
{
	HTREEITEM hItem = CTreeCtrl::InsertItem( &lpInsertStruct->m_tvIS );
	//Set the custom data as the item data
	SetItemData( hItem,(DWORD)(lpInsertStruct->m_pCustomData));
	return hItem;
}

HTREEITEM CCustomTreeCtrl::InsertItem(CString strText, int nImg, int nSelectedImg, HTREEITEM hParent, HTREEITEM hAfterItem)
{
	HTREEITEM hItem;

	hItem = CTreeCtrl::InsertItem(strText, nImg, nSelectedImg, hParent, hAfterItem);

	return hItem;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		Called when the item is expanding
/
/PARAM
/------
/		pNMHDR[in]		-	Contains the data about the item edited
/		pResult[in/out]	-	Result of the edition
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//Get the item data
	PCUSTOMITEMDATA pData = (PCUSTOMITEMDATA) GetItemData(pNMTreeView->itemNew.hItem);

	//Change the image for expanding/collapsing
	if( pData )
	{
		if( pNMTreeView->action == TVE_EXPAND )
		{
			SetItemImage( pNMTreeView->itemNew.hItem,pData->m_iExpandedImage,pData->m_iExpandedImage);
		}
		else
		{
			SetItemImage( pNMTreeView->itemNew.hItem,pData->m_iNormalImage,pData->m_iExpandedImage);
		}
	}
	*pResult = 0;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		Called when the item is dragged
/
/PARAM
/------
/		pNMHDR[in]		-	Contains the data about the item edited
/		pResult[in/out]	-	Result of the edition
/
/RESULT:
/-------
/		void
*/


void CCustomTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	CollectSelectedItems();
	if( m_aSelectedItems.GetSize() == 0 )
		return;


	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//Find out the node to be dragged
	//-------------------------------
		CPoint pt;
		//Get the current position of the cursor
		::GetCursorPos(& pt );

		//Cursor position is in screen coordiantes.Convert it to the client
		//Systen
		ScreenToClient(&pt );

		//Find out the item to dragged using hit test
		m_hDragSourceItem = HitTest(pt);
		m_hDragTargetItem = NULL;

		//Mark that dragging  in progress
		m_bIsDragging = true;

		//If already the drag image list exists then delete it
		if( m_pDragImageList )
			m_pDragImageList->DeleteImageList();

		//Create the new image list for the drag source item
		m_pDragImageList = CreateDragImage( m_hDragSourceItem);

				
		int ct = m_pDragImageList->GetImageCount();

		

		CImageList* pDragImgList = GetImageListForDrag();


		if( pDragImgList )
		{
			//Disable the exisiting cursor as we are going to use the 
			//drage image as the cursor
			//ShowCursor( false );

			//Start dragging the image
			pDragImgList->BeginDrag( 0,CPoint(0,0));
			
			//Locks updates to the window specified by first parameter (this)
			//and displays the drag image at the position specified by pt.
			pDragImgList->DragEnter( this,pt );
			
			 //Capture the mouse
			SetCapture();
			
		}
		else
			m_bIsDragging = true;


	*pResult = 0;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To copy an item to another item
/
/PARAM
/------
/		hSourceItem[in]	-	Item to be copied
/		hTargetItem[in]	-	Target where the source to be copied
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::CopyItem(HTREEITEM hSourceItem,HTREEITEM hTargetItem )
{
	if( !hSourceItem || !hTargetItem )
		return;

	//Get the info about the child item
	TVITEM tvItem;
	tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_CHILDREN | TVIF_HANDLE;

	wchar_t buf[128];
	tvItem.pszText = buf;
	tvItem.cchTextMax = 128;
	tvItem.hItem = hSourceItem;

	GetItem(&tvItem);

	CUSTOMINSERTSTRUCT tvIS;
	tvIS.m_tvIS.hParent			=	hTargetItem;
	tvIS.m_tvIS.hInsertAfter	=	TVI_LAST; 
	tvIS.m_tvIS.item            =	tvItem;

	//Get the item data of the hSourceItem
	PCUSTOMITEMDATA pData = (PCUSTOMITEMDATA) GetItemData(hSourceItem);
	tvIS.m_pCustomData    = pData;

	HTREEITEM hInsertedItem     =	InsertItem(&tvIS);
	HTREEITEM hChildItem;

	//Check whether the source has children or not
	if( tvItem.cChildren != 0 )
	{
		while( hSourceItem )
		{
			//Get the child item
			hChildItem = GetChildItem( hSourceItem );

			if( hChildItem )
				hSourceItem = hChildItem;	
			CopyItem( hSourceItem, hInsertedItem );
			hSourceItem = GetNextItem( hSourceItem,TVGN_NEXT);
			
		}

	}


	//Refresh the tree once
	Invalidate();

	
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To delete an items and all its children recursively
/
/PARAM
/------
/		hTreeItem[in]	-	Item to be deleted
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::DeleteItem( HTREEITEM hTreeItem )
{
	if( !hTreeItem )
		return;

	CTreeCtrl::DeleteItem( hTreeItem );
}



//***************************************************************************
void CCustomTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	TV_HITTESTINFO tvHitInfo;
	tvHitInfo.pt = point;
	m_hSelectedItem = HitTest(&tvHitInfo );
	if (m_hSelectedItem)
	{
		CWnd* pParentWnd = GetParent();
		if (pParentWnd)	
			pParentWnd->PostMessage(WM_USER + 0x50);
	}
	//ShowItemInfo( m_hSelectedItem );
	//SetBoldFont( m_hSelectedItem );
	if( m_bExpandItem )
	{
		ExpandTree( m_hSelectedItem );
		return;
	}

	//If the click is not on the item and not on the item button means enable the
	//banding
	if(0 && !(tvHitInfo.flags & TVHT_ONITEM) && !(tvHitInfo.flags & TVHT_ONITEMBUTTON) )
	{

		//if there is no selected item then ON the banding
		m_bIsBandingON = true;

		//Store the starting point
		m_startPt = m_endPt = point;
	}

	//Capture the mouse
	SetCapture();


	CTreeCtrl::OnLButtonDown(nFlags, point);
}
//***************************************************************************
void CCustomTreeCtrl::ShowItemInfo(HTREEITEM hItem )
{
	if( !hItem )
		return;
	CString str = GetItemText( hItem );

	TVITEM infoItem;
	infoItem.hItem = hItem;
	infoItem.mask = TVIF_TEXT ;

	wchar_t label[128];
	infoItem.pszText = label;
	infoItem.cchTextMax = 128;
	GetItem( &infoItem );
	MessageBox( infoItem.pszText);
}

void CCustomTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//If dragging is enabled then drag image
	if( m_bIsDragging )
	{
		if( m_pDragImageList )
		{

			// Move the drag image to the next position 
			m_pDragImageList->DragMove(point);

			
			//DragShowNolock:Shows or hides the drag image during a drag operation,
			//without locking the window.
			//As the window is locked during the BeginDrag,if you dont unlock
			//using DragShowNoLock(), the previosly highlighted target wont be
			//refreshed.So it will remain in highlighted state.
			m_pDragImageList->DragShowNolock(false );

			//Find out the target node
			UINT flags;
			m_hDragTargetItem = HitTest( point,&flags );

			//if both source and target are same,show invalid cursor
			if( m_hDragSourceItem == m_hDragTargetItem )
			{
				::SetCursor( m_noCursor );								
			}
			else
				::SetCursor( m_defaultCursor);
					
			if( m_hDragTargetItem )
			{
				//Highlight the target item
				SelectDropTarget( m_hDragTargetItem );

				//Expand the target item
				Expand( m_hDragTargetItem,TVE_EXPAND);

				//Again lock the window
				m_pDragImageList->DragShowNolock(true );				
			
			}
			
		}
	}
	else if( m_bIsBandingON )
	{
		CClientDC dc(this);
		InvertRectangle( &dc,m_startPt,m_endPt );
		InvertRectangle( &dc,m_startPt,point );
		m_endPt = point;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CCustomTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_bIsDragging )
	{
		//Unlock the window
		m_pDragImageList->DragLeave(this);

		//End the drag operation
		m_pDragImageList->EndDrag();

		//if( !m_hDragSourceItem || !m_hDragTargetItem )
		//	return;

		if( m_hDragSourceItem != m_hDragTargetItem )
		{

			//Drop the selected items one by one over the target item
			int iSelItemCnt = 0;
			for( ; iSelItemCnt < m_aSelectedItems.GetSize(); iSelItemCnt++ )
			{
				PCUSTOMITEMDATA pSourceData = NULL;
				if( m_aSelectedItems[iSelItemCnt] )
				pSourceData = (PCUSTOMITEMDATA) GetItemData(m_aSelectedItems[iSelItemCnt]);

				PCUSTOMITEMDATA pTargetData = NULL;
				if( m_hDragTargetItem )
					pTargetData = (PCUSTOMITEMDATA) GetItemData(m_hDragTargetItem);

				//Check the hierarchy of the source and target matches
				if( pSourceData && pTargetData)
				{
					if( pSourceData->m_iState > pTargetData->m_iState )
					{
					
						//If the parent of the source is same as the target 
						//item then dont allow.Because already the source is in the target
						HTREEITEM hSourceParent = GetParentItem( m_hDragSourceItem );

						if( hSourceParent != m_hDragTargetItem )
						{
							//delete the drage image list
							if( m_pDragImageList )
								m_pDragImageList->DeleteImageList();
							//delete m_pDragImageList;
							m_pDragImageList = NULL;

							CopyItem( m_aSelectedItems[iSelItemCnt],m_hDragTargetItem);

							//After copying the item remove the selection
							SetItemState( m_aSelectedItems[iSelItemCnt],~TVIS_SELECTED,TVIS_SELECTED );

							//Remove the item from the selection array
							m_aSelectedItems.RemoveAt(iSelItemCnt);
							iSelItemCnt--;

							UpdateWindow();
													
						}
					}
				}
			}		
		}
		
	}

	//if the banding is ON then remove the banding flag and rectangle drawn
	if( m_bIsBandingON )
	{
		m_bIsBandingON = false;
		CClientDC dc(this);
		InvertRectangle( &dc,m_startPt,m_endPt );

		if( m_startPt != m_endPt )
		{
				CRect rect;
			if( m_startPt.y < m_endPt.y )
			{
				rect.left = m_startPt.x;
				rect.top  = m_startPt.y;
				rect.right = m_endPt.x;
				rect.bottom = m_endPt.y;
				
			}
			else
			{
				rect.left = m_endPt.x;
				rect.top  = m_endPt.y;
				rect.right = m_startPt.x;
				rect.bottom = m_startPt.y;				

			}
			//Find the items inside the rectangle
			ItemsInRect( rect );
		}
	}
	
	if( m_pDragImageList )
			m_pDragImageList->DragShowNolock(FALSE );

	m_bIsDragging = false;
	SelectDropTarget(NULL);

	//Release the mouse
	ReleaseCapture();
	ShowCursor( true );
	CTreeCtrl::OnLButtonUp(nFlags, point);
}


/***************************************************************************
/DESCRIPTION:
/-----------
/		To iterate all the items and call the callback function for each item
/
/PARAM
/------
/		func[in]	-	Call back function to be called on each item
/		hStart[in]	-	Start node for the iteration
/		hEnd[in]	-	End node to stop the iteration
/		pInfo[in]	-	User parameter
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::IterateItems( ScanCallBackFunc func,
							HTREEITEM hStart /*= NULL*/,
							HTREEITEM hEnd, /*= NULL*/
							void* pInfo /*=NULL*/
							)
{
	//If there is no start then take the root item
	if( !hStart )
		hStart = GetRootItem();

	ScanItems( func,hStart,hEnd,pInfo );
}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To iterate all the items and call the callback function for each item
/
/PARAM
/------
/		func[in]	-	Call back function to be called on each item
/		hStart[in]	-	Start node for the iteration
/		hEnd[in]	-	End node to stop the iteration
/		pInfo[in]	-	User parameter
/
/RESULT:
/-------
/		void
*/
void CCustomTreeCtrl::ScanItems( ScanCallBackFunc func,
							HTREEITEM hStart /*= NULL*/,
							HTREEITEM hEnd, /*= NULL*/
							void* pInfo /*= NULL*/
							)
{
	
	//Loop from start till the end is reached
	while( hStart )
	{
		LOOPINFO lInfo;
		lInfo.pTree =  this;
		lInfo.hItem = hStart;
		lInfo.pParent = GetParent();
		lInfo.param   = pInfo;

		CRect rc;
		GetItemRect( hStart,&rc,FALSE );

		//Callback is called for the current item
		func( &lInfo );

		//Check whether we have reached iterating upto the end node
		if( hStart == hEnd )
			return;

		//Get the children of the current item and call recursively
		HTREEITEM hChild = NULL;
		if( ItemHasChildren( hStart ) )
		{
			hChild = GetChildItem( hStart );
			ScanItems( func,hChild,hEnd,pInfo);
		}

		hStart = GetNextSiblingItem( hStart );
	}

}
/***************************************************************************
/DESCRIPTION:
/-----------
/		To Draw the banding rectangle
/
/PARAM
/------
/		void
/RESULT:
/-------
/		void
*/
void	CCustomTreeCtrl::DrawBandingRect()
{

}
/***************************************************************************
/DESCRIPTION:
/-----------
/			To invert the rectangle being drawn
/
/PARAM
/------
/		pDC[in]		-	Device context used for drawing
/		startPt[in]	-	Start point of the rectangle
/		endPt[in]	-	End point of the rectangle
/RESULT:
/-------
/		void
*/
void	CCustomTreeCtrl::InvertRectangle( CDC* pDC,CPoint startPt,CPoint endPt )	
{
	int nOldMode = pDC->SetROP2( R2_NOT );
	pDC->Rectangle( startPt.x,startPt.y,endPt.x,endPt.y);
	pDC->SetROP2(nOldMode);
}
/***************************************************************************
/DESCRIPTION:
/-----------
/			To find the items inside the given rectangle
/
/PARAM
/------
/		rect[in]		-	Bounding rectangle area
/RESULT:
/-------
/		void
*/
bool	CCustomTreeCtrl::ItemsInRect(CRect rect)
{
	
	//First deselect all the items
	bool bFlag = false;
	IterateItems( SelectItems,NULL,NULL,&bFlag );
	UpdateWindow();

	ClientToScreen(&rect);
	m_aItemsInRect.RemoveAll();
	IterateItems( FindItemsInRect,NULL,NULL,&rect );

	int i = 0;
	for( ; i < m_aItemsInRect.GetSize(); i++ )
	{
		//CString str = GetItemText( m_aItemsInRect[i] );
		//MessageBox( str );
		SetItemState( m_aItemsInRect[i],TVIS_SELECTED,TVIS_SELECTED);
	}
	return true;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/			To get the first selected item in the tree
/
/PARAM
/------
/		void
/RESULT:
/-------
/		First selected item if any,otherwise NULL
*/

HTREEITEM CCustomTreeCtrl::GetFirstSelectedItem() const 
{
	for(HTREEITEM hItem = GetRootItem(); hItem; hItem = GetNextVisibleItem(hItem))
		if(GetItemState(hItem, UINT(TVIS_SELECTED)) & TVIS_SELECTED)
			return hItem;
	return 0;
}
/***************************************************************************
/DESCRIPTION:
/-----------
/			To get the next selected item in the tree
/
/PARAM
/------
/		void
/RESULT:
/-------
/		Selected item if any,otherwise NULL
*/

HTREEITEM CCustomTreeCtrl::GetNextSelectedItem(HTREEITEM hItem) const 
{
	if( !hItem )
		return 0;

	for( hItem = GetNextVisibleItem(hItem); hItem; hItem = GetNextVisibleItem(hItem))
	{
		if(GetItemState(hItem, UINT(TVIS_SELECTED)) & TVIS_SELECTED)
			return hItem;
	}
	return 0;
}

/***************************************************************************
/DESCRIPTION:
/-----------
/			To Collect the selected items in an array
/
/PARAM
/------
/		void
/RESULT:
/-------
/		void
*/
void	CCustomTreeCtrl::CollectSelectedItems()
{
	m_aSelectedItems.RemoveAll();
	for(HTREEITEM hItem = GetFirstSelectedItem(); hItem != 0; hItem = GetNextSelectedItem(hItem)) 
	{
		m_aSelectedItems.Add(hItem);		
	}

}
/***************************************************************************
/DESCRIPTION:
/-----------
/			To generate the imagelist for the dragging
/
/PARAM
/------
/		void
/RESULT:
/-------
/		Create imagelist if any,otherwise NULL
*/
CImageList* CCustomTreeCtrl::GetImageListForDrag()
{
	//If there is no selected item for the drag return NULL
	CImageList* pNewImageList = NULL;

	if( m_aSelectedItems.GetSize() == 0 )
		return NULL;

	//Get the imagelist of the tree
	CImageList* pTreeImgList = GetImageList(TVSIL_NORMAL );

	CClientDC dc(this);
	CDC		  cMemDC;
	CBitmap   cBitmap;


	int ImgWidth,ImgHeight;

	//Loop through all the selected items and get the size of the bitmap to be created
	int iSelItemCnt = 0;
	CRect rc;
	for( ; iSelItemCnt <  m_aSelectedItems.GetSize(); iSelItemCnt++ )
	{
		HTREEITEM hSelItem = m_aSelectedItems[iSelItemCnt];
		CRect itemRect;
		GetItemRect( hSelItem ,&itemRect,true);

		CImageList* pImgList = GetImageList( TVSIL_NORMAL );
		ImageList_GetIconSize( pImgList->GetSafeHandle(),&ImgWidth,&ImgHeight );

		if( iSelItemCnt == 0 )
		{
			rc.left = 0;
			rc.top  = 0;
			rc.right = ImgWidth + itemRect.Width();
			rc.bottom = ImgHeight;
		}
		else
		{
			if( rc.Width() < (itemRect.Width()+ImgWidth) )	
				rc.right = itemRect.Width()+32;

			rc.bottom += itemRect.Height();
		}

	}

	//Create the memory dc
	if(!cMemDC.CreateCompatibleDC(&dc))
		return NULL;

	//Create the bitmap of the required size
	if(!cBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = cMemDC.SelectObject( &cBitmap );

	//Fill the bitmap with green color which is used as the mask
	cMemDC.FillSolidRect(0,0,rc.Width(), rc.Height(), RGB(0, 255, 0));

	CFont * pFontOld = cMemDC.SelectObject(GetFont());

	//Loop through all the selected items and get their image
	iSelItemCnt = 0;
	CImageList* pSelImageList = NULL;
	CImageList * pImageList = GetImageList(TVSIL_NORMAL);

	CRect imgRect,textRect,itemRect;

	for( ; iSelItemCnt <  m_aSelectedItems.GetSize(); iSelItemCnt++ )
	{
		HTREEITEM hSelItem = m_aSelectedItems[iSelItemCnt];

		CString str;
		str = GetItemText( hSelItem );

		if( iSelItemCnt == 0 )
			GetItemRect( hSelItem,&itemRect,true ); 

		if( iSelItemCnt == 0 )
		{
			imgRect.left = 0;
			imgRect.top  = 0;
			imgRect.right = ImgWidth;
			imgRect.bottom = ImgHeight;

			textRect.left = ImgWidth;
			textRect.top  = 0;
			textRect.right = textRect.left  + itemRect.Width();
			textRect.bottom = itemRect.Height();
		}
		else
		{
			imgRect.top += ImgHeight;
			imgRect.bottom += ImgHeight;

			textRect.left = ImgWidth;
			textRect.top  = textRect.bottom;
			textRect.right = textRect.left + itemRect.Width();
			textRect.bottom = textRect.top + itemRect.Height();

		}

	
		if( hSelItem )
		{
			//Get the image of the selected item
			int nImg, nSelImg;
			GetItemImage(hSelItem,nImg,nSelImg);
			HICON hIcon = pImageList->ExtractIcon(nImg);

			//Draw this icon on to the memory dc
			::DrawIconEx(cMemDC.m_hDC, imgRect.left, imgRect.top, hIcon, 32, 32, 0, 0, DI_NORMAL);
		
			//Next draw the text
			cMemDC.DrawText(str, textRect, DT_LEFT| DT_VCENTER | DT_SINGLELINE|DT_NOPREFIX);
			DestroyIcon(hIcon);
							
		}
	}

	cMemDC.SelectObject( pOldMemDCBitmap );


	pNewImageList = new CImageList;
	BITMAP bm;
	cBitmap.GetBitmap(&bm);

	CBitmap bmp;
	pNewImageList->Create( bm.bmWidth,bm.bmHeight,ILC_COLOR32 | ILC_MASK,1,1);

	// Here green is used as mask color
	pNewImageList->Add(&cBitmap, RGB(0, 255, 0));

	return pNewImageList;
}



/*BOOL SaveBitmap(HDC hDC,HBITMAP hBitmap,CString szPath)
{
    FILE * fp= NULL;
    fp = fopen(szPath.GetBuffer(1),"wb");
    if(fp == NULL)
        return false;
    
    BITMAP Bm;
    BITMAPINFO BitInfo;
    ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
    BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BitInfo.bmiHeader.biBitCount = 0;

    if(!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
        return (false);

    Bm.bmHeight = BitInfo.bmiHeader.biHeight;
    Bm.bmWidth  = BitInfo.bmiHeader.biWidth;

    BITMAPFILEHEADER    BmHdr;
    
    BmHdr.bfType        = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
    BmHdr.bfSize        = (((3 * Bm.bmWidth + 3) & ~3) * Bm.bmHeight) 
                          + sizeof(BITMAPFILEHEADER) 
                          + sizeof(BITMAPINFOHEADER);
    BmHdr.bfReserved1    = BmHdr.bfReserved2 = 0;
    BmHdr.bfOffBits      = (DWORD) sizeof(BITMAPFILEHEADER) 
                          + sizeof(BITMAPINFOHEADER);
    
    BitInfo.bmiHeader.biCompression = 0;
    // Writing Bitmap File Header ////
    fwrite(&BmHdr,sizeof(BITMAPFILEHEADER),1,fp);

    fwrite(&BitInfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp);

    BYTE *pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
    if(!::GetDIBits(hDC, hBitmap, 0, Bm.bmHeight, 
                 pData, &BitInfo, DIB_RGB_COLORS))
        return (false);
    if(pData != NULL)
        fwrite(pData,1,BitInfo.bmiHeader.biSizeImage,fp);

    fclose(fp);
    delete (pData);

    return (true);
}*/
///***************************************************************************
void CCustomTreeCtrl::OnClickLButton(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

//TODO: Add your control notification handler code here
	
	//Do the hittest to find out which item is being clicked
		CPoint pt;
		//Get the current position of the cursor
		::GetCursorPos(& pt );

		//Cursor position is in screen coordiantes.Convert it to the client
		//Systen
		ScreenToClient(&pt );

		HTREEITEM hClickItem = HitTest(pt);

		if( !hClickItem )
			return;

	//Get the control and shift key states
		bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
		bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
		
		HTREEITEM hSelItem = GetSelectedItem() ;


		//If the Control Key is ON
		if( bCtrl )
		{
			//NOTE1: If the clicked item was already has the focus then no TVN_SELCHANGE
			//message will be genereated.So I'm handling this situation here;
			//If the item was selected already deselect it otherwise select it
			//if( (hSelItem==hClickItem) && GetItemState( hClickItem,TVIS_SELECTED) & TVIS_SELECTED )
			if( GetItemState( hClickItem,TVIS_SELECTED) & TVIS_SELECTED )
			{
				SetItemState( hClickItem,~TVIS_SELECTED,TVIS_SELECTED);

				//Once after deselecting the item, it goes into edit mode.To avoid this
				//I'm just setting the *pResult = 1 which avoids editing the label
				*pResult = 1;
				
			}
			UpdateWindow();
		}
}
//***************************************************************************
void CCustomTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	//Get the old selected item and new item
	HTREEITEM hOldSelItem = pNMTreeView->itemOld.hItem;
	HTREEITEM hNewSelItem = pNMTreeView->itemNew.hItem;

	CString str1,str2;
	str1 = GetItemText( hOldSelItem );
	str2= GetItemText( hNewSelItem );

	//Get the action by which the selection is generated
	UINT& action = pNMTreeView->action;

	//Get the shift and control key state
	bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;

	if (bShift && !m_hFirstSelectedItem)
	{
		if (hOldSelItem)
			m_hFirstSelectedItem = hOldSelItem;
		m_hLastSelectedItem = hNewSelItem;
	}
	else if(!bShift)
		m_hFirstSelectedItem = NULL;

	//If control key is ON then select the old item also.Anyhow the new item is selected
	//OnSelchanging
	if((action == TVC_BYMOUSE && bCtrl) ) 
	{
		//Select the old item
		if( hOldSelItem && m_bOldItemSelected)
		{
			SetItemState(hOldSelItem, UINT(TVIS_SELECTED), UINT(TVIS_SELECTED));			
		}
	}
	//Similary if already item1 is in selection.Now SHift + Down arrow key is pressed
	//Old = item1 and new = item2 and i have to select both item1 and item2
	else if ( (action == TVC_BYKEYBOARD) && bShift )
	{
		//Select the old item
		if( hOldSelItem && m_bOldItemSelected )
		{
			SetItemState(hOldSelItem, UINT(TVIS_SELECTED), UINT(TVIS_SELECTED));
		}
	}
	//If the selection is by mouse and shfit key is also used then select all the items
	//starting from old to new.All the intermediate items between old to new is selected
	else if( (action == TVC_BYMOUSE) && bShift && m_hFirstSelectedItem)
	{
		bool bSelect = true;
		//IterateItems( SelectItems,hOldSelItem,hNewSelItem,&bSelect );
		CRect rc1, rc2;
		GetItemRect(m_hFirstSelectedItem, &rc1, FALSE);
		GetItemRect(m_hLastSelectedItem, &rc2, FALSE);
		CRect rc;
		rc.left = rc1.left;
		rc.right = rc1.right;
		rc.top = min(rc1.top, rc2.top) + 3;
		rc.bottom = max(rc1.bottom, rc2.bottom) - 3;
		ItemsInRect(rc);
		//IterateItems(SelectItems, m_hFirstSelectedItem, m_hLastSelectedItem, &bSelect);
	}
	//If control and shift key is in off state and user clicked on an item.Means he wants to
	//remove whatever the previous selected items and wants to select only the clicked item
	if( !bCtrl && !bShift && hNewSelItem )
	{
		//Remove all the old selection
		bool bSelect = false;
		IterateItems( SelectItems,NULL,NULL,&bSelect );

		//Select only the new item
		SetItemState(hNewSelItem, UINT(TVIS_SELECTED), UINT(TVIS_SELECTED));

	}
	//May be the developer who uses the control programatically sets the selection.
	//He wants to retain the old selection in addition to the new item
	if( action == TVC_UNKNOWN && m_bOldItemSelected && hOldSelItem  ) 
	{
		SetItemState(hOldSelItem, UINT(TVIS_SELECTED), UINT(TVIS_SELECTED));		
	}

	UpdateWindow();
	*pResult = 0;
}
//***************************************************************************
void CCustomTreeCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	// TODO: Add your control notification handler code here
	*pResult = 0;

	//Get the old selected item and new item
	HTREEITEM hOldSelItem = pNMTreeView->itemOld.hItem;
	HTREEITEM hNewSelItem = pNMTreeView->itemNew.hItem;

	//--------------------------------------------------------------------------------------------
	//Get the state of the old item.This will become false in the following 2 cases.
	//CASE1: when the user does the first selection ie., there is no selection at all in the tree,
	//		 and user clicks an item for selection.In this case olditem = NULL
	//		 and newitem=clickeditem.

	//CASE2: Select item1 first [ newitem=clickeditem[!TVIS_SELECTED] olditem = NULL].
	//		 Press ctrl key and select item2[ newitem=clickeditem[!TVIS_SELECTED] olditem = item1[TVIS_SELECTED]].
	//	     Press ctrl key and deselect item2.Now in this case we wont get control here.It will be
	//	     handled by OnClickLButton which deselects item2.Again press ctrl key and deselect the
	//		 item1 [ newitem=item[TVIS_SELECTED] olditem = item2[!TVIS_SELECTED]].

	 m_bOldItemSelected = hOldSelItem && (GetItemState(hOldSelItem, UINT(TVIS_SELECTED)) & TVIS_SELECTED);

	//--------------------------------------------------------------------------------------------//
	//Get the shift and control key state
	 bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	 bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;

	 //--------------------------------------------------------------------------------------------//

	 //--------------------------------------------------------------------------------------------//
#if 1

	 //Get the action by which the selection is generated
	 UINT& action = pNMTreeView->action;

	 //If the action is by mouse and ctrl is key on at the same time
	 if ((action == TVC_BYMOUSE && bCtrl))
	 {
		 //If the new item was already in selected state, then deselect it.This may happen say for example
		 //item1 is first selected and by pressing ctrl key item2 is selected.Now we have 2 items in selection
		 //Now if the user presses ctrl key and clicks on item1.Now our olditem is item2[TVIS_SELECTED] and new item is
		 //item1[TVIS_SELECTED] and and at the same time item1 is in the selected state and our ctrl key is ON
		 if (hNewSelItem)
		 {
			 SetItemState(hNewSelItem, UINT(TVIS_SELECTED), UINT(TVIS_SELECTED));
			 UpdateWindow();

			 //abort change of selection.Otherwise TVN_SELCHANGED message will be generated.
			 if ((GetItemState(hNewSelItem, TVIS_SELECTED)&TVIS_SELECTED) && 0)
			 {
				 //Deselect the new item
				 SetItemState(hNewSelItem, UINT(~TVIS_SELECTED), UINT(TVIS_SELECTED));
				 UpdateWindow();
			 }
		 }
	 }
	 //If say item1 and item2 is already selected.Now the user presses shift + UP arrow key.
	 //Now our olditem = item2[TVIS_SELECTED] and newitem = item1[TVIS_SELECTED].So we have
	 //to keep the item1 as selected and item2 as deselected one.
	 if ((action == TVC_BYKEYBOARD) && bShift)
	 {
		 //If the new item is in selected state then remove the selection of the old item
		 //i.e, the selection of item2
		 if (GetItemState(hNewSelItem, TVIS_SELECTED)&TVIS_SELECTED)
			 m_bOldItemSelected = false;
	 }
#endif

	
}
//***************************************************************************
void CCustomTreeCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	CollectSelectedItems();

	if( pTVKeyDown->wVKey == VK_DELETE )
	{
		//Delete the selected items
		return;
		int iSelItemCnt = 0;
		for( ; iSelItemCnt < m_aSelectedItems.GetSize(); iSelItemCnt++ )
		{
			SetItemData( m_aSelectedItems[iSelItemCnt],0 );
			DeleteItem( m_aSelectedItems[iSelItemCnt] );
		}
	}
	if( pTVKeyDown->wVKey == VK_F2 )
	{
		if( m_aSelectedItems.GetSize() == 1 )
			EditLabel( m_aSelectedItems[0] );
	}

	UpdateWindow();	
}
//***************************************************************************
