/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif



#include <stdio.h>

typedef struct {
        unsigned char c;
        unsigned short u;
    } charsetItem;



int cmpt(const void *i1, const void *i2)
{
    unsigned short u1 = ((charsetItem*)i1) -> u;
    unsigned short u2 = ((charsetItem*)i2) -> u;
    return (u1 - u2);
}



int main(int argc, char *argv[])
{
    unsigned enc, unic;
    unsigned i;
    charsetItem table[256];

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for (i = 0; i < 256; i++) { table[i].c = i, table[i].u = 0; /* unknown */}

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    while (!feof(stdin))
    {
        scanf("%i\t%i\n", &enc, &unic);
        table[enc].u = unic;
        table[enc].c = enc;
        if (enc < 128 && enc != unic) 
            fprintf(stderr, "7bit ASCII incompatibilit (%s): %i->%i\n", 
                    argv[2], enc, unic);
    }
    
    /* dump it: */
    
    printf("\n\n"
           "/*\n"
           " * %s to Unicode recoding table\n"
           " * based on file %s by Unicode Consortium\n"
           " */\n\n"
           "static const wxUint16 encoding_table__%s[128] = {",
           argv[2], argv[1], argv[2]);
           
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
    for (i = 128; i < 256; i++)
    { 
        if (i % 8 == 0)
            printf("\n   ");
        printf(" 0x%04X%c", table[i].u, (i == 255) ? '\n' : ',');
    }
    printf(" };\n");

    qsort(table + 128, 128, sizeof(table[0]), cmpt);


/*
    NO, WE DON'T NEED REVERSE TABLE, WE CAN BUILD IT AT RUNTIME
    (won't take that much time, after all you don't init
    conversion so often...)

    printf("\n"
           "static wxUint16 encoding_table_rev__%s[128] = {",
           argv[2]);
           
    for (i = 128; i < 256; i++)
    { 
        if (i % 4 == 0)
            printf("\n    ");
        printf("{c:0x%02X,u:0x%04X}%c ", table[i].c, table[i].u, (i == 255) ? '\n' : ',');
    }
    printf("};\n");
*/

    return 1;
}
