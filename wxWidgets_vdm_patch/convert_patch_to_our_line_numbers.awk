BEGIN {
    number_of_lines_in_prologue=7;

    FS="[ ,]";
}

/^@@ [+-][[:digit:]+,[[:digit:]]+ [+-][[:digit:]]+,[[:digit:]]+ @@$/ {
    first=$2 - number_of_lines_in_prologue;
    if(first>0)
        first="+" first;
    second=$4 + number_of_lines_in_prologue;
    if(second>0)
        second="+" second;
    printf("%s %s,%d %s,%d %s\n",$1,first,$3,second,$5,$6);
    next;
}

// { print $0; }

#eof
