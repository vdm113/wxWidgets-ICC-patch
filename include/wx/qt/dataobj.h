/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/dataobj.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_DATAOBJ_H_
#define _WX_QT_DATAOBJ_H_

#include <QMimeData>

class WXDLLIMPEXP_CORE wxDataObject : public wxDataObjectBase
{
public:
    wxDataObject() {}
    
    virtual bool IsSupportedFormat(const wxDataFormat& format, Direction dir) const;
    virtual wxDataFormat GetPreferredFormat(Direction dir = Get) const;
    virtual size_t GetFormatCount(Direction dir = Get) const;
    virtual void GetAllFormats(wxDataFormat *formats, Direction dir = Get) const;
    virtual size_t GetDataSize(const wxDataFormat& format) const;
    virtual bool GetDataHere(const wxDataFormat& format, void *buf) const;
    virtual bool SetData(const wxDataFormat& format, size_t len, const void * buf);

private:
    QMimeData m_qtMimeData; // to handle formats that have no helper classes
};

#endif // _WX_QT_DATAOBJ_H_
