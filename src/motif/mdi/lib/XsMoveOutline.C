/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

/*
   Copyright (C) 1996 Scott W. Sadler
   All rights reserved.
*/

/*
   XsMoveOutline.C

   History
      03-Mar-96 1.0; Scott W. Sadler (ssadler@cisco.com)
                     Created         
*/

// Includes

#include <assert.h>
#include "XsMoveOutline.h"
#include <X11/cursorfont.h>

// Static definitions

Cursor XsMoveOutline::_fleur = None;

// Constructor

XsMoveOutline::XsMoveOutline (Widget w) : XsOutline (w)
{
   unsigned int mask;      // Not used
   Window   root;
   int      xy;
   
// Create the cursor (if necessary)

   if (_fleur == None)
      _fleur = XCreateFontCursor (XtDisplay (_w), XC_fleur);

// Get the current mouse root coordinates

   XQueryPointer (XtDisplay (_w), XtWindow (_w), &root, &root, &_rootX,
      &_rootY, &xy, &xy, &mask);
}

// Destructor

XsMoveOutline::~XsMoveOutline ( )
{
   // Empty
}

// _motionHandler

void XsMoveOutline::_motionHandler (XEvent *event)
{
   int   curX, curY;
            
// Get current mouse position

   curX = event->xbutton.x_root;
   curY = event->xbutton.y_root;
               
// Compute the new window position

   _x += (curX - _rootX);
   _y += (curY - _rootY);

// Move the window

   _drawOutline (False);

// Save the new root position

   _rootX = curX;
   _rootY = curY;
}

// _getCursor

Cursor XsMoveOutline::_getCursor ( ) const
{
   return (_fleur);
}

