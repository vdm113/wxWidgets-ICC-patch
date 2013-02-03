This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 12.1/13.0.

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2010 for ICL 12.1/13.0.

The original sources were patched by (see above) SW for ICL 12.1 and 13.0. There was one assertion failure in the compiler (ICC 12.1) compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the ICC 12.1 compiler will abide by it.

I can do a very limited support... I support only ICC 12.1 and ICC 13.0 (which is beta ATM) under Windows's MSVC 2010 with x64 builds, though x86 (32-bit) builds might be successful, but I have not tried it (I mean x86).

This fork of wxWidgets is as of official SVN revision:

r73453

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
Set in your project files these preprocessor defines, in other case you might encounter random crashes:
_HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0
or remove these from MSVC project files of WX for debugging STL iterators, but you need to keep this consistent, i.e. WX and your project needs to have same defines.
*** NB ***

If you are using 32-bit ICC, or 64-bit one and you don't have enough RAM or time, I recommend change for sub-library "core" from /Qipo to /Qip.

Best regards,
Marian 'VooDooMan' Meravy
vdm 113 (<-remove the space character) [at sign] gmail (dot character) com
.
