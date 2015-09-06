/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/accel.cpp
// Author:      Peter Most, Javier Torres, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/accel.h"
#include "wx/list.h"
#include "wx/qt/private/converter.h"
#include <QtCore/QVariant>

// ----------------------------------------------------------------------------
// wxAccelList: a list of wxAcceleratorEntries
// ----------------------------------------------------------------------------

WX_DECLARE_LIST(wxAcceleratorEntry, wxAccelList);
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxAccelList)

// ----------------------------------------------------------------------------
// wxAccelRefData: the data used by wxAcceleratorTable
// ----------------------------------------------------------------------------

class wxAccelRefData : public wxObjectRefData
{
    public:
        wxAccelRefData()
        {
        }
        
        wxAccelRefData(const wxAccelRefData& data)
        : wxObjectRefData()
        {
            m_accels = data.m_accels;
        }
        
        virtual ~wxAccelRefData()
        {
            WX_CLEAR_LIST(wxAccelList, m_accels);
        }
        
        wxAccelList m_accels;
};

// macro which can be used to access wxAccelRefData from wxAcceleratorTable
#define M_ACCELDATA ((wxAccelRefData *)m_refData)


// ============================================================================
// implementation
// ============================================================================

wxIMPLEMENT_DYNAMIC_CLASS( wxAcceleratorTable, wxObject );

QShortcut *ConvertAccelerator( wxAcceleratorEntry *e, QWidget *parent )
{
    // TODO: Not all keys have the same string representation in wx and qt
    QShortcut *s = new QShortcut( wxQtConvertString( e->ToString() ), parent );
    
    // Set a property to save wx Command to send when activated
    s->setProperty( "wxQt_Command", e->GetCommand() );

    return s;
}

wxAcceleratorTable::wxAcceleratorTable()
{
}

wxAcceleratorTable::wxAcceleratorTable(int n, const wxAcceleratorEntry entries[])
{
    m_refData = new wxAccelRefData;
    
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for ( int i = 0; i < n; i++ )
    {
        M_ACCELDATA->m_accels.Append( new wxAcceleratorEntry( entries[i] ) );
    }
}

QList< QShortcut* > wxAcceleratorTable::ConvertShortcutTable( QWidget *parent ) const
{   
    QList< QShortcut* > qtList;
    
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for ( wxAccelList::Node *node = M_ACCELDATA->m_accels.GetFirst(); node; node = node->GetNext() )
    {
        qtList << ConvertAccelerator( node->GetData(), parent );
    }
    
    return qtList;
}

wxObjectRefData *wxAcceleratorTable::CreateRefData() const
{
    return new wxAccelRefData;
}

wxObjectRefData *wxAcceleratorTable::CloneRefData(const wxObjectRefData *data) const
{
    return new wxAccelRefData(*(wxAccelRefData *)data);
}

bool wxAcceleratorTable::IsOk() const
{
    return (m_refData != NULL);
}
