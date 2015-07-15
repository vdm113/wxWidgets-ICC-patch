This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 15.0, though earlier versions might do work well.

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2013 for ICL 12.1/13.0/14.0/15.0.

The original sources were patched by (see above) SW for ICL 12.1, 13.0, 14.0 and 15.0. There was one assertion failure in the compiler (ICC 12.1) compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the ICC 12.1 compiler will abide by it.

The original sources were patched by (see above) SW for ICL 15.0.

I can do a very limited support... I support only ICC 15.0 under Windows's MSVC 2013 with x64 builds, though x86 (32-bit) builds might be successful, but I have not tried it (I mean x86).

This fork of wxWidgets is as of the official GIT repository:

https://github.com/wxWidgets/wxWidgets

If you are using 32-bit ICC, or 64-bit one and you don't have enough RAM or time, I recommend change for sub-library "core" from "/Qipo" to "/Qip".

NB: Please, read file "BuildSVN.txt" in this directory as well. There are instructions for building from SVN, but as this GIT repository is regularly updated from official SVN repository, and it is its fork, it is relevant to read it.

NB: I only support x64 builds, as Win32 builds are known to be broken. This is due to the ICC bug.

If you get errors with ICC 14 SP1 Update 1 with MSVS 2013 RTM like "TRACKER : error TRK0002: Failed to execute command [...] The operation identifier is not valid.", follow these two URL's for workaround:
http://software.intel.com/en-us/forums/topic/486611
http://software.intel.com/en-us/articles/fail-to-build-x64-configuration-in-visual-studio-2013-with-error-trk0002-failed-to-execute
Please note ICC 14 SP1 Update 1 is broken, and unable to link the library (linker produces errors with unresolved external that actually doesn't exist). I recommend to use ICC 14 SP1 (first non-beta version).

Best regards,
Marian 'VooDooMan' Meravy
vdm 113 (<-remove the space character) [at sign] gmail (dot character) com
.
