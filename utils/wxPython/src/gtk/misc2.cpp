/*
 * FILE : gtk/misc2.cpp
 * 
 * This file was automatically generated by :
 * Simplified Wrapper and Interface Generator (SWIG)
 * Version 1.1 (Patch 5)
 * 
 * Portions Copyright (c) 1995-1998
 * The University of Utah and The Regents of the University of California.
 * Permission is granted to distribute this file in any manner provided
 * this notice remains intact.
 * 
 * Do not make changes to this file--changes will be lost!
 *
 */


#define SWIGCODE
/* Implementation : PYTHON */

#define SWIGPYTHON
#include <string.h>
#include <stdlib.h>
/* Definitions for Windows/Unix exporting */
#if defined(__WIN32__)
#   if defined(_MSC_VER)
#	define SWIGEXPORT(a,b) __declspec(dllexport) a b
#   else
#	if defined(__BORLANDC__)
#	    define SWIGEXPORT(a,b) a _export b
#	else
#	    define SWIGEXPORT(a,b) a b
#	endif
#   endif
#else
#   define SWIGEXPORT(a,b) a b
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "Python.h"
extern void SWIG_MakePtr(char *, void *, char *);
extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));
extern char *SWIG_GetPtr(char *, void **, char *);
extern void SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
extern PyObject *SWIG_newvarlink(void);
#ifdef __cplusplus
}
#endif

#define SWIG_init    initmisc2c

#define SWIG_name    "misc2c"

#include "helpers.h"
#include <wx/resource.h>
#include <wx/tooltip.h>

static PyObject* l_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;
    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyList_Check(target)) {
            o2 = target;
            target = PyList_New(0);
            PyList_Append(target, o2);
	    Py_XDECREF(o2);
        }
        PyList_Append(target,o);
	Py_XDECREF(o);
    }
    return target;
}

static PyObject* t_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;

    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyTuple_Check(target)) {
            o2 = target;
            target = PyTuple_New(1);
            PyTuple_SetItem(target, 0, o2);
        }
        o3 = PyTuple_New(1);            
        PyTuple_SetItem(o3, 0, o);      

        o2 = target;
        target = PySequence_Concat(o2, o3); 
        Py_DECREF(o2);                      
        Py_DECREF(o3);
    }
    return target;
}


extern byte* byte_LIST_helper(PyObject* source);
extern int* int_LIST_helper(PyObject* source);
extern long* long_LIST_helper(PyObject* source);
extern char** string_LIST_helper(PyObject* source);
extern wxPoint* wxPoint_LIST_helper(PyObject* source);
extern wxBitmap** wxBitmap_LIST_helper(PyObject* source);
extern wxString* wxString_LIST_helper(PyObject* source);
extern wxAcceleratorEntry* wxAcceleratorEntry_LIST_helper(PyObject* source);


static char* wxStringErrorMsg = "string type is required for parameter";

    void wxToolTip_Enable(bool flag) {
        wxToolTip::Enable(flag);
    }

    void wxToolTip_SetDelay(long milliseconds) {
        wxToolTip::SetDelay(milliseconds);
    }
static PyObject *_wrap_wxFindWindowByLabel(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxString * _arg0;
    wxWindow * _arg1 = NULL;
    PyObject * _obj0 = 0;
    char * _argc1 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"O|s:wxFindWindowByLabel",&_obj0,&_argc1)) 
        return NULL;
{
    if (!PyString_Check(_obj0)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg0 = new wxString(PyString_AsString(_obj0), PyString_Size(_obj0));
}
    if (_argc1) {
        if (SWIG_GetPtr(_argc1,(void **) &_arg1,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxFindWindowByLabel. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxWindow *)wxFindWindowByLabel(*_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
{
    if (_obj0)
        delete _arg0;
}
    return _resultobj;
}

static PyObject *_wrap_wxFindWindowByName(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxString * _arg0;
    wxWindow * _arg1 = NULL;
    PyObject * _obj0 = 0;
    char * _argc1 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"O|s:wxFindWindowByName",&_obj0,&_argc1)) 
        return NULL;
{
    if (!PyString_Check(_obj0)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg0 = new wxString(PyString_AsString(_obj0), PyString_Size(_obj0));
}
    if (_argc1) {
        if (SWIG_GetPtr(_argc1,(void **) &_arg1,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxFindWindowByName. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxWindow *)wxFindWindowByName(*_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
{
    if (_obj0)
        delete _arg0;
}
    return _resultobj;
}

static PyObject *_wrap_wxToolTip_Enable(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    bool  _arg0;
    int tempbool0;

    self = self;
    if(!PyArg_ParseTuple(args,"i:wxToolTip_Enable",&tempbool0)) 
        return NULL;
    _arg0 = (bool ) tempbool0;
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxToolTip_Enable(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static PyObject *_wrap_wxToolTip_SetDelay(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    long  _arg0;

    self = self;
    if(!PyArg_ParseTuple(args,"l:wxToolTip_SetDelay",&_arg0)) 
        return NULL;
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxToolTip_SetDelay(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define new_wxToolTip(_swigarg0) (new wxToolTip(_swigarg0))
static PyObject *_wrap_new_wxToolTip(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxToolTip * _result;
    wxString * _arg0;
    PyObject * _obj0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"O:new_wxToolTip",&_obj0)) 
        return NULL;
{
    if (!PyString_Check(_obj0)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg0 = new wxString(PyString_AsString(_obj0), PyString_Size(_obj0));
}
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxToolTip *)new_wxToolTip(*_arg0);

    wxPy_END_ALLOW_THREADS;
}    SWIG_MakePtr(_ptemp, (char *) _result,"_wxToolTip_p");
    _resultobj = Py_BuildValue("s",_ptemp);
{
    if (_obj0)
        delete _arg0;
}
    return _resultobj;
}

#define wxToolTip_SetTip(_swigobj,_swigarg0)  (_swigobj->SetTip(_swigarg0))
static PyObject *_wrap_wxToolTip_SetTip(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxToolTip * _arg0;
    wxString * _arg1;
    char * _argc0 = 0;
    PyObject * _obj1 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"sO:wxToolTip_SetTip",&_argc0,&_obj1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxToolTip_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxToolTip_SetTip. Expected _wxToolTip_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj1)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg1 = new wxString(PyString_AsString(_obj1), PyString_Size(_obj1));
}
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxToolTip_SetTip(_arg0,*_arg1);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
{
    if (_obj1)
        delete _arg1;
}
    return _resultobj;
}

#define wxToolTip_GetTip(_swigobj)  (_swigobj->GetTip())
static PyObject *_wrap_wxToolTip_GetTip(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxString * _result;
    wxToolTip * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxToolTip_GetTip",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxToolTip_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxToolTip_GetTip. Expected _wxToolTip_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = new wxString (wxToolTip_GetTip(_arg0));

    wxPy_END_ALLOW_THREADS;
}{
    _resultobj = PyString_FromString(WXSTRINGCAST *(_result));
}
{
    delete _result;
}
    return _resultobj;
}

#define wxToolTip_GetWindow(_swigobj)  (_swigobj->GetWindow())
static PyObject *_wrap_wxToolTip_GetWindow(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxToolTip * _arg0;
    char * _argc0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxToolTip_GetWindow",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxToolTip_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxToolTip_GetWindow. Expected _wxToolTip_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxWindow *)wxToolTip_GetWindow(_arg0);

    wxPy_END_ALLOW_THREADS;
}    SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
    return _resultobj;
}

static PyMethodDef misc2cMethods[] = {
	 { "wxToolTip_GetWindow", _wrap_wxToolTip_GetWindow, 1 },
	 { "wxToolTip_GetTip", _wrap_wxToolTip_GetTip, 1 },
	 { "wxToolTip_SetTip", _wrap_wxToolTip_SetTip, 1 },
	 { "new_wxToolTip", _wrap_new_wxToolTip, 1 },
	 { "wxToolTip_SetDelay", _wrap_wxToolTip_SetDelay, 1 },
	 { "wxToolTip_Enable", _wrap_wxToolTip_Enable, 1 },
	 { "wxFindWindowByName", _wrap_wxFindWindowByName, 1 },
	 { "wxFindWindowByLabel", _wrap_wxFindWindowByLabel, 1 },
	 { NULL, NULL }
};
static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void,initmisc2c)() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("misc2c", misc2cMethods);
	 d = PyModule_GetDict(m);
/*
 * These are the pointer type-equivalency mappings. 
 * (Used by the SWIG pointer type-checker).
 */
	 SWIG_RegisterMapping("_wxAcceleratorTable","_class_wxAcceleratorTable",0);
	 SWIG_RegisterMapping("_signed_long","_long",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_int",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_signed_int",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_unsigned_int",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_wxWindowID",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_uint",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_EBool",0);
	 SWIG_RegisterMapping("_wxPrintQuality","_size_t",0);
	 SWIG_RegisterMapping("_class_wxRegionIterator","_wxRegionIterator",0);
	 SWIG_RegisterMapping("_class_wxMenuBar","_wxMenuBar",0);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_wxEvtHandler",0);
	 SWIG_RegisterMapping("_wxIndividualLayoutConstraint","_class_wxIndividualLayoutConstraint",0);
	 SWIG_RegisterMapping("_wxCursor","_class_wxCursor",0);
	 SWIG_RegisterMapping("_wxMask","_class_wxMask",0);
	 SWIG_RegisterMapping("_wxToolTip","_class_wxToolTip",0);
	 SWIG_RegisterMapping("_wxPyMenu","_class_wxPyMenu",0);
	 SWIG_RegisterMapping("_wxPen","_class_wxPen",0);
	 SWIG_RegisterMapping("_byte","_unsigned_char",0);
	 SWIG_RegisterMapping("_long","_wxDash",0);
	 SWIG_RegisterMapping("_long","_unsigned_long",0);
	 SWIG_RegisterMapping("_long","_signed_long",0);
	 SWIG_RegisterMapping("_wxImageList","_class_wxImageList",0);
	 SWIG_RegisterMapping("_class_wxAcceleratorTable","_wxAcceleratorTable",0);
	 SWIG_RegisterMapping("_wxDC","_class_wxDC",0);
	 SWIG_RegisterMapping("_size_t","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_size_t","_unsigned_int",0);
	 SWIG_RegisterMapping("_size_t","_int",0);
	 SWIG_RegisterMapping("_size_t","_wxWindowID",0);
	 SWIG_RegisterMapping("_size_t","_uint",0);
	 SWIG_RegisterMapping("_class_wxRealPoint","_wxRealPoint",0);
	 SWIG_RegisterMapping("_class_wxMenuItem","_wxMenuItem",0);
	 SWIG_RegisterMapping("_class_wxPostScriptDC","_wxPostScriptDC",0);
	 SWIG_RegisterMapping("_wxPanel","_class_wxPanel",0);
	 SWIG_RegisterMapping("_class_wxMask","_wxMask",0);
	 SWIG_RegisterMapping("_class_wxToolTip","_wxToolTip",0);
	 SWIG_RegisterMapping("_wxColour","_class_wxColour",0);
	 SWIG_RegisterMapping("_class_wxDialog","_wxDialog",0);
	 SWIG_RegisterMapping("_wxBrush","_class_wxBrush",0);
	 SWIG_RegisterMapping("_uint","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_uint","_size_t",0);
	 SWIG_RegisterMapping("_uint","_unsigned_int",0);
	 SWIG_RegisterMapping("_uint","_int",0);
	 SWIG_RegisterMapping("_uint","_wxWindowID",0);
	 SWIG_RegisterMapping("_wxRect","_class_wxRect",0);
	 SWIG_RegisterMapping("_wxPoint","_class_wxPoint",0);
	 SWIG_RegisterMapping("_wxBitmap","_class_wxBitmap",0);
	 SWIG_RegisterMapping("_wxPyTimer","_class_wxPyTimer",0);
	 SWIG_RegisterMapping("_wxWindowDC","_class_wxWindowDC",0);
	 SWIG_RegisterMapping("_class_wxIndividualLayoutConstraint","_wxIndividualLayoutConstraint",0);
	 SWIG_RegisterMapping("_EBool","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_EBool","_signed_int",0);
	 SWIG_RegisterMapping("_EBool","_int",0);
	 SWIG_RegisterMapping("_EBool","_wxWindowID",0);
	 SWIG_RegisterMapping("_class_wxRegion","_wxRegion",0);
	 SWIG_RegisterMapping("_wxFont","_class_wxFont",0);
	 SWIG_RegisterMapping("_unsigned_long","_wxDash",0);
	 SWIG_RegisterMapping("_unsigned_long","_long",0);
	 SWIG_RegisterMapping("_class_wxRect","_wxRect",0);
	 SWIG_RegisterMapping("_class_wxDC","_wxDC",0);
	 SWIG_RegisterMapping("_class_wxPyTimer","_wxPyTimer",0);
	 SWIG_RegisterMapping("_wxAcceleratorEntry","_class_wxAcceleratorEntry",0);
	 SWIG_RegisterMapping("_class_wxPanel","_wxPanel",0);
	 SWIG_RegisterMapping("_signed_int","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_signed_int","_EBool",0);
	 SWIG_RegisterMapping("_signed_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_signed_int","_int",0);
	 SWIG_RegisterMapping("_wxLayoutConstraints","_class_wxLayoutConstraints",0);
	 SWIG_RegisterMapping("_wxMenu","_class_wxMenu",0);
	 SWIG_RegisterMapping("_wxScreenDC","_class_wxScreenDC",0);
	 SWIG_RegisterMapping("_WXTYPE","_short",0);
	 SWIG_RegisterMapping("_WXTYPE","_signed_short",0);
	 SWIG_RegisterMapping("_WXTYPE","_unsigned_short",0);
	 SWIG_RegisterMapping("_class_wxBrush","_wxBrush",0);
	 SWIG_RegisterMapping("_unsigned_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_unsigned_short","_short",0);
	 SWIG_RegisterMapping("_class_wxWindow","_wxWindow",0);
	 SWIG_RegisterMapping("_class_wxFont","_wxFont",0);
	 SWIG_RegisterMapping("_wxClientDC","_class_wxClientDC",0);
	 SWIG_RegisterMapping("_class_wxPoint","_wxPoint",0);
	 SWIG_RegisterMapping("_wxRealPoint","_class_wxRealPoint",0);
	 SWIG_RegisterMapping("_signed_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_signed_short","_short",0);
	 SWIG_RegisterMapping("_wxMemoryDC","_class_wxMemoryDC",0);
	 SWIG_RegisterMapping("_wxPaintDC","_class_wxPaintDC",0);
	 SWIG_RegisterMapping("_class_wxWindowDC","_wxWindowDC",0);
	 SWIG_RegisterMapping("_class_wxAcceleratorEntry","_wxAcceleratorEntry",0);
	 SWIG_RegisterMapping("_class_wxCursor","_wxCursor",0);
	 SWIG_RegisterMapping("_wxPostScriptDC","_class_wxPostScriptDC",0);
	 SWIG_RegisterMapping("_wxScrolledWindow","_class_wxScrolledWindow",0);
	 SWIG_RegisterMapping("_unsigned_char","_byte",0);
	 SWIG_RegisterMapping("_class_wxMenu","_wxMenu",0);
	 SWIG_RegisterMapping("_unsigned_int","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_unsigned_int","_size_t",0);
	 SWIG_RegisterMapping("_unsigned_int","_uint",0);
	 SWIG_RegisterMapping("_unsigned_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_unsigned_int","_int",0);
	 SWIG_RegisterMapping("_wxIcon","_class_wxIcon",0);
	 SWIG_RegisterMapping("_wxDialog","_class_wxDialog",0);
	 SWIG_RegisterMapping("_class_wxPyMenu","_wxPyMenu",0);
	 SWIG_RegisterMapping("_class_wxPen","_wxPen",0);
	 SWIG_RegisterMapping("_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_short","_unsigned_short",0);
	 SWIG_RegisterMapping("_short","_signed_short",0);
	 SWIG_RegisterMapping("_class_wxImageList","_wxImageList",0);
	 SWIG_RegisterMapping("_wxWindowID","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_wxWindowID","_size_t",0);
	 SWIG_RegisterMapping("_wxWindowID","_EBool",0);
	 SWIG_RegisterMapping("_wxWindowID","_uint",0);
	 SWIG_RegisterMapping("_wxWindowID","_int",0);
	 SWIG_RegisterMapping("_wxWindowID","_signed_int",0);
	 SWIG_RegisterMapping("_wxWindowID","_unsigned_int",0);
	 SWIG_RegisterMapping("_int","_wxPrintQuality",0);
	 SWIG_RegisterMapping("_int","_size_t",0);
	 SWIG_RegisterMapping("_int","_EBool",0);
	 SWIG_RegisterMapping("_int","_uint",0);
	 SWIG_RegisterMapping("_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_int","_unsigned_int",0);
	 SWIG_RegisterMapping("_int","_signed_int",0);
	 SWIG_RegisterMapping("_wxSize","_class_wxSize",0);
	 SWIG_RegisterMapping("_wxRegionIterator","_class_wxRegionIterator",0);
	 SWIG_RegisterMapping("_class_wxPaintDC","_wxPaintDC",0);
	 SWIG_RegisterMapping("_class_wxLayoutConstraints","_wxLayoutConstraints",0);
	 SWIG_RegisterMapping("_class_wxIcon","_wxIcon",0);
	 SWIG_RegisterMapping("_class_wxColour","_wxColour",0);
	 SWIG_RegisterMapping("_class_wxScreenDC","_wxScreenDC",0);
	 SWIG_RegisterMapping("_wxPalette","_class_wxPalette",0);
	 SWIG_RegisterMapping("_wxRegion","_class_wxRegion",0);
	 SWIG_RegisterMapping("_class_wxClientDC","_wxClientDC",0);
	 SWIG_RegisterMapping("_class_wxSize","_wxSize",0);
	 SWIG_RegisterMapping("_class_wxBitmap","_wxBitmap",0);
	 SWIG_RegisterMapping("_class_wxMemoryDC","_wxMemoryDC",0);
	 SWIG_RegisterMapping("_wxMenuBar","_class_wxMenuBar",0);
	 SWIG_RegisterMapping("_wxEvtHandler","_class_wxEvtHandler",0);
	 SWIG_RegisterMapping("_wxMenuItem","_class_wxMenuItem",0);
	 SWIG_RegisterMapping("_wxDash","_unsigned_long",0);
	 SWIG_RegisterMapping("_wxDash","_long",0);
	 SWIG_RegisterMapping("_class_wxScrolledWindow","_wxScrolledWindow",0);
	 SWIG_RegisterMapping("_class_wxPalette","_wxPalette",0);
	 SWIG_RegisterMapping("_wxWindow","_class_wxWindow",0);
}
