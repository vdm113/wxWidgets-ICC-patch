/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

#define xtifficon_width 32
#define xtifficon_height 32
static char xtifficon_bits[] = {
   0xff, 0x00, 0x00, 0xc0, 0xfe, 0x01, 0x7e, 0xc0, 0xfc, 0x03, 0x7e, 0x60,
   0xf8, 0x07, 0x06, 0x30, 0xf8, 0x07, 0x1e, 0x18, 0xf0, 0x0f, 0x1e, 0x0c,
   0xe0, 0x1f, 0x06, 0x06, 0xc0, 0x3f, 0x06, 0x06, 0xc0, 0x3f, 0x06, 0x03,
   0x80, 0x7f, 0x80, 0x01, 0x00, 0xff, 0xc0, 0x00, 0x00, 0xfe, 0x61, 0x00,
   0x00, 0xfe, 0x31, 0x7e, 0x7e, 0xfc, 0x33, 0x7e, 0x7e, 0xf8, 0x1b, 0x06,
   0x18, 0xf0, 0x0d, 0x1e, 0x18, 0xf0, 0x0e, 0x1e, 0x18, 0x60, 0x1f, 0x06,
   0x18, 0xb0, 0x3f, 0x06, 0x18, 0x98, 0x7f, 0x06, 0x18, 0x98, 0x7f, 0x00,
   0x00, 0x0c, 0xff, 0x00, 0x00, 0x06, 0xfe, 0x01, 0x00, 0x63, 0xfc, 0x03,
   0x80, 0x61, 0xfc, 0x03, 0xc0, 0x60, 0xf8, 0x07, 0xc0, 0x60, 0xf0, 0x0f,
   0x60, 0x60, 0xe0, 0x1f, 0x30, 0x60, 0xe0, 0x1f, 0x18, 0x60, 0xc0, 0x3f,
   0x0c, 0x60, 0x80, 0x7f, 0x06, 0x00, 0x00, 0xff};
/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 8
 * fill-column: 78
 * End:
 */
