/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/xrc/xh_chckl.cpp
// Purpose:     XRC resource for wxCheckListBox
// Author:      Bob Mitchell
// Created:     2000/03/21
// Copyright:   (c) 2000 Bob Mitchell and Verant Interactive
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_XRC && wxUSE_CHECKLISTBOX

#include "wx/xrc/xh_chckl.h"

#ifndef WX_PRECOMP
    #include "wx/intl.h"
    #include "wx/log.h"
    #include "wx/checklst.h"
#endif

#include "wx/xml/xml.h"

wxIMPLEMENT_DYNAMIC_CLASS(wxCheckListBoxXmlHandler, wxXmlResourceHandler);

wxCheckListBoxXmlHandler::wxCheckListBoxXmlHandler()
: wxXmlResourceHandler(), m_insideBox(false)
{
    // wxListBox styles:
    XRC_ADD_STYLE(wxLB_SINGLE);
    XRC_ADD_STYLE(wxLB_MULTIPLE);
    XRC_ADD_STYLE(wxLB_EXTENDED);
    XRC_ADD_STYLE(wxLB_HSCROLL);
    XRC_ADD_STYLE(wxLB_ALWAYS_SB);
    XRC_ADD_STYLE(wxLB_NEEDED_SB);
    XRC_ADD_STYLE(wxLB_SORT);

    AddWindowStyles();
}

wxObject *wxCheckListBoxXmlHandler::DoCreateResource()
{
    if (m_class == wxT("wxCheckListBox"))
    {
        // need to build the list of strings from children
        m_insideBox = true;
        CreateChildrenPrivately(NULL, GetParamNode(wxT("content")));

        XRC_MAKE_INSTANCE(control, wxCheckListBox)

        control->Create(m_parentAsWindow,
                        GetID(),
                        GetPosition(), GetSize(),
                        strList,
                        GetStyle(),
                        wxDefaultValidator,
                        GetName());

        // step through children myself (again.)
        wxXmlNode *n = GetParamNode(wxT("content"));
        if (n)
            n = n->GetChildren();
        int i = 0;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
        while (n)
        {
            if (n->GetType() != wxXML_ELEMENT_NODE ||
                n->GetName() != wxT("item"))
               { n = n->GetNext(); continue; }

            // checking boolean is a bit ugly here (see GetBool() )
            wxString v = n->GetAttribute(wxT("checked"), wxEmptyString);
            v.MakeLower();
            if (v == wxT("1"))
                control->Check( i, true );

            i++;
            n = n->GetNext();
        }

        SetupWindow(control);

        strList.Clear();    // dump the strings

        return control;
    }
    else
    {
        // on the inside now.
        // handle <item checked="boolean">Label</item>

        // add to the list
        wxString str = GetNodeContent(m_node);
        if (m_resource->GetFlags() & wxXRC_USE_LOCALE)
            str = wxGetTranslation(str, m_resource->GetDomain());
        strList.Add(str);
        return NULL;
    }
}

bool wxCheckListBoxXmlHandler::CanHandle(wxXmlNode *node)
{
    return (IsOfClass(node, wxT("wxCheckListBox")) ||
           (m_insideBox && node->GetName() == wxT("item")));
}

#endif // wxUSE_XRC && wxUSE_CHECKLISTBOX
