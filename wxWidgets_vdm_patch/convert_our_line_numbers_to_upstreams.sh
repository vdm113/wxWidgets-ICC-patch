#!/bin/sh

dos2unix log.txt

rm -f log_out.tmp

( cat log.txt | awk -f convert_our_line_numbers_to_upstreams.awk - ) >log_out.tmp

rm -f log_out2.tmp

sed -r 's/^[0-9]+>//' log_out.tmp >log_out2.tmp

rm -f log_out.tmp

( cat log_out2.tmp | sort | uniq ) >log_out.txt

rm -f log_out2.tmp

unix2dos log_out.txt

#eof
