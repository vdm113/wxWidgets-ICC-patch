/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/dnd.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DND_H_
#define _WX_QT_DND_H_

#define wxDROP_ICON(name)   wxICON(name)

class WXDLLIMPEXP_CORE wxDropTarget : public wxDropTargetBase
{
public:
    wxDropTarget(wxDataObject *dataObject = NULL );
    
    virtual bool OnDrop(wxCoord x, wxCoord y);
    virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult def);
    virtual bool GetData();

    wxDataFormat GetMatchingPair();

protected:

private:
};


class WXDLLIMPEXP_CORE wxDropSource: public wxDropSourceBase
{
public:
    wxDropSource( wxWindow *win = NULL,
                  const wxIcon &copy = wxNullIcon,
                  const wxIcon &move = wxNullIcon,
                  const wxIcon &none = wxNullIcon);

    wxDropSource( wxDataObject& data,
                  wxWindow *win,
                  const wxIcon &copy = wxNullIcon,
                  const wxIcon &move = wxNullIcon,
                  const wxIcon &none = wxNullIcon);
    
    virtual wxDragResult DoDragDrop(int flags = wxDrag_CopyOnly);
};
#endif // _WX_QT_DND_H_
