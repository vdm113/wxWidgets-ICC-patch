/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        mfctest.h
// Purpose:     Sample to demonstrate mixing MFC and wxWidgets code
// Author:      Julian Smart
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __MFCTEST_H__
#define __MFCTEST_H__

/////////////////////////////////////////////////////////////////////////////

// CMainWindow:
// See hello.cpp for the code to the member functions and the message map.
//
class CMainWindow : public CFrameWnd
{
public:
    CMainWindow();

    //{{AFX_MSG( CMainWindow )
    afx_msg void OnPaint();
    afx_msg void OnAbout();
    afx_msg void OnTest();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()

private:
    class wxNativeContainerWindow* m_containerWX;
};

// A dummy CWnd pointing to a wxWindow's HWND
class CDummyWindow: public CWnd
{
  public:
    CDummyWindow(HWND hWnd);
    ~CDummyWindow(void);
};

/////////////////////////////////////////////////////////////////////////////

// CTheApp:
//
class CTheApp : public CWinApp
{
public:
    BOOL InitInstance();
    int ExitInstance();

    // Override this to provide wxWidgets message loop
    // compatibility
    BOOL PreTranslateMessage(MSG *msg);
    BOOL OnIdle(LONG lCount);
};

/////////////////////////////////////////////////////////////////////////////

#endif // __MFCTEST_H__
