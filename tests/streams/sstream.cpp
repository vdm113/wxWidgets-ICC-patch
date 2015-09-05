/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        tests/streams/sstream.cpp
// Purpose:     Test wxStringInputStream/wxStringOutputStream
// Author:      Vadim Zeitlin
// Copyright:   (c) 2004 Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
// and "wx/cppunit.h"
#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#endif

#include "wx/sstream.h"

#include "bstream.h"

///////////////////////////////////////////////////////////////////////////////
// The test case
//
// Try to fully test wxStringInputStream and wxStringOutputStream

class strStream :
        public BaseStreamTestCase<wxStringInputStream, wxStringOutputStream>
{
public:
    strStream();
    virtual ~strStream();

    CPPUNIT_TEST_SUITE(strStream);
        // Base class stream tests the strStream supports.
        CPPUNIT_TEST(Input_GetSize);
        CPPUNIT_TEST(Input_GetC);
        CPPUNIT_TEST(Input_Read);
        CPPUNIT_TEST(Input_Eof);
        CPPUNIT_TEST(Input_LastRead);
        CPPUNIT_TEST(Input_CanRead);
        CPPUNIT_TEST(Input_SeekI);
        CPPUNIT_TEST(Input_TellI);
        CPPUNIT_TEST(Input_Peek);
        CPPUNIT_TEST(Input_Ungetch);

        CPPUNIT_TEST(Output_PutC);
        CPPUNIT_TEST(Output_Write);
        CPPUNIT_TEST(Output_LastWrite);
        // seeking currently not supported by output string stream
        //CPPUNIT_TEST(Output_SeekO);
        //CPPUNIT_TEST(Output_TellO);

        // Other test specific for String stream test case.
        CPPUNIT_TEST(Output_Check);
    CPPUNIT_TEST_SUITE_END();

protected:
    void Output_Check();

private:
    // Implement base class functions.
    virtual wxStringInputStream  *DoCreateInStream();
    virtual wxStringOutputStream *DoCreateOutStream();

    // output the given string to wxStringOutputStream and check that its
    // contents is exactly the same string
    void CheckString(const wxString& text);

    wxString m_str;
};

strStream::strStream()
{
    static const size_t LEN = 256;
    m_str.reserve(LEN);
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for ( size_t n = 0; n < LEN; n++ )
    {
        m_str += wxChar(wxT('A') + n % (wxT('Z') - wxT('A') + 1));
    }
}

strStream::~strStream()
{
}

wxStringInputStream *strStream::DoCreateInStream()
{
    wxStringInputStream *pStrInStream = new wxStringInputStream(m_str);
    CPPUNIT_ASSERT(pStrInStream->IsOk());
    return pStrInStream;
}

wxStringOutputStream *strStream::DoCreateOutStream()
{
    wxStringOutputStream *pStrOutStream = new wxStringOutputStream();
    CPPUNIT_ASSERT(pStrOutStream->IsOk());
    return pStrOutStream;
}

void strStream::CheckString(const wxString& text)
{
    wxStringOutputStream sos;

    const wxCharBuffer buf(text.To8BitData());
    sos.Write(buf, buf.length());

    CPPUNIT_ASSERT_EQUAL( text, sos.GetString() );
}

void strStream::Output_Check()
{
    CheckString("Hello world!");
    CheckString(wxString("hi\0dden", 8));
}

// Register the stream sub suite, by using some stream helper macro.
STREAM_TEST_SUBSUITE_NAMED_REGISTRATION(strStream)
