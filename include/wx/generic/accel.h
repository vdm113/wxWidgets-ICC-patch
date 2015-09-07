/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/accel.h
// Purpose:     wxAcceleratorTable class
// Author:      Robert Roebling
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_ACCEL_H_
#define _WX_GENERIC_ACCEL_H_

class WXDLLIMPEXP_FWD_CORE wxKeyEvent;

// ----------------------------------------------------------------------------
// wxAcceleratorTable
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxAcceleratorTable : public wxObject
{
public:
    wxAcceleratorTable();
    wxAcceleratorTable(int n, const wxAcceleratorEntry entries[]);
    virtual ~wxAcceleratorTable();

    bool Ok() const { return IsOk(); }
    bool IsOk() const;

    void Add(const wxAcceleratorEntry& entry);
    void Remove(const wxAcceleratorEntry& entry);

    // implementation
    // --------------

    wxMenuItem *GetMenuItem(const wxKeyEvent& event) const;
    int GetCommand(const wxKeyEvent& event) const;

    const wxAcceleratorEntry *GetEntry(const wxKeyEvent& event) const;

protected:
    // ref counting code
    virtual wxObjectRefData *CreateRefData() const wxOVERRIDE;
    virtual wxObjectRefData *CloneRefData(const wxObjectRefData *data) const wxOVERRIDE;

private:
    wxDECLARE_DYNAMIC_CLASS(wxAcceleratorTable);
};

#endif // _WX_GENERIC_ACCEL_H_

