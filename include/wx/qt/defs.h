/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/*
 *  Name:        wx/qt/defs.h
 *  Author:      Peter Most
 *  Copyright:   (c) Peter Most
 *  Licence:     wxWindows licence
 */

#ifndef _WX_QT_DEFS_H_
#define _WX_QT_DEFS_H_

#ifdef __cplusplus

typedef class QWidget *WXWidget;

#endif 

#endif /* _WX_QT_DEFS_H_ */
