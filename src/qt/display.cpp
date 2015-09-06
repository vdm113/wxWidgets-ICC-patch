/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/display.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/display.h"
#include "wx/display_impl.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include "wx/qt/private/converter.h"

class wxDisplayImplQt : public wxDisplayImpl
{
public:
    wxDisplayImplQt( unsigned n );

    virtual wxRect GetGeometry() const;
    virtual wxString GetName() const;

    virtual wxArrayVideoModes GetModes(const wxVideoMode& mode) const;
    virtual wxVideoMode GetCurrentMode() const;
    virtual bool ChangeMode(const wxVideoMode& mode);
};

wxDisplayImplQt::wxDisplayImplQt( unsigned n )
    : wxDisplayImpl( n )
{
}

wxRect wxDisplayImplQt::GetGeometry() const
{
    return wxQtConvertRect( QApplication::desktop()->screenGeometry( GetIndex() ));
}

wxString wxDisplayImplQt::GetName() const
{
    return wxString();
}

wxArrayVideoModes wxDisplayImplQt::GetModes(const wxVideoMode& WXUNUSED(mode)) const
{
    return wxArrayVideoModes();
}

wxVideoMode wxDisplayImplQt::GetCurrentMode() const
{
    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();
    int depth = QApplication::desktop()->depth();

    return wxVideoMode( width, height, depth );
}

bool wxDisplayImplQt::ChangeMode(const wxVideoMode& WXUNUSED(mode))
{
    return false;
}


//##############################################################################

class wxDisplayFactoryQt : public wxDisplayFactory
{
public:
    wxDisplayFactoryQt();

    virtual wxDisplayImpl *CreateDisplay(unsigned n);
    virtual unsigned GetCount();
    virtual int GetFromPoint(const wxPoint& pt);
};

    
wxDisplayFactoryQt::wxDisplayFactoryQt()
{
}

wxDisplayImpl *wxDisplayFactoryQt::CreateDisplay(unsigned n)
{
    return new wxDisplayImplQt( n );
}

unsigned wxDisplayFactoryQt::GetCount()
{
    return QApplication::desktop()->screenCount();
}

int wxDisplayFactoryQt::GetFromPoint(const wxPoint& pt)
{
    return QApplication::desktop()->screenNumber( wxQtConvertPoint( pt ));
}

//##############################################################################

/* static */ wxDisplayFactory *wxDisplay::CreateFactory()
{
    return new wxDisplayFactoryQt;
}
