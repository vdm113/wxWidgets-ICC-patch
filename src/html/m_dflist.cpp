/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/html/m_dflist.cpp
// Purpose:     wxHtml module for definition lists (DL,DT,DD)
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
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

#include "wx/html/htmlcell.h"

FORCE_LINK_ME(m_dflist)




TAG_HANDLER_BEGIN(DEFLIST, "DL,DT,DD" )

    TAG_HANDLER_CONSTR(DEFLIST) { }

    TAG_HANDLER_PROC(tag)
    {
        wxHtmlContainerCell *c;


        if (tag.GetName() == wxT("DL"))
        {
            if (m_WParser->GetContainer()->GetFirstChild() != NULL)
            {
                m_WParser->CloseContainer();
                m_WParser->OpenContainer();
            }
            m_WParser->GetContainer()->SetIndent(m_WParser->GetCharHeight(), wxHTML_INDENT_TOP);

            ParseInner(tag);

            if (m_WParser->GetContainer()->GetFirstChild() != NULL)
            {
                m_WParser->CloseContainer();
                m_WParser->OpenContainer();
            }
            m_WParser->GetContainer()->SetIndent(m_WParser->GetCharHeight(), wxHTML_INDENT_TOP);

            return true;
        }
        else if (tag.GetName() == wxT("DT"))
        {
            m_WParser->CloseContainer();
            c = m_WParser->OpenContainer();
            c->SetAlignHor(wxHTML_ALIGN_LEFT);
            c->SetMinHeight(m_WParser->GetCharHeight());
            return false;
        }
        else // "DD"
        {
            m_WParser->CloseContainer();
            c = m_WParser->OpenContainer();
            c->SetIndent(5 * m_WParser->GetCharWidth(), wxHTML_INDENT_LEFT);
            return false;
        }
    }

TAG_HANDLER_END(DEFLIST)


TAGS_MODULE_BEGIN(DefinitionList)

    TAGS_MODULE_ADD(DEFLIST)

TAGS_MODULE_END(DefinitionList)

#endif
