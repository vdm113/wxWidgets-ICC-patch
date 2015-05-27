/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/dataobj2.h
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DATAOBJ2_H_
#define _WX_QT_DATAOBJ2_H_

class WXDLLIMPEXP_CORE wxBitmapDataObject : public wxBitmapDataObjectBase
{
public:
    wxBitmapDataObject();
    wxBitmapDataObject(const wxBitmap& bitmap);

protected:

private:
};



class WXDLLIMPEXP_CORE wxFileDataObject : public wxFileDataObjectBase
{
public:
    wxFileDataObject();

    void AddFile( const wxString &filename );
};

#endif // _WX_QT_DATAOBJ2_H_
