This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 12.1/13.0.

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2010 for ICL 12.1/13.0.

The original sources were patched by (see above) SW for ICL 12.1 and 13.0. There was one assertion failure in the compiler (ICC 12.1) compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the ICC 12.1 compiler will abide by it.

I can do a very limited support... I support only ICC 12.1 and ICC 13.0 (which is beta ATM) under Windows's MSVC 2010 with x64 builds, though x86 (32-bit) builds might be successful, but I have not tried it (I mean x86).

This fork of wxWidgets is as of official SVN revision:

r74257

Also, it uses my own pre-configured "setup.h" configuration. I have changed only this parameter (compared to default 'setup0.h'):

#define wxUSE_COMPILER_TLS 1
#ifdef _MSC_VER
#   undef wxUSE_COMPILER_TLS
#   define wxUSE_COMPILER_TLS 0 /* VDM: it crashes with ICC */
#endif

*** NB ***
Set in your project files these preprocessor defines, in other case you might encounter random crashes:
_HAS_ITERATOR_DEBUGGING=0;_ITERATOR_DEBUG_LEVEL=0
or remove these from MSVC project files of WX for debugging STL iterators, but you need to keep this consistent, i.e. WX and your project needs to have same defines.
*** NB ***

If you are using 32-bit ICC, or 64-bit one and you don't have enough RAM or time, I recommend change for sub-library "core" from /Qipo to /Qip.

NB: Please, read file "BuildSVN.txt" in this directory as well. There are instructions for building from SVN, but as this GIT repository is regurarily updated from official SVN repository, and it is its fork, it is relevant to read it.

NB: Please, checkout from this GIT repository by GIT's tag revision. They are considered as "stable". When I will check that SVN from upstream is compile-able without errors, then I test GUI, I will make a GIT tag like 'wx_svn_XXX' where "XXX" is the upstream/official SVN revision number. This is just to make you sure that this commit/tag is working well (upstream might do a mistake, we all are humans...).

Best regards,
Marian 'VooDooMan' Meravy
vdm 113 (<-remove the space character) [at sign] gmail (dot character) com
.
