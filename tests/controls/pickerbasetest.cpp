/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/pickerbasetest.cpp
// Purpose:     wxPickerBase unit test
// Author:      Steven Lamerton
// Created:     2010-08-07
// Copyright:   (c) 2010 Steven Lamerton
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#include "wx/pickerbase.h"
#include "pickerbasetest.h"

void PickerBaseTestCase::Margin()
{
    wxPickerBase* const base = GetBase();

    CPPUNIT_ASSERT(base->HasTextCtrl());
    CPPUNIT_ASSERT(base->GetInternalMargin() >= 0);

    base->SetInternalMargin(15);

    CPPUNIT_ASSERT_EQUAL(15, base->GetInternalMargin());
}

void PickerBaseTestCase::Proportion()
{
    wxPickerBase* const base = GetBase();

    CPPUNIT_ASSERT(base->HasTextCtrl());

    base->SetPickerCtrlProportion(1);
    base->SetTextCtrlProportion(1);

    CPPUNIT_ASSERT_EQUAL(1, base->GetPickerCtrlProportion());
    CPPUNIT_ASSERT_EQUAL(1, base->GetTextCtrlProportion());
}

void PickerBaseTestCase::Growable()
{
    wxPickerBase* const base = GetBase();

    CPPUNIT_ASSERT(base->HasTextCtrl());

    base->SetPickerCtrlGrowable();
    base->SetTextCtrlGrowable();

    CPPUNIT_ASSERT(base->IsPickerCtrlGrowable());
    CPPUNIT_ASSERT(base->IsTextCtrlGrowable());

    base->SetPickerCtrlGrowable(false);
    base->SetTextCtrlGrowable(false);

    CPPUNIT_ASSERT(!base->IsPickerCtrlGrowable());
    CPPUNIT_ASSERT(!base->IsTextCtrlGrowable());
}

void PickerBaseTestCase::Controls()
{
    wxPickerBase* const base = GetBase();

    CPPUNIT_ASSERT(base->HasTextCtrl());
    CPPUNIT_ASSERT(base->GetTextCtrl() != NULL);
    CPPUNIT_ASSERT(base->GetPickerCtrl() != NULL);
}
