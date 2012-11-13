#!/bin/bash

dos2unix log.txt

( cat log.txt | awk -f convert_our_line_numbers_to_upstreams.awk - ) >log_out.txt

unix2dos log_out.txt

