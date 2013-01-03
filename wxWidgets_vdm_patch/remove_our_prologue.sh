#!/bin/sh

find ../ -type f -iname '*.h' -o -iname '*.hh' -o -iname '*.hpp' -o -iname '*.hxx' -o -iname '*.c' -o -iname '*.cc' -o -iname '*.cpp' -o -iname '*.cxx' | while read fn ; do echo "======= '${fn}' =======" ; awk -f remove_our_prologue.awk "${fn}" >"${fn},tmp" ; mv -f "${fn},tmp" "${fn}" ; done

#eof
