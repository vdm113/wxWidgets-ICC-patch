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
   XsComponent.h

   History
      03-Mar-96 1.0; Scott W. Sadler (ssadler@cisco.com)
                     Created         
*/

#ifndef XSCOMPONENT_H
#define XSCOMPONENT_H

// Includes

#include <X11/Intrinsic.h>

// XsComponent class

class XsComponent {

   public:

// Destructor

      virtual  ~XsComponent ( );

// Component manipulation

      virtual  void show ( );             // Show the component
      virtual  void hide ( );             // Hide the component
      
// Component name

      const char *name ( ) const;

// Utilities

      Widget   base ( ) const;
      virtual  operator Widget ( ) const;

// Operators

      Boolean operator == (const XsComponent&);

// Class name

      virtual const char *className ( ) const;

   protected:

// Protected constructor

      XsComponent (const char *name);
      
// Component life-cycle

      virtual void _componentDestroyed ( );
      void  _installDestroyHandler ( );
      void  _removeDestroyHandler ( );
		  
// Resource manager

      void  _setResources (Widget w, const String *);
      void  _getResources (const XtResourceList, int);

// Implementation

      char    *_name;            // Component name
      Widget   _base;            // Base widget

   private:

// Callbacks

      static void _componentDestroyedCallback (Widget, XtPointer, XtPointer);
};
   
// Inline member functions

inline Widget XsComponent::base ( ) const
{
   return (_base);
}

inline const char* XsComponent::name ( ) const
{
   return (_name);
}

#endif

