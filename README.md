wxWidgets-ICC-patch
===================
About
-----

Latest wxWidgets trunk with ICC (a.k.a. ICL, Intel Compiler) pragma's for cutting-edge performance.

wxWidgets is a free and open source cross-platform C++ framework
for writing advanced GUI applications using native controls.

This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 12.1/13.0/14.0/15.0.
![wxWidgets Logo](http://www.wxwidgets.org/assets/img/header-logo.png)

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2013 for ICL 12.1/13.0/15.0.

The original sources were patched by (see above) SW for ICL 12.1, 13.0, 14.0 and 15.0. There was one assertion failure in the compiler (ICC 12.1) compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the ICC 12.1 compiler will abide by it.

wxWidgets allows you to write native-looking GUI applications for
all the major desktop platforms and also helps with abstracting
the differences in the non-GUI aspects between them. It is free
for the use in both open source and commercial applications, comes
with the full, easy to read and modify, source and extensive
documentation and a collection of more than a hundred examples.
You can learn more about wxWidgets at http://www.wxwidgets.org/
and read its documentation online at http://docs.wxwidgets.org/

I can do a very limited support... I support only ICC 15.0 under Windows's MSVC 2013 with x64 builds, though x86 (32-bit) builds might be successful, but I have not tried it (I mean x86).

This fork of wxWidgets is as of the official GIT repository: https://github.com/wxWidgets/wxWidgets

Platforms
---------

wxWidgets currently supports the following primary platforms:

If you are using 32-bit ICC, or 64-bit one and you don't have enough RAM or time, I recommend change for sub-library "core" from /Qipo to /Qip.
- Windows XP, Vista, 7 and 8 (32/64 bits).
- Most Unix variants using the GTK+ toolkit (version 2.6 or newer or 3.x).
- Mac OS X (10.7 or newer) using Cocoa (32/64 bits) or Carbon (32 only).

NB: Please, read file "BuildSVN.txt" in this directory as well. There are instructions for building from SVN, but as this GIT repository is regurarily updated from official SVN repository, and it is its fork, it is relevant to read it.

Most popular C++ compilers are supported including but not limited to:

NB: I only support x64 builds, as Win32 builds are known to be broken. This is due to the ICC bug.

- Microsoft Visual C++ 2003 or later (up to 2013).
- g++ 3.4 or later, including MinGW/MinGW-64/TDM under Windows.
- Clang under OS X and Linux.
- Intel icc compiler.
- Oracle (ex-Sun) aCC.

If you get errors with ICC 14 SP1 Update 1 with MSVS 2013 RTM like "TRACKER : error TRK0002: Failed to execute command [...] The operation identifier is not valid.", follow these two URL's for workaround: http://software.intel.com/en-us/forums/topic/486611 and http://software.intel.com/en-us/articles/fail-to-build-x64-configuration-in-visual-studio-2013-with-error-trk0002-failed-to-execute

Licence
-------

[wxWidgets licence](https://github.com/wxWidgets/wxWidgets/blob/master/docs/licence.txt)
is a modified version of LGPL explicitly allowing not distributing the sources
of an application using the library even in the case of static linking.

Please note ICC 14 SP1 Update 1 is broken, and unable to link the library (linker produces errors with unresolved external that actually doesn't exist). I recommend to use ICC 14 SP1 (first non-beta version), or even better the latest one: ICC 15.0 Update 1.

Best regards,
Further information
-------------------

Marian 'VooDooMan' Meravy
If you are looking for support, you can get it from

vdm 113 (<-remove the space character) [at sign] gmail (dot character) com

- wx-users [mailing list](http://www.wxwidgets.org/support/mailing-lists/)
- wxForum at http://forums.wxwidgets.org/
- #wxwidgets IRC channel
- http://stackoverflow.com/ if you tag your question with "wxwidgets"
- Please report bugs at http://trac.wxwidgets.org/newticket

We would also gladly welcome [your contributions](CONTRIBUTING.md).


Have fun!

The wxWidgets Team.
...and VooDooMan
.
