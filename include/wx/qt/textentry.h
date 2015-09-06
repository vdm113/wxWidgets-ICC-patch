/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/////////////////////////////////////////////////////////////////////////////
// Name:        wx/qt/textentry.h
// Author:      Peter Most, Mariano Reingart
// Copyright:   (c) 2010 wxWidgets dev team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_QT_TEXTENTRY_H_
#define _WX_QT_TEXTENTRY_H_

class WXDLLIMPEXP_CORE wxTextEntry : public wxTextEntryBase
{
public:
    wxTextEntry();

    virtual void WriteText(const wxString& text);

    virtual void Remove(long from, long to);

    virtual void Copy();
    virtual void Cut();
    virtual void Paste();
    
    virtual void Undo();
    virtual void Redo();
    virtual bool CanUndo() const;
    virtual bool CanRedo() const;

    virtual void SetInsertionPoint(long pos);
    virtual long GetInsertionPoint() const;
    virtual long GetLastPosition() const;

    virtual void SetSelection(long from, long to);
    virtual void GetSelection(long *from, long *to) const;
    
    virtual bool IsEditable() const;
    virtual void SetEditable(bool editable);
    
protected:
    virtual wxString DoGetValue() const;
    virtual void DoSetValue(const wxString& value, int flags=0);

    virtual wxWindow *GetEditableWindow();

private:
};

#endif // _WX_QT_TEXTENTRY_H_
