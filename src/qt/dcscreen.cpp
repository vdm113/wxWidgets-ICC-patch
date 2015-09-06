/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/dcscreen.cpp
// Author:      Sean D'Epagnier
// Copyright:   (c) Sean D'Epagnier
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/dcscreen.h"
#include "wx/qt/dcscreen.h"

#include <QtWidgets/QDesktopWidget>
#include <QtGui/QScreen>
#include <QPixmap>

wxIMPLEMENT_ABSTRACT_CLASS(wxScreenDCImpl, wxWindowDCImpl);

wxScreenDCImpl::wxScreenDCImpl( wxScreenDC *owner )
    : wxWindowDCImpl( owner )
{
    m_qtImage = new QImage(QApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId()).toImage());
}

wxScreenDCImpl::~wxScreenDCImpl( )
{
    delete m_qtImage;
}

void wxScreenDCImpl::DoGetSize(int *width, int *height) const
{
    wxDisplaySize(width, height);
}
