/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        reader.h
// Purpose:     Life! pattern reader (writer coming soon)
// Author:      Guillermo Rodriguez Garcia, <guille@iies.es>
// Modified by:
// Created:     Jan/2000
// Copyright:   (c) 2000, Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _LIFE_READER_H_
#define _LIFE_READER_H_

#include "game.h"

// --------------------------------------------------------------------------
// LifeReader
// --------------------------------------------------------------------------

class LifeReader
{
public:
    LifeReader(wxInputStream& is);

    inline bool          IsOk() const           { return m_ok; };
    inline wxString      GetDescription() const { return m_description; };
    inline wxString      GetRules() const       { return m_rules; };
    inline wxArrayString GetShape() const       { return m_shape; };
    inline LifePattern   GetPattern() const
    {
        return LifePattern(wxEmptyString, m_description, m_rules, m_shape);
    };

private:
    bool             m_ok;
    wxString         m_description;
    wxString         m_rules;
    wxArrayString    m_shape;
};

#endif // _LIFE_READER_H_
