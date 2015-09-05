/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif

///////////////////////////////////////////////////////////////////////////////
// Name:        tests/archive/ziptest.cpp
// Purpose:     Test the zip classes
// Author:      Mike Wetherell
// Copyright:   (c) 2004 Mike Wetherell
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
#   pragma hdrstop
#endif

#ifndef WX_PRECOMP
#   include "wx/wx.h"
#endif

#if wxUSE_STREAMS && wxUSE_ZIPSTREAM

#include "archivetest.h"
#include "wx/zipstrm.h"

using std::string;
using std::auto_ptr;


///////////////////////////////////////////////////////////////////////////////
// ArchiveTestCase<wxZipClassFactory> could be used directly, but instead this
// derived class is used so that zip specific features can be tested.

class ZipTestCase : public ArchiveTestCase<wxZipClassFactory>
{
public:
    ZipTestCase(string name,
                int options,
                const wxString& archiver = wxEmptyString,
                const wxString& unarchiver = wxEmptyString)
    :
        ArchiveTestCase<wxZipClassFactory>(name, new wxZipClassFactory,
                                           options, archiver, unarchiver),
        m_count(0)
    { }

protected:
    void OnCreateArchive(wxZipOutputStream& zip);
    
    void OnArchiveExtracted(wxZipInputStream& zip, int expectedTotal);
    
    void OnCreateEntry(wxZipOutputStream& zip,
                       TestEntry& testEntry,
                       wxZipEntry *entry);
    
    void OnEntryExtracted(wxZipEntry& entry,
                          const TestEntry& testEntry,
                          wxZipInputStream *arc);

    void OnSetNotifier(EntryT& entry);

    int m_count;
    wxString m_comment;
};

void ZipTestCase::OnCreateArchive(wxZipOutputStream& zip)
{
    m_comment << wxT("Comment for test ") << m_id;
    zip.SetComment(m_comment);
}

void ZipTestCase::OnArchiveExtracted(wxZipInputStream& zip, int expectedTotal)
{
    CPPUNIT_ASSERT(zip.GetComment() == m_comment);
    CPPUNIT_ASSERT(zip.GetTotalEntries() == expectedTotal);
}

void ZipTestCase::OnCreateEntry(wxZipOutputStream& zip,
                                TestEntry& testEntry,
                                wxZipEntry *entry)
{
    zip.SetLevel((m_id + m_count) % 10);

    if (entry) {
        switch ((m_id + m_count) % 5) {
            case 0:
            {
                wxString comment = wxT("Comment for ") + entry->GetName();
                entry->SetComment(comment);
                // lowercase the expected result, and the notifier should do
                // the same for the zip entries when ModifyArchive() runs
                testEntry.SetComment(comment.Lower());
                break;
            }
            case 2:
                entry->SetMethod(wxZIP_METHOD_STORE);
                break;
            case 4:
                entry->SetMethod(wxZIP_METHOD_DEFLATE);
                break;
        }
        entry->SetIsText(testEntry.IsText());
    }

    m_count++;
}

void ZipTestCase::OnEntryExtracted(wxZipEntry& entry,
                                   const TestEntry& testEntry,
                                   wxZipInputStream *arc)
{
    // provide some context for the error message so that we know which
    // iteration of the loop we were on
    wxString name = wxT(" '") + entry.GetName() + wxT("'");
    string error_entry(name.mb_str());
    string error_context(" failed for entry" + error_entry);

    CPPUNIT_ASSERT_MESSAGE("GetComment" + error_context,
        entry.GetComment() == testEntry.GetComment());

    // for seekable streams, GetNextEntry() doesn't read the local header so
    // call OpenEntry() to do it
    if (arc && (m_options & PipeIn) == 0 && entry.IsDir())
        arc->OpenEntry(entry);

    CPPUNIT_ASSERT_MESSAGE("IsText" + error_context,
                           entry.IsText() == testEntry.IsText());

    CPPUNIT_ASSERT_MESSAGE("Extra/LocalExtra mismatch for entry" + error_entry,
        (entry.GetExtraLen() != 0 && entry.GetLocalExtraLen() != 0) ||
        (entry.GetExtraLen() == 0 && entry.GetLocalExtraLen() == 0));
}

// check the notifier mechanism by using it to fold the entry comments to
// lowercase
//
class ZipNotifier : public wxZipNotifier
{
public:
    void OnEntryUpdated(wxZipEntry& entry);
};

void ZipNotifier::OnEntryUpdated(wxZipEntry& entry)
{
    entry.SetComment(entry.GetComment().Lower());
}

void ZipTestCase::OnSetNotifier(EntryT& entry)
{
    static ZipNotifier notifier;
    entry.SetNotifier(notifier);
}


///////////////////////////////////////////////////////////////////////////////
// 'zip - -' produces local headers without the size field set. This is a
// case not covered by all the other tests, so this class tests it as a
// special case

class ZipPipeTestCase : public CppUnit::TestCase
{
public:
    ZipPipeTestCase(string name, int options) :
        CppUnit::TestCase(TestId::MakeId() + name),
        m_options(options),
        m_id(TestId::GetId())
    { }

protected:
    void runTest();
    int m_options;
    int m_id;
};

void ZipPipeTestCase::runTest()
{
    TestOutputStream out(m_options);

    wxString testdata = wxT("test data to pipe through zip");
    wxString cmd = wxT("echo ") + testdata + wxT(" | zip -q - -");

    {
        PFileInputStream in(cmd);
        if (in.IsOk())
            out.Write(in);
    }

    TestInputStream in(out, m_id % ((m_options & PipeIn) ? 4 : 3));
    wxZipInputStream zip(in);

    auto_ptr<wxZipEntry> entry(zip.GetNextEntry());
    CPPUNIT_ASSERT(entry.get() != NULL);

    if ((m_options & PipeIn) == 0)
        CPPUNIT_ASSERT(entry->GetSize() != wxInvalidOffset);

    char buf[64];
    size_t len = zip.Read(buf, sizeof(buf) - 1).LastRead();

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    while (len > 0 && buf[len - 1] <= 32)
        --len;
    buf[len] = 0;

    CPPUNIT_ASSERT(zip.Eof());
    CPPUNIT_ASSERT(wxString(buf, *wxConvCurrent) == testdata);
}


///////////////////////////////////////////////////////////////////////////////
// Zip suite 

class ziptest : public ArchiveTestSuite
{
public:
    ziptest();
    static CppUnit::Test *suite() { return (new ziptest)->makeSuite(); }

protected:
    ArchiveTestSuite *makeSuite();

    CppUnit::Test *makeTest(string descr, int options,
                            bool genericInterface, const wxString& archiver,
                            const wxString& unarchiver);
};

ziptest::ziptest()
  : ArchiveTestSuite("zip")
{
    AddArchiver(wxT("zip -qr %s *"));
    AddUnArchiver(wxT("unzip -q %s"));
}

ArchiveTestSuite *ziptest::makeSuite()
{
    ArchiveTestSuite::makeSuite();

#if 0
    // zip doesn't support this any more so disabled
    if (IsInPath(wxT("zip")))
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for (int options = 0; options <= PipeIn; options += PipeIn) {
            string name = Description(wxT("ZipPipeTestCase"), options,
                                      false, wxT(""), wxT("zip -q - -"));
            addTest(new ZipPipeTestCase(name, options));
        }
#endif

    return this;
}

CppUnit::Test *ziptest::makeTest(
    string descr,
    int   options,
    bool  genericInterface,
    const wxString& archiver,
    const wxString& unarchiver)
{
    // unzip doesn't support piping in the zip
    if ((options & PipeIn) && !unarchiver.empty())
        return NULL;

    if (genericInterface)
    {
        return new ArchiveTestCase<wxArchiveClassFactory>(
                            descr, new wxZipClassFactory,
                            options, archiver, unarchiver);
    }

    return new ZipTestCase(descr, options, archiver, unarchiver);
}

CPPUNIT_TEST_SUITE_REGISTRATION(ziptest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ziptest, "archive");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ziptest, "archive/zip");

#endif // wxUSE_STREAMS && wxUSE_ZIPSTREAM
