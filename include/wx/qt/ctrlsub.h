/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/ctrlsub.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_CTRLSUB_H_
#define _WX_QT_CTRLSUB_H_

class WXDLLIMPEXP_CORE wxControlWithItems : public wxControlWithItemsBase
{
public:
    wxControlWithItems();

protected:

private:
    DECLARE_ABSTRACT_CLASS(wxControlWithItems)
};

#endif // _WX_QT_CTRLSUB_H_
