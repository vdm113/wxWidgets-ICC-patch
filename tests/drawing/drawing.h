/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        tests/drawing/drawing.h
// Purpose:     Drawing test case header
// Author:      Armel Asselin
// Created:     2014-02-26
// Copyright:   (c) 2014 Ellié Computing <opensource@elliecomputing.com>
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TESTS_DRAWING_H_
#define _WX_TESTS_DRAWING_H_

#include "gcfactory.h"

#if wxUSE_TEST_GC_DRAWING

#if wxUSE_SVG
#include "wx/dcsvg.h"
#endif

class WXDLLIMPEXP_FWD_BASE wxDynamicLibrary;

class GraphicsContextDrawingTestCase : public CppUnit::TestCase
{
public:
    GraphicsContextDrawingTestCase()
        : m_drawingPluginsLoaded(false)
    {
        wxImage::AddHandler (new wxPNGHandler());
        wxImage::AddHandler (new wxBMPHandler());
    }
    ~GraphicsContextDrawingTestCase() {
        ms_referenceDirectory.clear();
    }

private:

    // NB: individual test cases launchers are copied/pasted so that the CppUnit
    //  test case selection can be used

    CPPUNIT_TEST_SUITE( GraphicsContextDrawingTestCase );
        CPPUNIT_TEST( DrawToImage_Basics );
#if wxUSE_SVG
//        CPPUNIT_TEST( DrawToSVG_Basics );
#endif
        CPPUNIT_TEST( DrawToPlugins_Basics );
   CPPUNIT_TEST_SUITE_END();

    class ImageGraphicsContextLifeCycle: public DrawingTestGCFactory
    {
    public:
        virtual ~ImageGraphicsContextLifeCycle() {}
        virtual wxString GetIdForFileName () const { return "image"; }
        virtual wxString GetExtensionForFileName () const { return "png"; }
        virtual bool UseImageComparison() const { return true; }
        virtual bool PlatformIndependent() const { return false; }
        virtual wxGraphicsContext *BuildNewContext (wxSize expectedSize,
            double pointsPerInch, const wxFileName &targetFileName);
        virtual void SaveBuiltContext (wxGraphicsContext *&gc);
        virtual void CleanUp (wxGraphicsContext *gc);
    private:
        wxImage *m_image;
        wxString m_targetFileName;
    };

    static ImageGraphicsContextLifeCycle ms_imageLifeCycle;

#if wxUSE_SVG
    class SvgGraphicsContextLifeCycle: public DrawingTestGCFactory
    {
    public:
        virtual wxString GetIdForFileName () const { return "svg"; }
        virtual wxString GetExtensionForFileName () const { return "svg"; }
        virtual bool UseImageComparison() const { return false; }
        virtual bool PlatformIndependent() const { return true; }
        virtual wxGraphicsContext *BuildNewContext (wxSize expectedSize,
            double pointsPerInch, const wxFileName &targetFileName);
        virtual void SaveBuiltContext (wxGraphicsContext *&gc);
        virtual void CleanUp (wxGraphicsContext *gc);

    private:
        wxSVGFileDC *m_svgFileDc;
    };

    static SvgGraphicsContextLifeCycle ms_svgLifeCycle;
#endif // wxUSE_SVG

    struct DrawingTestCase {
        unsigned caseNumber;
        void (GraphicsContextDrawingTestCase::*m_drawingF) (
            wxGraphicsContext *gc);
        unsigned width, height;
        double pointsPerInch;

        // if true, it means that this test case is "simple" enough to be
        //  platform independent even for targets that normally are dependent
        //  on the platform
        bool platformIndependent;
    };

    // test cases
    static const DrawingTestCase ms_drawingBasicTc;

    // cases functions
    void DoBasicDrawings (wxGraphicsContext *gc);

    void RunIndividualDrawingCase (
        DrawingTestGCFactory& gcFactory,
        const DrawingTestCase & testCase);

    // enumerates the dll names as specified in WX_TEST_SUITE_GC_DRAWING_PLUGINS
    //  (coma separated list of DLL to load and test)
    // each DLL should have these procedures:
    //  - DrawingTestGCFactory* CreateDrawingTestLifeCycle(),
    //          to create the life cycle object
    //  - DestroyDrawingTestLifeCycle(DrawingTestGCFactory *),
    //          to destroy the life cycle object
    void RunPluginsDrawingCase (const DrawingTestCase & testCase);

    void DrawToImage_Basics() {
        RunIndividualDrawingCase (ms_imageLifeCycle, ms_drawingBasicTc);
    }
#if wxUSE_SVG
    void DrawToSVG_Basics() {
        RunIndividualDrawingCase (ms_svgLifeCycle, ms_drawingBasicTc);
    }
#endif
    void DrawToPlugins_Basics() {
        RunPluginsDrawingCase (ms_drawingBasicTc);
    }

    bool GetBuildReference() const;

    wxString GetTestsReferenceDirectory() const;

    wxString GetPlatformTag() const;

    static wxString ms_referenceDirectory;
    static bool ms_buildReference, ms_buildReferenceDetermined;
    bool m_drawingPluginsLoaded;

    struct PluginInfo {
        PluginInfo();
        ~PluginInfo();

        wxDynamicLibrary* library;

        DrawingTestGCFactory *gcFactory;
        DestroyDrawingTestLifeCycleFunction destroy;
    };

    wxVector<PluginInfo> m_drawingPlugins;

    DECLARE_NO_COPY_CLASS(GraphicsContextDrawingTestCase)
};


#endif // wxUSE_TEST_GC_DRAWING

#endif // _WX_TESTS_DRAWING_H_
