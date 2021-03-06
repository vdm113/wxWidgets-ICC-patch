/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/colourdata.h
// Author:      Julian Smart
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COLOURDATA_H_
#define _WX_COLOURDATA_H_

#include "wx/colour.h"

class WXDLLIMPEXP_CORE wxColourData : public wxObject
{
public:
    // number of custom colours we store
    enum
    {
        NUM_CUSTOM = 16
    };

    wxColourData();
    wxColourData(const wxColourData& data);
    wxColourData& operator=(const wxColourData& data);
    virtual ~wxColourData();

    void SetChooseFull(bool flag) { m_chooseFull = flag; }
    bool GetChooseFull() const { return m_chooseFull; }
    void SetColour(const wxColour& colour) { m_dataColour = colour; }
    const wxColour& GetColour() const { return m_dataColour; }
    wxColour& GetColour() { return m_dataColour; }

    // SetCustomColour() modifies colours in an internal array of NUM_CUSTOM
    // custom colours;
    void SetCustomColour(int i, const wxColour& colour);
    wxColour GetCustomColour(int i) const;

    // Serialize the object to a string and restore it from it
    wxString ToString() const;
    bool FromString(const wxString& str);


    // public for backwards compatibility only: don't use directly
    wxColour        m_dataColour;
    wxColour        m_custColours[NUM_CUSTOM];
    bool            m_chooseFull;

    wxDECLARE_DYNAMIC_CLASS(wxColourData);
};

#endif // _WX_COLOURDATA_H_
