This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 12.1/13.0.

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2010 for ICL 12.1/13.0.

The original sources were patched by (see above) SW for ICL 12.1 and 13.0. There was one assertion failure in the compiler (ICC 12.1) compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the ICC 12.1 compiler will abide by it.

I can do a very limited support... I support only ICC 12.1 and ICC 13.0 (which is beta ATM) under Windows's MSVC 2010 with x64 builds, though x86 (32-bit) builds might be successful, but I have not tried it (I mean x86).

This fork of wxWidgets is as of official SVN revision:

r73053

Also, it uses my own pre-configured "setup.h" configuration. I have changed only these parameters (compared to default 'setup0.h'):
#define WXWIN_COMPATIBILITY_2_8 0 /* VDM: changed explicitly to 1 from 0 */

#define wxUSE_COMPILER_TLS 1 /* VDM: see below */
#ifdef _MSC_VER
#   undef wxUSE_COMPILER_TLS
#   define wxUSE_COMPILER_TLS 0 /* VDM: see comment above in setup.h */
#endif

#define wxUSE_STD_CONTAINERS 1 /* VDM: changed explicitly to 0 from 1 */
#define wxUSE_IPV6          1 /* VDM: changed explicitly to 0 from 1 */
#define wxUSE_POSTSCRIPT  1 /* VDM: changed explicitly to 0 from 1 */
#define wxUSE_INICONF 1 /* VDM: changed explicitely to 1 from 0 */

I have recently changed all profiles and architectures (unsupported x86, and supported x64) to use "/Qip" ICL switch rather than "/Qipo". So Intra-procedural Optimization will be performed over each *.obj, rather at then multi-.obj manner. Performance impact of resulting code for my benchmarks is negligible, and compilation will not take 2 days (on my 8 GiB RAM machine) while compiler is requesting like 12 GiB of (virtual) RAM [Windows(R) was swapping excessively].

*** NB ***
include this code BEFORE any inclusion of wx's or other includes (especially STL), in other case you might encounter random crashes:
---snip---
#if defined(_MSC_VER) && defined(MY_MSC_NO_ITERATOR_DEBUGGING) && MY_MSC_NO_ITERATOR_DEBUGGING==1
#   if defined(_SECURE_SCL)
#       undef _SECURE_SCL
#   endif
#   define _SECURE_SCL 0

#   if defined(_HAS_ITERATOR_DEBUGGING)
#       undef _HAS_ITERATOR_DEBUGGING
#   endif
#   define _HAS_ITERATOR_DEBUGGING 0
#endif
---snip---
Also, make sure you have same preprocessor settings of "MY_MSC_NO_ITERATOR_DEBUGGING" in wxWidgets library and in your project!
*** NB ***
NB: The x64 build DLL's in x64 DLL build are located in directory lib/vc_dll . When your project wants to use import libraries under MSVC, to use DLL's at run-time, you will need to copy directories lib/vc_dll/mswu and lib/vc_dll/mswud (and so on) into new directory lib/vc_x64_dll/ aftere building WX x64. There is setup.h for your project to include, so put this new path into include path of your compiler.

If you are using 32-bit ICC, or 64-bit one and you don't have enough RAM or time, I recommend change for sub-library "core" from /Qipo to /Qip.

Best regards,
Marian 'VooDooMan' Meravy
vdm 113 (<-remove the space character) [at sign] gmail (dot character) com
.
