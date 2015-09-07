/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/dnd.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dnd.h"

wxDropTarget::wxDropTarget(wxDataObject *WXUNUSED(dataObject))
{
}

bool wxDropTarget::OnDrop(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y))
{
    return false;
}

wxDragResult wxDropTarget::OnData(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), wxDragResult WXUNUSED(def))
{
    return wxDragResult();
}

bool wxDropTarget::GetData()
{
    return false;
}

wxDataFormat wxDropTarget::GetMatchingPair()
{
    wxFAIL_MSG("wxDropTarget::GetMatchingPair() not implemented in src/qt/dnd.cpp");
    return wxDF_INVALID;
}

//##############################################################################


wxDropSource::wxDropSource( wxWindow *WXUNUSED(win),
              const wxIcon &WXUNUSED(copy),
              const wxIcon &WXUNUSED(move),
              const wxIcon &WXUNUSED(none))
{
}

wxDropSource::wxDropSource( wxDataObject& WXUNUSED(data),
              wxWindow *WXUNUSED(win),
              const wxIcon &WXUNUSED(copy),
              const wxIcon &WXUNUSED(move),
              const wxIcon &WXUNUSED(none))
{
}

wxDragResult wxDropSource::DoDragDrop(int WXUNUSED(flags))
{
    return wxDragResult();
}

