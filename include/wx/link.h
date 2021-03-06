/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/link.h
// Purpose:     macros to force linking modules which might otherwise be
//              discarded by the linker
// Author:      Vaclav Slavik
// Copyright:   (c) Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_LINK_H_
#define _WX_LINK_H_

// This must be part of the module you want to force:
#define wxFORCE_LINK_THIS_MODULE(module_name)                         \
                extern void _wx_link_dummy_func_##module_name ();     \
                void _wx_link_dummy_func_##module_name () { }


// And this must be somewhere where it certainly will be linked:
#define wxFORCE_LINK_MODULE(module_name)                              \
                extern void _wx_link_dummy_func_##module_name ();     \
                static struct wxForceLink##module_name                \
                {                                                     \
                    wxForceLink##module_name()                        \
                    {                                                 \
                        _wx_link_dummy_func_##module_name ();         \
                    }                                                 \
                } _wx_link_dummy_var_##module_name;


#endif // _WX_LINK_H_
