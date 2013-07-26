///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/selstore.cpp
// Purpose:     wxSelectionStore implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     08.06.03 (extracted from src/generic/listctrl.cpp)
// Copyright:   (c) 2000-2003 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/selstore.h"

// ============================================================================
// wxSelectionStore
// ============================================================================

// ----------------------------------------------------------------------------
// tests
// ----------------------------------------------------------------------------

bool wxSelectionStore::IsSelected(unsigned item) const
{
    bool isSel = m_itemsSel.Index(item) != wxNOT_FOUND;

    // if the default state is to be selected, being in m_itemsSel means that
    // the item is not selected, so we have to inverse the logic
    return m_defaultState ? !isSel : isSel;
}

// ----------------------------------------------------------------------------
// Select*()
// ----------------------------------------------------------------------------

bool wxSelectionStore::SelectItem(unsigned item, bool select)
{
    // search for the item ourselves as like this we get the index where to
    // insert it later if needed, so we do only one search in the array instead
    // of two (adding item to a sorted array requires a search)
    size_t index = m_itemsSel.IndexForInsert(item);
    bool isSel = index < m_itemsSel.GetCount() && m_itemsSel[index] == item;

    if ( select != m_defaultState )
    {
        if ( !isSel )
        {
            m_itemsSel.AddAt(item, index);

            return true;
        }
    }
    else // reset to default state
    {
        if ( isSel )
        {
            m_itemsSel.RemoveAt(index);
            return true;
        }
    }

    return false;
}

bool wxSelectionStore::SelectRange(unsigned itemFrom, unsigned itemTo,
                                   bool select,
                                   wxArrayInt *itemsChanged)
{
    // 100 is hardcoded but it shouldn't matter much: the important thing is
    // that we don't refresh everything when really few (e.g. 1 or 2) items
    // change state
    static const unsigned MANY_ITEMS = 100;

    wxASSERT_MSG( itemFrom <= itemTo, wxT("should be in order") );

    // are we going to have more [un]selected items than the other ones?
    if ( itemTo - itemFrom > m_count/2 )
    {
        if ( select != m_defaultState )
        {
            // the default state now becomes the same as 'select'
            m_defaultState = select;

            // so all the old selections (which had state select) shouldn't be
            // selected any more, but all the other ones should
            wxSelectedIndices selOld = m_itemsSel;
            m_itemsSel.Empty();

            // TODO: it should be possible to optimize the searches a bit
            //       knowing the possible range

            unsigned item;
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            for ( item = 0; item < itemFrom; item++ )
            {
                if ( selOld.Index(item) == wxNOT_FOUND )
                    m_itemsSel.Add(item);
            }

#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
            for ( item = itemTo + 1; item < m_count; item++ )
            {
                if ( selOld.Index(item) == wxNOT_FOUND )
                    m_itemsSel.Add(item);
            }

            // many items (> half) changed state
            itemsChanged = NULL;
        }
        else // select == m_defaultState
        {
            // get the inclusive range of items between itemFrom and itemTo
            size_t count = m_itemsSel.GetCount(),
                   start = m_itemsSel.IndexForInsert(itemFrom),
                   end = m_itemsSel.IndexForInsert(itemTo);

            if ( start == count || m_itemsSel[start] < itemFrom )
            {
                start++;
            }

            if ( end == count || m_itemsSel[end] > itemTo )
            {
                end--;
            }

            if ( start <= end )
            {
                // delete all of them (from end to avoid changing indices)
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
                for ( int i = end; i >= (int)start; i-- )
                {
                    if ( itemsChanged )
                    {
                        if ( itemsChanged->GetCount() > MANY_ITEMS )
                        {
                            // stop counting (see comment below)
                            itemsChanged = NULL;
                        }
                        else
                        {
                            itemsChanged->Add(m_itemsSel[i]);
                        }
                    }

                    m_itemsSel.RemoveAt(i);
                }
            }
        }
    }
    else // "few" items change state
    {
        if ( itemsChanged )
        {
            itemsChanged->Empty();
        }

        // just add the items to the selection
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        for ( unsigned item = itemFrom; item <= itemTo; item++ )
        {
            if ( SelectItem(item, select) && itemsChanged )
            {
                itemsChanged->Add(item);

                if ( itemsChanged->GetCount() > MANY_ITEMS )
                {
                    // stop counting them, we'll just eat gobs of memory
                    // for nothing at all - faster to refresh everything in
                    // this case
                    itemsChanged = NULL;
                }
            }
        }
    }

    // we set it to NULL if there are many items changing state
    return itemsChanged != NULL;
}

// ----------------------------------------------------------------------------
// callbacks
// ----------------------------------------------------------------------------

void wxSelectionStore::OnItemDelete(unsigned item)
{
    size_t count = m_itemsSel.GetCount(),
           i = m_itemsSel.IndexForInsert(item);

    if ( i < count && m_itemsSel[i] == item )
    {
        // this item itself was in m_itemsSel, remove it from there
        m_itemsSel.RemoveAt(i);

        count--;
    }

    // and adjust the index of all which follow it
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
    while ( i < count )
    {
        // all following elements must be greater than the one we deleted
        wxASSERT_MSG( m_itemsSel[i] > item, wxT("logic error") );

        m_itemsSel[i++]--;
    }
}

void wxSelectionStore::SetItemCount(unsigned count)
{
    // forget about all items whose indices are now invalid if the size
    // decreased
    if ( count < m_count )
    {
#if defined(__INTEL_COMPILER)
#   pragma ivdep
#endif
        for ( size_t i = m_itemsSel.GetCount(); i > 0; i-- )
        {
            if ( m_itemsSel[i - 1] >= count )
                m_itemsSel.RemoveAt(i - 1);
        }
    }

    // remember the new number of items
    m_count = count;
}
