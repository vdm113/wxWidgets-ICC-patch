/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        server.h
// Purpose:     DDE sample: server
// Author:      Julian Smart
// Modified by:
// Created:     25/01/99
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "connection.h"

enum
{
    ID_START = 10000,
    ID_DISCONNECT,
    ID_ADVISE,
    ID_SERVERNAME,
};

// Define a new application
class MyServer;
class MyFrame;

class MyApp : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    MyFrame *GetFrame() { return m_frame; }

protected:
    MyFrame *m_frame;
};

wxDECLARE_APP(MyApp);

// Define a new frame
class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString& title);

    void OnClose(wxCloseEvent& event);

    void UpdateUI();
    void Disconnect();

protected:
    wxButton* GetStart()  { return (wxButton*) FindWindow( ID_START ); }
    wxChoice* GetServername()  { return (wxChoice*) FindWindow( ID_SERVERNAME ); }
    wxButton* GetDisconnect()  { return (wxButton*) FindWindow( ID_DISCONNECT ); }
    wxButton* GetAdvise()  { return (wxButton*) FindWindow( ID_ADVISE ); }


    MyServer *m_server;

    void OnStart( wxCommandEvent &event );
    void OnServerName( wxCommandEvent &event );
    void OnDisconnect( wxCommandEvent &event );
    void OnAdvise( wxCommandEvent &event );

    wxDECLARE_EVENT_TABLE();
};

class MyConnection : public MyConnectionBase
{
public:
    virtual bool OnExecute(const wxString& topic, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual const void *OnRequest(const wxString& topic, const wxString& item, size_t *size, wxIPCFormat format) wxOVERRIDE;
    virtual bool OnPoke(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual bool OnStartAdvise(const wxString& topic, const wxString& item) wxOVERRIDE;
    virtual bool OnStopAdvise(const wxString& topic, const wxString& item) wxOVERRIDE;
    virtual bool DoAdvise(const wxString& item, const void *data, size_t size, wxIPCFormat format) wxOVERRIDE;
    virtual bool OnDisconnect() wxOVERRIDE;

    // topic for which we advise the client or empty if none
    wxString m_advise;

protected:
    // the data returned by last OnRequest(): we keep it in this buffer to
    // ensure that the pointer we return from OnRequest() stays valid
    wxCharBuffer m_requestData;
};

class MyServer : public wxServer
{
public:
    MyServer();
    virtual ~MyServer();

    void Disconnect();
    bool IsConnected() { return m_connection != NULL; }
    MyConnection *GetConnection() { return m_connection; }

    void Advise();
    bool CanAdvise() { return m_connection && !m_connection->m_advise.empty(); }

    virtual wxConnectionBase *OnAcceptConnection(const wxString& topic) wxOVERRIDE;

protected:
    MyConnection *m_connection;
};

