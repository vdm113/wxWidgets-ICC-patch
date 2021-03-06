/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/html/m_style.cpp
// Purpose:     wxHtml module for parsing <style> tag
// Author:      Vaclav Slavik
// Copyright:   (c) 2002 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_HTML && wxUSE_STREAMS

#ifndef WX_PRECOMP
#endif

#include "wx/html/forcelnk.h"
#include "wx/html/m_templ.h"

FORCE_LINK_ME(m_style)


TAG_HANDLER_BEGIN(STYLE, "STYLE")
    TAG_HANDLER_CONSTR(STYLE) { }

    TAG_HANDLER_PROC(WXUNUSED(tag))
    {
        // VS: Ignore styles for now. We must have this handler present,
        //     because CSS style text would be rendered verbatim otherwise
        return true;
    }

TAG_HANDLER_END(STYLE)


TAGS_MODULE_BEGIN(StyleTag)

    TAGS_MODULE_ADD(STYLE)

TAGS_MODULE_END(StyleTag)

#endif
