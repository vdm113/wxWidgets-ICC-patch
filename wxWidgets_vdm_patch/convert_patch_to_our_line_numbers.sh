#!/bin/sh

dos2unix patch_in.patch

rm -f patch_out.patch

awk -f convert_patch_to_our_line_numbers.awk patch_in.patch >patch_out.patch

dos2unix patch_out.patch

#eof
