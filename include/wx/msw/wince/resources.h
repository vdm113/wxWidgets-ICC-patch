/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/wince/resources.h
// Purpose:     identifiers shared between resource compiler and eVC
// Author:      Wlodzimierz ABX Skiba
// Modified by:
// Created:     01.05.2004
// Copyright:   (c) Wlodzimierz Skiba
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// Windows CE dummy menu for SHCreateMenuBar()
#define wxIDM_SHMENU             40000

#define IDM_LEFT                 40001
#define IDM_RIGHT                40002
#define IDM_ITEM                 40003

#define IDS_EMPTY                40010

#define IDR_POPUP_1              40020
#define IDR_POPUP_2              40021

#define IDR_MENUBAR_ONE_BUTTON   40030
#define IDR_MENUBAR_LEFT_MENU    40031
#define IDR_MENUBAR_RIGHT_MENU   40032
#define IDR_MENUBAR_BOTH_MENUS   40033
