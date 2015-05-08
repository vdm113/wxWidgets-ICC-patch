/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

//////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/dragimgg.h
// Purpose:     wxDragImage class: a kind of a cursor, that can cope
//              with more sophisticated images
// Author:      Julian Smart
// Modified by:
// Created:     29/2/2000
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DRAGIMGG_H_
#define _WX_DRAGIMGG_H_

#include "wx/bitmap.h"
#include "wx/icon.h"
#include "wx/cursor.h"
#include "wx/treectrl.h"
#include "wx/listctrl.h"
#include "wx/log.h"
#include "wx/overlay.h"

/*
  To use this class, create a wxDragImage when you start dragging, for example:

  void MyTreeCtrl::OnBeginDrag(wxTreeEvent& event)
  {
#ifdef __WXMSW__
    ::UpdateWindow((HWND) GetHWND()); // We need to implement this in wxWidgets
#endif

    CaptureMouse();

    m_dragImage = new wxDragImage(* this, itemId);
    m_dragImage->BeginDrag(wxPoint(0, 0), this);
    m_dragImage->Move(pt, this);
    m_dragImage->Show(this);
    ...
  }

  In your OnMouseMove function, hide the image, do any display updating required,
  then move and show the image again:

  void MyTreeCtrl::OnMouseMove(wxMouseEvent& event)
  {
    if (m_dragMode == MY_TREE_DRAG_NONE)
    {
        event.Skip();
        return;
    }

    // Prevent screen corruption by hiding the image
    if (m_dragImage)
        m_dragImage->Hide(this);

    // Do some updating of the window, such as highlighting the drop target
    ...

#ifdef __WXMSW__
    if (updateWindow)
        ::UpdateWindow((HWND) GetHWND());
#endif

    // Move and show the image again
    m_dragImage->Move(event.GetPosition(), this);
    m_dragImage->Show(this);
 }

 Eventually we end the drag and delete the drag image.

 void MyTreeCtrl::OnLeftUp(wxMouseEvent& event)
 {
    ...

    // End the drag and delete the drag image
    if (m_dragImage)
    {
        m_dragImage->EndDrag(this);
        delete m_dragImage;
        m_dragImage = NULL;
    }
    ReleaseMouse();
 }
*/

/*
 * wxGenericDragImage
 */

class WXDLLIMPEXP_CORE wxGenericDragImage: public wxObject
{
public:

    // Ctors & dtor
    ////////////////////////////////////////////////////////////////////////////

    wxGenericDragImage(const wxCursor& cursor = wxNullCursor)
    {
        Init();
        Create(cursor);
    }

    wxGenericDragImage(const wxBitmap& image, const wxCursor& cursor = wxNullCursor)
    {
        Init();

        Create(image, cursor);
    }

    wxGenericDragImage(const wxIcon& image, const wxCursor& cursor = wxNullCursor)
    {
        Init();

        Create(image, cursor);
    }

    wxGenericDragImage(const wxString& str, const wxCursor& cursor = wxNullCursor)
    {
        Init();

        Create(str, cursor);
    }

#if wxUSE_TREECTRL
    wxGenericDragImage(const wxTreeCtrl& treeCtrl, wxTreeItemId& id)
    {
        Init();

        Create(treeCtrl, id);
    }
#endif

#if wxUSE_LISTCTRL
    wxGenericDragImage(const wxListCtrl& listCtrl, long id)
    {
        Init();

        Create(listCtrl, id);
    }
#endif

    virtual ~wxGenericDragImage();

    // Attributes
    ////////////////////////////////////////////////////////////////////////////

#ifdef wxHAS_NATIVE_OVERLAY
    // backing store is not used when native overlays are
    void SetBackingBitmap(wxBitmap* WXUNUSED(bitmap)) { }
#else
    // For efficiency, tell wxGenericDragImage to use a bitmap that's already
    // created (e.g. from last drag)
    void SetBackingBitmap(wxBitmap* bitmap) { m_pBackingBitmap = bitmap; }
#endif // wxHAS_NATIVE_OVERLAY/!wxHAS_NATIVE_OVERLAY

    // Operations
    ////////////////////////////////////////////////////////////////////////////

    // Create a drag image with a virtual image (need to override DoDrawImage, GetImageRect)
    bool Create(const wxCursor& cursor = wxNullCursor);

    // Create a drag image from a bitmap and optional cursor
    bool Create(const wxBitmap& image, const wxCursor& cursor = wxNullCursor);

    // Create a drag image from an icon and optional cursor
    bool Create(const wxIcon& image, const wxCursor& cursor = wxNullCursor);

    // Create a drag image from a string and optional cursor
    bool Create(const wxString& str, const wxCursor& cursor = wxNullCursor);

#if wxUSE_TREECTRL
    // Create a drag image for the given tree control item
    bool Create(const wxTreeCtrl& treeCtrl, wxTreeItemId& id);
#endif

#if wxUSE_LISTCTRL
    // Create a drag image for the given list control item
    bool Create(const wxListCtrl& listCtrl, long id);
#endif

    // Begin drag. hotspot is the location of the drag position relative to the upper-left
    // corner of the image.
    bool BeginDrag(const wxPoint& hotspot, wxWindow* window, bool fullScreen = false, wxRect* rect = NULL);

    // Begin drag. hotspot is the location of the drag position relative to the upper-left
    // corner of the image. This is full screen only. fullScreenRect gives the
    // position of the window on the screen, to restrict the drag to.
    bool BeginDrag(const wxPoint& hotspot, wxWindow* window, wxWindow* fullScreenRect);

    // End drag
    bool EndDrag();

    // Move the image: call from OnMouseMove. Pt is in window client coordinates if window
    // is non-NULL, or in screen coordinates if NULL.
    bool Move(const wxPoint& pt);

    // Show the image
    bool Show();

    // Hide the image
    bool Hide();

    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    void Init();

    // Override this if you are using a virtual image (drawing your own image)
    virtual wxRect GetImageRect(const wxPoint& pos) const;

    // Override this if you are using a virtual image (drawing your own image)
    virtual bool DoDrawImage(wxDC& dc, const wxPoint& pos) const;

    // Override this if you wish to draw the window contents to the backing bitmap
    // yourself. This can be desirable if you wish to avoid flicker by not having to
    // redraw the window itself before dragging in order to be graphic-minus-dragged-objects.
    // Instead, paint the drag image's backing bitmap to be correct, and leave the window
    // to be updated only when dragging the objects away (thus giving a smoother appearance).
    virtual bool UpdateBackingFromWindow(wxDC& windowDC, wxMemoryDC& destDC,
                                         const wxRect& sourceRect, const wxRect& destRect) const;

    // Erase and redraw simultaneously if possible
    virtual bool RedrawImage(const wxPoint& oldPos, const wxPoint& newPos, bool eraseOld, bool drawNew);

protected:
    wxBitmap        m_bitmap;
    wxIcon          m_icon;
    wxCursor        m_cursor;
    wxCursor        m_oldCursor;
//    wxPoint         m_hotspot;
    wxPoint         m_offset; // The hostpot value passed to BeginDrag
    wxPoint         m_position;
    bool            m_isDirty;
    bool            m_isShown;
    wxWindow*       m_window;
    wxDC*           m_windowDC;

#ifdef wxHAS_NATIVE_OVERLAY
    wxOverlay       m_overlay;
    wxDCOverlay*     m_dcOverlay;
#else
    // Stores the window contents while we're dragging the image around
    wxBitmap        m_backingBitmap;
    wxBitmap*       m_pBackingBitmap; // Pointer to existing backing bitmap
                                      // (pass to wxGenericDragImage as an efficiency measure)
    // A temporary bitmap for repairing/redrawing
    wxBitmap        m_repairBitmap;
#endif // !wxHAS_NATIVE_OVERLAY

    wxRect          m_boundingRect;
    bool            m_fullScreen;

private:
    wxDECLARE_DYNAMIC_CLASS(wxGenericDragImage);
    wxDECLARE_NO_COPY_CLASS(wxGenericDragImage);
};

#endif
    // _WX_DRAGIMGG_H_
