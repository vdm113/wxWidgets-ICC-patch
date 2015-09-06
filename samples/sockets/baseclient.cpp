/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        samples/sockbase/client.cpp
// Purpose:     Sockets sample for wxBase
// Author:      Lukasz Michalski
// Modified by:
// Created:     27.06.2005
// Copyright:   (c) 2005 Lukasz Michalski <lmichalski@sf.net>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wx.h"
#include "wx/socket.h"
#include "wx/event.h"
#include "wx/list.h"
#include "wx/cmdline.h"
#include "wx/ffile.h"
#include "wx/datetime.h"
#include "wx/timer.h"
#include "wx/thread.h"

const wxEventType wxEVT_WORKER = wxNewEventType();
#define EVT_WORKER(func) DECLARE_EVENT_TABLE_ENTRY( wxEVT_WORKER, -1, -1, (wxObjectEventFunction) (wxEventFunction) (WorkerEventFunction) & func, (wxObject *) NULL ),

const int timeout_val = 1000;

class WorkerEvent : public wxEvent {
public:
    typedef enum {
        CONNECTING,
        SENDING,
        RECEIVING,
        DISCONNECTING,
        DONE
    } evt_type;
    WorkerEvent(void* pSender, evt_type type)
    {
        SetId(-1);
        SetEventType(wxEVT_WORKER);
        m_sender = pSender;
        m_eventType = type;
        m_isFailed = false;
    }

    void setFailed() { m_isFailed = true; }
    bool isFailed() const { return m_isFailed; }

    virtual wxEvent* Clone() const wxOVERRIDE
    {
        return new WorkerEvent(*this);
    }
    void* m_sender;
    bool m_isFailed;
    wxString m_workerIdent;
    evt_type m_eventType;
};

typedef void (wxEvtHandler::*WorkerEventFunction)(WorkerEvent&);

class ThreadWorker;
class EventWorker;

WX_DECLARE_LIST(ThreadWorker, TList);
WX_DECLARE_LIST(EventWorker, EList);

class Client : public wxApp {
    wxDECLARE_EVENT_TABLE();
public:
    void RemoveEventWorker(EventWorker* p_worker);
private:
    typedef enum
    {
      THREADS,
      EVENTS
    } workMode;

    typedef enum
    {
      SEND_RANDOM,
      SEND_MESSAGE,
      STRESS_TEST
    } sendType;

    workMode m_workMode;
    sendType m_sendType;
    wxString m_message;
    wxString m_host;
    long m_stressWorkers;

    virtual bool OnInit() wxOVERRIDE;
    virtual int OnRun() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;
    void OnInitCmdLine(wxCmdLineParser& pParser) wxOVERRIDE;
    bool OnCmdLineParsed(wxCmdLineParser& pParser) wxOVERRIDE;
    void OnWorkerEvent(WorkerEvent& pEvent);
    void OnTimerEvent(wxTimerEvent& pEvent);

    void StartWorker(workMode pMode, const wxString& pMessage);
    void StartWorker(workMode pMode);
    char* CreateBuffer(int *msgsize);

    void dumpStatistics();

    TList m_threadWorkers;
    EList m_eventWorkers;

    unsigned m_statConnecting;
    unsigned m_statSending;
    unsigned m_statReceiving;
    unsigned m_statDisconnecting;
    unsigned m_statDone;
    unsigned m_statFailed;

    wxTimer mTimer;
};

wxDECLARE_APP(Client);

class ThreadWorker : public wxThread
{
public:
    ThreadWorker(const wxString& p_host, char* p_buf, int p_size);
    virtual ExitCode Entry() wxOVERRIDE;
private:
    wxString m_host;
    wxSocketClient* m_clientSocket;
    char* m_inbuf;
    char* m_outbuf;
    int m_outsize;
    int m_insize;
    wxString m_workerIdent;
};

class EventWorker : public wxEvtHandler
{
    wxDECLARE_EVENT_TABLE();
public:
    EventWorker(const wxString& p_host, char* p_buf, int p_size);
    void Run();
    virtual ~EventWorker();
private:
    wxString m_host;
    wxSocketClient* m_clientSocket;
    char* m_inbuf;
    char* m_outbuf;
    int m_outsize;
    int m_written;
    int m_insize;
    int m_readed;

    WorkerEvent::evt_type m_currentType;
    bool m_doneSent;
    wxIPV4address m_localaddr;

    void OnSocketEvent(wxSocketEvent& pEvent);
    void SendEvent(bool failed);
};

/******************* Implementation ******************/
wxIMPLEMENT_APP_CONSOLE(Client);

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(TList);
WX_DEFINE_LIST(EList);

wxString
CreateIdent(const wxIPV4address& addr)
{
    return wxString::Format(wxT("%s:%d"),addr.IPAddress().c_str(),addr.Service());
}

void
Client::OnInitCmdLine(wxCmdLineParser& pParser)
{
    wxApp::OnInitCmdLine(pParser);
    pParser.AddSwitch(wxT("e"),wxT("event"),_("Use event based worker (default)"),wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddSwitch(wxT("t"),wxT("thread"),_("Use thread based worker"),wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddSwitch(wxT("r"),wxT("random"),_("Send random data (default)"),wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddOption(wxT("m"),wxT("message"),_("Send message from <str>"),wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddOption(wxT("f"),wxT("file"),_("Send contents of <file>"),wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddOption(wxT("H"),wxT("hostname"),_("IP or name of host to connect to"),wxCMD_LINE_VAL_STRING,wxCMD_LINE_PARAM_OPTIONAL);
    pParser.AddOption(wxT("s"),wxT("stress"),_("stress test with <num> concurrent connections"),wxCMD_LINE_VAL_NUMBER,wxCMD_LINE_PARAM_OPTIONAL);
}


bool
Client::OnCmdLineParsed(wxCmdLineParser& pParser)
{
    wxString fname;
    m_workMode = EVENTS;
    m_stressWorkers = 50;

    if (pParser.Found(_("verbose")))
    {
        wxLog::AddTraceMask(wxT("wxSocket"));
        wxLog::AddTraceMask(wxT("epolldispatcher"));
        wxLog::AddTraceMask(wxT("selectdispatcher"));
        wxLog::AddTraceMask(wxT("thread"));
        wxLog::AddTraceMask(wxT("events"));
    }

    if (pParser.Found(wxT("t")))
        m_workMode = THREADS;
    m_sendType = SEND_RANDOM;

    if (pParser.Found(wxT("m"),&m_message))
        m_sendType = SEND_MESSAGE;
    else if (pParser.Found(wxT("f"),&fname))
    {
        wxFFile file(fname);
        if (!file.IsOpened()) {
            wxLogError(wxT("Cannot open file %s"),fname.c_str());
            return false;
        };
        if (!file.ReadAll(&m_message)) {
            wxLogError(wxT("Cannot read content of file %s"),fname.c_str());
            return false;
        };
        m_sendType = SEND_MESSAGE;
    };

    if (pParser.Found(wxT("s"),&m_stressWorkers))
        m_sendType = STRESS_TEST;

    m_host = wxT("127.0.0.1");
    pParser.Found(wxT("H"),&m_host);
    return wxApp::OnCmdLineParsed(pParser);
};

bool
Client::OnInit()
{
    if (!wxApp::OnInit())
        return false;
    srand(wxDateTime::Now().GetTicks());
    mTimer.SetOwner(this);
    m_statConnecting = 0;
    m_statSending = 0;
    m_statReceiving = 0;
    m_statDisconnecting = 0;
    m_statDone = 0;
    m_statFailed = 0;
    return true;
}

int
Client::OnRun()
{
    int i;
    switch(m_sendType)
    {
        case STRESS_TEST:
            switch(m_workMode)
            {
                case THREADS:
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                    for (i = 0; i < m_stressWorkers; i++) {
                        if (m_message.empty())
                            StartWorker(THREADS);
                        else
                            StartWorker(THREADS, m_message);
                    }
                    break;
                case EVENTS:
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                    for (i = 0; i < m_stressWorkers; i++) {
                        if (m_message.empty())
                            StartWorker(EVENTS);
                        else
                            StartWorker(EVENTS, m_message);
                    }
                    break;
                default:
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
                    for (i = 0; i < m_stressWorkers; i++) {
                        if (m_message.empty())
                            StartWorker(i % 5 == 0 ? THREADS : EVENTS);
                        else
                            StartWorker(i % 5 == 0 ? THREADS : EVENTS, m_message);
                    }
                break;
            }
        break;
        case SEND_MESSAGE:
            StartWorker(m_workMode,m_message);
        break;
        case SEND_RANDOM:
            StartWorker(m_workMode);
        break;
    }
    mTimer.Start(timeout_val,true);
    return wxApp::OnRun();
}

int
Client::OnExit()
{
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(EList::compatibility_iterator it = m_eventWorkers.GetFirst(); it ; it->GetNext()) {
        delete it->GetData();
    }
    return 0;
}

// Create buffer to be sent by client. Buffer contains test indicator
// message size and place for data
// msgsize parameter contains size of data in bytes and
// if input value does not fit into 250 bytes then
// on exit is updated to new value that is multiply of 1024 bytes
char*
Client::CreateBuffer(int* msgsize)
{
    int bufsize = 0;
    char* buf;
    //if message should have more than 256 bytes then set it as
    //test3 for compatibility with GUI server sample
    if ((*msgsize) > 250)
    {
        //send at least one KB of data
        int size = (*msgsize)/1024 + 1;
        //returned buffer will contain test indicator, message size in KB and data
        bufsize = size*1024+2;
        buf = new char[bufsize];
        buf[0] = (unsigned char)0xDE; //second byte contains size in kilobytes
        buf[1] = (char)(size);
        *msgsize = size*1024;
    }
    else
    {
        //returned buffer will contain test indicator, message size in KB and data
        bufsize = (*msgsize)+2;
        buf = new char[bufsize];
        buf[0] = (unsigned char)0xBE; //second byte contains size in bytes
        buf[1] = (char)(*msgsize);
    }
    return buf;
}

void
Client::StartWorker(workMode pMode) {
    int msgsize = 1 + (int) (250000.0 * (rand() / (RAND_MAX + 1.0)));
    char* buf = CreateBuffer(&msgsize);

    //fill data part of buffer with random bytes
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for (int i = 2; i < (msgsize); i++) {
        buf[i] = i % 256;
    }

    if (pMode == THREADS) {
        ThreadWorker* c = new ThreadWorker(m_host,buf,msgsize+2);
        if (c->Create() != wxTHREAD_NO_ERROR) {
            wxLogError(wxT("Cannot create more threads"));
        } else {
            c->Run();
            m_threadWorkers.Append(c);
        }
    } else {
        EventWorker* e = new EventWorker(m_host,buf,msgsize+2);
        e->Run();
        m_eventWorkers.Append(e);
    }
    m_statConnecting++;
}

void
Client::StartWorker(workMode pMode, const wxString& pMessage) {
    char* tmpbuf = wxStrdup(pMessage.mb_str());
    int msgsize = strlen(tmpbuf);
    char* buf = CreateBuffer(&msgsize);
    memset(buf+2,0x0,msgsize);
    memcpy(buf+2,tmpbuf,msgsize);
    free(tmpbuf);

    if (pMode == THREADS) {
        ThreadWorker* c = new ThreadWorker(m_host,buf,msgsize+2);
        if (c->Create() != wxTHREAD_NO_ERROR) {
            wxLogError(wxT("Cannot create more threads"));
        } else {
            c->Run();
            m_threadWorkers.Append(c);
        }
    } else {
        EventWorker* e = new EventWorker(m_host,buf,msgsize+2);
        e->Run();
        m_eventWorkers.Append(e);
    }
    m_statConnecting++;
}

void
Client::OnWorkerEvent(WorkerEvent& pEvent) {
    switch (pEvent.m_eventType) {
        case WorkerEvent::CONNECTING:
            if (pEvent.isFailed())
            {
                m_statConnecting--;
                m_statFailed++;
            }
        break;
        case WorkerEvent::SENDING:
            if (pEvent.isFailed())
            {
                m_statFailed++;
                m_statSending--;
            }
            else
            {
                m_statConnecting--;
                m_statSending++;
            }
        break;
        case WorkerEvent::RECEIVING:
            if (pEvent.isFailed())
            {
                m_statReceiving--;
                m_statFailed++;
            }
            else
            {
                m_statSending--;
                m_statReceiving++;
            }
        break;
        case WorkerEvent::DISCONNECTING:
            if (pEvent.isFailed())
            {
                m_statDisconnecting--;
                m_statFailed++;
            }
            else
            {
                m_statReceiving--;
                m_statDisconnecting++;
            }
        break;
        case WorkerEvent::DONE:
            m_statDone++;
            m_statDisconnecting--;
        break;
    };

    if (pEvent.isFailed() || pEvent.m_eventType == WorkerEvent::DONE)
    {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(TList::compatibility_iterator it = m_threadWorkers.GetFirst(); it ; it = it->GetNext()) {
            if (it->GetData() == pEvent.m_sender) {
                m_threadWorkers.DeleteNode(it);
                break;
            }
        }
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        for(EList::compatibility_iterator it2 = m_eventWorkers.GetFirst(); it2 ; it2 = it2->GetNext())
        {
            if (it2->GetData() == pEvent.m_sender) {
                delete it2->GetData();
                m_eventWorkers.DeleteNode(it2);
                break;
            }
        }
        if ((m_threadWorkers.GetCount() == 0) && (m_eventWorkers.GetCount() == 0))
        {
            mTimer.Stop();
            dumpStatistics();
            wxSleep(2);
            ExitMainLoop();
        }
        else
        {
            mTimer.Start(timeout_val,true);
        }
    }
}

void
Client::RemoveEventWorker(EventWorker* p_worker) {
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
    for(EList::compatibility_iterator it = m_eventWorkers.GetFirst(); it ; it = it->GetNext()) {
        if (it->GetData() == p_worker) {
            //wxLogDebug(wxT("Deleting event worker"));
            delete it->GetData();
            m_eventWorkers.DeleteNode(it);
            return;
        }
    }
}

void
Client::dumpStatistics() {
    wxString msg(
        wxString::Format(_("Connecting:\t%d\nSending\t\t%d\nReceiving\t%d\nDisconnecting:\t%d\nDone:\t\t%d\nFailed:\t\t%d\n"),
                m_statConnecting,
                m_statSending,
                m_statReceiving,
                m_statDisconnecting,
                m_statDone,
                m_statFailed
                ));

    wxLogMessage(wxT("Current status:\n%s\n"),msg.c_str());
}

void
Client::OnTimerEvent(wxTimerEvent&) {
    dumpStatistics();
}

wxBEGIN_EVENT_TABLE(Client,wxEvtHandler)
    EVT_WORKER(Client::OnWorkerEvent)
    EVT_TIMER(wxID_ANY,Client::OnTimerEvent)
wxEND_EVENT_TABLE()



EventWorker::EventWorker(const wxString& p_host, char* p_buf, int p_size)
  : m_host(p_host),
    m_outbuf(p_buf),
    m_outsize(p_size),
    m_written(0),
    m_readed(0)
{
    m_clientSocket = new wxSocketClient(wxSOCKET_NOWAIT);
    m_clientSocket->SetEventHandler(*this);
    m_insize = m_outsize - 2;
    m_inbuf = new char[m_insize];
}

void
EventWorker::Run() {
    wxIPV4address ca;
    ca.Hostname(m_host);
    ca.Service(3000);
    m_clientSocket->SetNotify(wxSOCKET_CONNECTION_FLAG|wxSOCKET_LOST_FLAG|wxSOCKET_OUTPUT_FLAG|wxSOCKET_INPUT_FLAG);
    m_clientSocket->Notify(true);
    m_currentType = WorkerEvent::CONNECTING;
    m_doneSent = false;
    //wxLogMessage(wxT("EventWorker: Connecting....."));
    m_clientSocket->Connect(ca,false);
}

void
EventWorker::OnSocketEvent(wxSocketEvent& pEvent) {
    switch(pEvent.GetSocketEvent()) {
        case wxSOCKET_INPUT:
            //wxLogDebug(wxT("EventWorker: INPUT"));
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            do {
                if (m_readed == m_insize)
                    return; //event already posted
                m_clientSocket->Read(m_inbuf + m_readed, m_insize - m_readed);
                if (m_clientSocket->Error())
                {
                    if (m_clientSocket->LastError() != wxSOCKET_WOULDBLOCK)
                    {
                        wxLogError(wxT("%s: read error"),CreateIdent(m_localaddr).c_str());
                        SendEvent(true);
                    }
                }

                m_readed += m_clientSocket->LastCount();
                //wxLogDebug(wxT("EventWorker: readed %d bytes, %d bytes to do"),m_clientSocket->LastCount(), m_insize - m_readed);
                if (m_readed == m_insize)
                {
                    if (!memcmp(m_inbuf,m_outbuf,m_insize)) {
                        wxLogError(wxT("%s: data mismatch"),CreateIdent(m_localaddr).c_str());
                        SendEvent(true);
                    }
                    m_currentType = WorkerEvent::DISCONNECTING;
                    wxLogDebug(wxT("%s: DISCONNECTING"),CreateIdent(m_localaddr).c_str());
                    SendEvent(false);

                    //wxLogDebug(wxT("EventWorker %p closing"),this);
                    m_clientSocket->Close();

                    m_currentType = WorkerEvent::DONE;
                    wxLogDebug(wxT("%s: DONE"),CreateIdent(m_localaddr).c_str());
                    SendEvent(false);
                }
            } while (!m_clientSocket->Error());
        break;
        case wxSOCKET_OUTPUT:
            //wxLogDebug(wxT("EventWorker: OUTPUT"));
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            do {
                if (m_written == m_outsize)
                    return;
                if (m_written == 0)
                {
                    m_currentType = WorkerEvent::SENDING;
                    wxLogDebug(wxT("%s: SENDING"),CreateIdent(m_localaddr).c_str());
                }
                m_clientSocket->Write(m_outbuf + m_written, m_outsize - m_written);
                if (m_clientSocket->Error())
                {
                    if (m_clientSocket->LastError() != wxSOCKET_WOULDBLOCK) {
                        wxLogError(wxT("%s: Write error"),CreateIdent(m_localaddr).c_str());
                        SendEvent(true);
                    }
                }
                m_written += m_clientSocket->LastCount();
                if (m_written != m_outsize)
                {
                    //wxLogDebug(wxT("EventWorker: written %d bytes, %d bytes to do"),m_clientSocket->LastCount(),m_outsize - m_written);
                }
                else
                {
                    //wxLogDebug(wxT("EventWorker %p SENDING->RECEIVING"),this);
                    m_currentType = WorkerEvent::RECEIVING;
                    wxLogDebug(wxT("%s: RECEIVING"),CreateIdent(m_localaddr).c_str());
                    SendEvent(false);
                }
            } while(!m_clientSocket->Error());
        break;
        case wxSOCKET_CONNECTION:
        {
            //wxLogMessage(wxT("EventWorker: got connection"));
            wxLogMessage(wxT("%s: starting writing message (2 bytes for signature and %d bytes of data to write)"),CreateIdent(m_localaddr).c_str(),m_outsize-2);
            if (!m_clientSocket->GetLocal(m_localaddr))
            {
                wxLogError(_("Cannot get peer data for socket %p"),m_clientSocket);
            }
            m_currentType = WorkerEvent::SENDING;
            wxLogDebug(wxT("%s: CONNECTING"),CreateIdent(m_localaddr).c_str());
            SendEvent(false);
        }
        break;
        case wxSOCKET_LOST:
        {
            wxLogError(_("%s: connection lost"),CreateIdent(m_localaddr).c_str());
            SendEvent(true);
        }
        break;
    }
}

void
EventWorker::SendEvent(bool failed) {
    if (m_doneSent)
        return;
    WorkerEvent e(this,m_currentType);
    if (failed) e.setFailed();
    wxGetApp().AddPendingEvent(e);
    m_doneSent = failed || m_currentType == WorkerEvent::DONE;
};

EventWorker::~EventWorker() {
    m_clientSocket->Destroy();
    delete [] m_outbuf;
    delete [] m_inbuf;
}

wxBEGIN_EVENT_TABLE(EventWorker,wxEvtHandler)
    EVT_SOCKET(wxID_ANY,EventWorker::OnSocketEvent)
wxEND_EVENT_TABLE()


ThreadWorker::ThreadWorker(const wxString& p_host, char* p_buf, int p_size)
  : wxThread(wxTHREAD_DETACHED),
    m_host(p_host),
    m_outbuf(p_buf),
    m_outsize(p_size)
{
    m_clientSocket = new wxSocketClient(wxSOCKET_BLOCK|wxSOCKET_WAITALL);
    m_insize = m_outsize - 2;
    m_inbuf = new char[m_insize];
}

wxThread::ExitCode ThreadWorker::Entry()
{
    wxIPV4address ca;
    ca.Hostname(m_host);
    ca.Service(5678);
    //wxLogDebug(wxT("ThreadWorker: Connecting....."));
    m_clientSocket->SetTimeout(60);
    bool failed = false;
    WorkerEvent::evt_type etype = WorkerEvent::CONNECTING;
    if (!m_clientSocket->Connect(ca)) {
        wxLogError(wxT("Cannot connect to %s:%d"),ca.IPAddress().c_str(), ca.Service());
        failed = true;
    } else {
        //wxLogMessage(wxT("ThreadWorker: Connected. Sending %d bytes of data"),m_outsize);
        etype = WorkerEvent::SENDING;
        WorkerEvent e(this,etype);
        wxGetApp().AddPendingEvent(e);
        int to_process = m_outsize;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
        do {
            m_clientSocket->Write(m_outbuf,m_outsize);
            if (m_clientSocket->Error()) {
                wxLogError(wxT("ThreadWorker: Write error"));
                failed  = true;
            }
            to_process -= m_clientSocket->LastCount();
            //wxLogDebug(wxT("EventWorker: written %d bytes, %d bytes to do"),m_clientSocket->LastCount(),to_process);
        } while(!m_clientSocket->Error() && to_process != 0);

        if (!failed) {
            etype = WorkerEvent::RECEIVING;
            WorkerEvent e(this,etype);
            wxGetApp().AddPendingEvent(e);
            to_process = m_insize;
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            do {
                m_clientSocket->Read(m_inbuf,m_insize);
                if (m_clientSocket->Error()) {
                    wxLogError(wxT("ThreadWorker: Read error"));
                    failed = true;
                    break;
                }
                to_process -= m_clientSocket->LastCount();
                //wxLogDebug(wxT("EventWorker: readed %d bytes, %d bytes to do"),m_clientSocket->LastCount(),to_process);
            } while(!m_clientSocket->Error() && to_process != 0);
        }

        char* outdat = (char*)m_outbuf+2;
        if (!failed && (memcmp(m_inbuf,outdat,m_insize) != 0))
        {
            wxLogError(wxT("Data mismatch"));
            failed = true;
        }
    }
    //wxLogDebug(wxT("ThreadWorker: Finished"));
    if (!failed) {
        etype = WorkerEvent::DISCONNECTING;
        WorkerEvent e(this,etype);
        wxGetApp().AddPendingEvent(e);
    };
    m_clientSocket->Close();
    m_clientSocket->Destroy();
    m_clientSocket = NULL;
    delete [] m_outbuf;
    delete [] m_inbuf;
    if (!failed)
        etype = WorkerEvent::DONE;
    WorkerEvent e(this,etype);
    if (failed) e.setFailed();
    wxGetApp().AddPendingEvent(e);
    return 0;
}

