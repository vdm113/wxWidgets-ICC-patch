set Path=..\..\lib\vc_x64_dll\;%Path%
start bench_vc12.sln

rem --num-runs 1000 ParseHTML CharBuffer MSWCompareStringIgnoreCase MSWCompareString MSWlstrcmpi MSWlstrcmp StringCmpNoCase StringCmp StricmpW StricmpA StrcmpW StrcmpA Upper Lower ReplaceShorter ReplaceLonger ReplaceAll ReplaceSome ReplaceNone ReplaceLoop ForStringRIter ForStringIter ForStringIndex ForCString FromAsciiWithLen FromAsciiWithNpos FromAscii FromUTF8UncheckedWithLen FromUTF8UncheckedWithNpos FromUTF8Unchecked FromUTF8WithLen FromUTF8WithNpos FromUTF8 Strlen ShrinkHighQuality ShrinkNormal EnlargeHighQuality EnlargeNormal LoadTIFF LoadPNG LoadJPEG LoadBMP ParseDate
