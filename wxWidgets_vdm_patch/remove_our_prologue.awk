BEGIN {
    number_of_lines_in_prologue=7;

    process=0;
}

/^\/\* token_VDM_prologue \*\/$/ {
    if(1==NR) {
        process=1;
    }
}

// {
    if(1==process) {
        if(NR>number_of_lines_in_prologue) {
            print $0;
        }
    } else {
        print $0;
    }
}

#eof
