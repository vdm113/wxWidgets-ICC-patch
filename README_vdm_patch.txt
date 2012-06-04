This is cutting-edge performance preprocessed code of wxWidgets for Intel CPU's fork of wxWidgets, to be compiled with ICL 12.1.

The main preprocessing/modification SW is under directory "wxWidgets_vdm_patch" for MSVC 2010 with ICL 12.1.

The original sources were patched by (see above) SW for ICL 12.1. There was one assertion failure in the compiler compiling this code, so if you encounter this behaviour add to preprocessor directive for ICL " && 0" and (above mentioned), the SW will abide by it.

I can do a very limited support... I support only ICL 12.1 under Windows's MSVC 2010 with x64 builds, though x86 (32-bit) builds might be successful.

This fork of wxWidgets is as of official SVN revision:

r71616

Also, it uses my own pre-configured "setup.h" configuration. I have changed only these parameters (compared to default 'setup0.h'):
wxUSE_THEME_WIN32 set to 1
wxUSE_INICONF set to 1

I have recently changed all profiles and architectures (unsupported x86, and x64) to use "/Qip" ICL switch rather than "/Qipo". So Intra-procedural Optimization will be performed over each .obj, rather then multi-.obj manner. Performance impact of resulting code for my benchmarks is negligible, and compilation will not take 2 days (on my 8 GiB RAM machine) while compiler is requesting like 12 GiB of (virtual) RAM (was swapping).

Best regards,
VooDooMan
.
