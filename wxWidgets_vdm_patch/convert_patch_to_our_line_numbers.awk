BEGIN {
    number_of_lines_in_prologue=7;

    FS="[ ,]";
}

/^@@ -[[:digit:]+,[[:digit:]]+ \+[[:digit:]]+,[[:digit:]]+ @@$/ {
    printf("%s %d,%d %d,%d %s\n",$1,( $2 - number_of_lines_in_prologue ),$3,( $2 - number_of_lines_in_prologue ),$5,$6);
    next;
}

// { print $0; }

#eof
