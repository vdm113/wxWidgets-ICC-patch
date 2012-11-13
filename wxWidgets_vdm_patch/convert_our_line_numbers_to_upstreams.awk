BEGIN {
    number_of_lines_in_prologue=7;

    FS="/(|)/";
}

/.*\([[:digit:]]+\):.*/ {
    printf("%s%d%s\n",$1,( 0 + $2 - number_of_lines_in_prologue ),$3);
}

#eof
