BEGIN {
    number_of_lines_in_prologue=7;

    FS="[()]";
}

/.*\([[:digit:]]+\): warning.*/ && !/warning #650: this pragma must immediately precede a statement/ {
    printf("%s(%d)%s\n",$1,( $2 - number_of_lines_in_prologue ),$3);
}

#eof
