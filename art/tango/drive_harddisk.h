/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/* drive_harddisk.png - 603 bytes */
static const unsigned char drive_harddisk_16x16_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff,
  0x61, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4b, 0x47,
  0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
  0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70,
  0x48, 0x59, 0x73, 0x00, 0x00, 0x0b, 0x13, 0x00,
  0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18,
  0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45,
  0x07, 0xd5, 0x04, 0x05, 0x0f, 0x35, 0x08, 0xeb,
  0xcf, 0x7c, 0x77, 0x00, 0x00, 0x00, 0x8c, 0x74,
  0x45, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65,
  0x6e, 0x74, 0x00, 0x4d, 0x65, 0x6e, 0x75, 0x2d,
  0x73, 0x69, 0x7a, 0x65, 0x64, 0x20, 0x69, 0x63,
  0x6f, 0x6e, 0x0a, 0x3d, 0x3d, 0x3d, 0x3d, 0x3d,
  0x3d, 0x3d, 0x3d, 0x3d, 0x3d, 0x0a, 0x0a, 0x28,
  0x63, 0x29, 0x20, 0x32, 0x30, 0x30, 0x33, 0x20,
  0x4a, 0x61, 0x6b, 0x75, 0x62, 0x20, 0x27, 0x6a,
  0x69, 0x6d, 0x6d, 0x61, 0x63, 0x27, 0x20, 0x53,
  0x74, 0x65, 0x69, 0x6e, 0x65, 0x72, 0x2c, 0x20,
  0x0a, 0x68, 0x74, 0x74, 0x70, 0x3a, 0x2f, 0x2f,
  0x6a, 0x69, 0x6d, 0x6d, 0x61, 0x63, 0x2e, 0x6d,
  0x75, 0x73, 0x69, 0x63, 0x68, 0x61, 0x6c, 0x6c,
  0x2e, 0x63, 0x7a, 0x0a, 0x0a, 0x63, 0x72, 0x65,
  0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74,
  0x68, 0x20, 0x74, 0x68, 0x65, 0x20, 0x47, 0x49,
  0x4d, 0x50, 0x2c, 0x0a, 0x68, 0x74, 0x74, 0x70,
  0x3a, 0x2f, 0x2f, 0x77, 0x77, 0x77, 0x2e, 0x67,
  0x69, 0x6d, 0x70, 0x2e, 0x6f, 0x72, 0x67, 0x67,
  0x8a, 0xc7, 0x47, 0x00, 0x00, 0x01, 0x50, 0x49,
  0x44, 0x41, 0x54, 0x38, 0xcb, 0xa5, 0x93, 0x4d,
  0x6e, 0xc2, 0x30, 0x10, 0x85, 0xbf, 0xa0, 0x40,
  0xa4, 0x20, 0xcb, 0xc9, 0x26, 0x6a, 0x24, 0xf6,
  0x70, 0x37, 0x24, 0xc4, 0x0d, 0xda, 0xde, 0x90,
  0x15, 0x27, 0x40, 0x2c, 0xd8, 0x41, 0x6c, 0x10,
  0xf9, 0x99, 0xe9, 0x22, 0xe0, 0x28, 0xed, 0xa2,
  0x55, 0x99, 0x85, 0xed, 0x19, 0xcf, 0x7b, 0x7e,
  0x33, 0xb6, 0xe1, 0x45, 0x8b, 0x36, 0xdb, 0xf5,
  0x07, 0xf0, 0xfe, 0x4f, 0xfc, 0x27, 0x9b, 0xed,
  0x5a, 0xff, 0x63, 0xce, 0x39, 0xdd, 0x6c, 0xd7,
  0x1a, 0x03, 0x88, 0x08, 0xe7, 0xf3, 0x19, 0x55,
  0x85, 0x28, 0x02, 0xd5, 0xb1, 0x4c, 0x40, 0x9f,
  0x43, 0x04, 0xa0, 0xcc, 0x66, 0x33, 0x00, 0x62,
  0x00, 0x15, 0xa5, 0x13, 0xe9, 0xb3, 0x45, 0x06,
  0x00, 0x10, 0x11, 0x21, 0xaa, 0x7d, 0x8c, 0xa8,
  0xc7, 0xf7, 0xce, 0x40, 0x20, 0x2a, 0x48, 0x27,
  0x44, 0x0f, 0x90, 0x02, 0xa8, 0x3e, 0x00, 0xca,
  0x53, 0x18, 0xaa, 0x3d, 0xb1, 0xd0, 0xab, 0x0d,
  0x0a, 0x54, 0xc9, 0xf3, 0x8c, 0xd3, 0xe9, 0xf4,
  0xa7, 0xce, 0x65, 0x59, 0x46, 0xd7, 0x75, 0x00,
  0x4c, 0x9e, 0x41, 0xe7, 0x1c, 0x45, 0x51, 0xfc,
  0x0a, 0x2e, 0x8a, 0x82, 0xba, 0xae, 0x83, 0x1f,
  0x9a, 0xe8, 0xbd, 0x0f, 0x09, 0x00, 0xd7, 0xeb,
  0x95, 0xaa, 0xaa, 0x00, 0x30, 0xc6, 0x90, 0xa6,
  0x29, 0x93, 0xc9, 0x84, 0xcb, 0xe5, 0x82, 0xf7,
  0x1e, 0x63, 0xcc, 0x40, 0x90, 0x24, 0x09, 0x8b,
  0xc5, 0x62, 0x74, 0x92, 0xb5, 0x16, 0x6b, 0x2d,
  0x00, 0xde, 0x7b, 0xea, 0xba, 0xe6, 0x76, 0xbb,
  0xfd, 0x50, 0x14, 0x03, 0xec, 0x76, 0x3b, 0x9a,
  0xa6, 0x19, 0x6d, 0xb4, 0x6d, 0x4b, 0x1c, 0xc7,
  0x61, 0xfd, 0xdd, 0xf2, 0x3c, 0x1f, 0xf7, 0xa0,
  0x6d, 0x5b, 0xf6, 0xfb, 0x3d, 0xc7, 0xe3, 0x91,
  0xaa, 0xaa, 0x50, 0x55, 0x54, 0x95, 0x24, 0x49,
  0x38, 0x1c, 0x0e, 0x18, 0x63, 0xc2, 0xec, 0x9c,
  0xc3, 0x39, 0x37, 0x10, 0xa8, 0x2a, 0x22, 0xc2,
  0x7c, 0x9e, 0x32, 0x9d, 0x4e, 0xc3, 0x23, 0x01,
  0x68, 0x9a, 0x06, 0x95, 0x9e, 0x4c, 0x45, 0xfa,
  0xdc, 0xc7, 0x0d, 0x84, 0x12, 0xb2, 0x2c, 0xc3,
  0x5a, 0x4b, 0x59, 0x96, 0x00, 0xdc, 0xef, 0x77,
  0x80, 0xd0, 0xed, 0xe5, 0x6a, 0x89, 0x88, 0xb0,
  0x5c, 0xad, 0x10, 0x11, 0xde, 0xca, 0x32, 0x94,
  0xfc, 0xfa, 0x67, 0x7a, 0xd5, 0xbe, 0x00, 0x97,
  0x55, 0xed, 0x26, 0xb6, 0x6c, 0xb5, 0x0b, 0x00,
  0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae,
  0x42, 0x60, 0x82};


/* drive_harddisk.png - 881 bytes */
static const unsigned char drive_harddisk_24x24_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a,
  0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18,
  0x10, 0x06, 0x00, 0x00, 0x00, 0xb0, 0xe7, 0xe1,
  0xbb, 0x00, 0x00, 0x00, 0x06, 0x62, 0x4b, 0x47,
  0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf9,
  0x43, 0xbb, 0x7f, 0x00, 0x00, 0x00, 0x09, 0x70,
  0x48, 0x59, 0x73, 0x00, 0x00, 0x0d, 0xd7, 0x00,
  0x00, 0x0d, 0xd7, 0x01, 0x42, 0x28, 0x9b, 0x78,
  0x00, 0x00, 0x00, 0x09, 0x76, 0x70, 0x41, 0x67,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x18,
  0x00, 0x78, 0x4c, 0xa5, 0xa6, 0x00, 0x00, 0x02,
  0xfc, 0x49, 0x44, 0x41, 0x54, 0x58, 0xc3, 0xed,
  0x57, 0x4d, 0x4b, 0xeb, 0x40, 0x14, 0xbd, 0x93,
  0xa4, 0x4d, 0xd3, 0xa4, 0x36, 0x21, 0xb4, 0xa6,
  0x95, 0x2a, 0xb5, 0x35, 0xa4, 0x96, 0xd6, 0x8f,
  0x5d, 0xd5, 0x5f, 0xe0, 0xa2, 0x5b, 0x7f, 0x80,
  0x82, 0x0b, 0x41, 0xdc, 0x88, 0x22, 0xae, 0xc5,
  0x8f, 0x8d, 0x08, 0x82, 0xd0, 0x8d, 0x20, 0x2e,
  0xf4, 0x47, 0x89, 0x16, 0x21, 0xd2, 0x12, 0x42,
  0x4b, 0xd2, 0x4f, 0xdb, 0xbc, 0x45, 0x18, 0xf3,
  0x48, 0xed, 0xb3, 0x4f, 0xeb, 0xf3, 0x2d, 0x7a,
  0x20, 0x24, 0x67, 0x26, 0x93, 0x39, 0xe7, 0xde,
  0xb9, 0x33, 0x04, 0x60, 0x84, 0x11, 0xbe, 0x04,
  0xd4, 0xaf, 0x63, 0x7b, 0x7b, 0x67, 0x67, 0x6b,
  0x6b, 0x73, 0xd3, 0x66, 0x57, 0x57, 0x3f, 0x2b,
  0x73, 0x6f, 0xef, 0xe2, 0xe2, 0xfc, 0xfc, 0xf2,
  0xf2, 0xf4, 0xd4, 0xdd, 0x43, 0xbd, 0x2f, 0x3c,
  0x12, 0xc1, 0xc2, 0x8f, 0x8f, 0x8f, 0x8e, 0xce,
  0xce, 0x7e, 0x4e, 0xfa, 0xfe, 0xfe, 0xc1, 0xc1,
  0xee, 0xee, 0xc9, 0x89, 0xcd, 0x7a, 0x0d, 0x10,
  0xef, 0x0f, 0x4b, 0xa7, 0x43, 0xa1, 0x50, 0x28,
  0x14, 0x02, 0xa8, 0xd5, 0x6a, 0xb5, 0x5a, 0xcd,
  0xe9, 0x31, 0x4d, 0xd3, 0x34, 0xcd, 0xef, 0xbb,
  0x17, 0x8b, 0xcf, 0xcf, 0xc5, 0xa2, 0xc3, 0xb1,
  0x0e, 0x3b, 0xb0, 0xf3, 0xf3, 0x03, 0x1a, 0x48,
  0x24, 0x24, 0x49, 0x92, 0x70, 0x1e, 0x00, 0x00,
  0x0c, 0xc3, 0x34, 0x0d, 0x03, 0x80, 0x65, 0x59,
  0x96, 0x65, 0x87, 0x1f, 0x69, 0x4d, 0xd3, 0x75,
  0x4d, 0x03, 0x60, 0x18, 0x9f, 0x8f, 0x61, 0x9c,
  0x76, 0x41, 0x10, 0x04, 0x41, 0xc0, 0x4c, 0x96,
  0x07, 0x34, 0x90, 0xcf, 0x2f, 0x2c, 0xcc, 0xcd,
  0x2d, 0x2e, 0x02, 0xd4, 0xeb, 0xf5, 0x7a, 0xbd,
  0x0e, 0x80, 0x90, 0x7d, 0xe1, 0xc8, 0x0c, 0x1b,
  0x3c, 0x1f, 0x0c, 0xf2, 0x3c, 0x80, 0xdf, 0xcf,
  0x30, 0x7e, 0xbf, 0xd3, 0xbe, 0xbc, 0x9c, 0xcb,
  0xad, 0xac, 0x60, 0x96, 0xc9, 0x0c, 0x68, 0x60,
  0x75, 0x35, 0x1a, 0x8d, 0x46, 0x27, 0x26, 0x86,
  0x2f, 0xb4, 0x1f, 0x48, 0x92, 0x20, 0x48, 0xb2,
  0xb7, 0x3d, 0x16, 0x8b, 0xc5, 0xa6, 0xa6, 0x30,
  0x3b, 0x3c, 0xec, 0x6b, 0x00, 0xaf, 0x31, 0x86,
  0xb1, 0x23, 0xe0, 0xf5, 0x7a, 0xbd, 0x5e, 0x2f,
  0x80, 0x28, 0x8a, 0xa2, 0x28, 0xfe, 0x3b, 0x23,
  0x18, 0xee, 0xa5, 0x2a, 0x08, 0x82, 0x20, 0x8a,
  0xbd, 0xb5, 0xe0, 0xca, 0x80, 0x2c, 0x8f, 0x8f,
  0x87, 0xc3, 0x92, 0xe4, 0xb4, 0xe8, 0xba, 0xae,
  0xeb, 0xba, 0xf3, 0x41, 0x7c, 0x27, 0x08, 0x82,
  0x20, 0x08, 0x18, 0x1a, 0xdc, 0xdf, 0xc7, 0xf3,
  0x62, 0x04, 0x83, 0x63, 0x63, 0xc1, 0xa0, 0xa3,
  0x13, 0x3f, 0xb9, 0xb6, 0xd1, 0x4c, 0x66, 0x76,
  0x36, 0x95, 0x4a, 0xa7, 0x01, 0xda, 0xed, 0x76,
  0xbb, 0xdd, 0xb6, 0x2c, 0x8a, 0xa2, 0x28, 0x92,
  0x44, 0xa8, 0x52, 0xa9, 0x54, 0x2a, 0x15, 0x80,
  0x6e, 0xb7, 0xdb, 0xed, 0x76, 0xdd, 0xc5, 0xf5,
  0x75, 0xb8, 0x05, 0x77, 0x3a, 0x9d, 0xce, 0xeb,
  0x2b, 0x80, 0x61, 0x18, 0x46, 0xbb, 0x6d, 0x59,
  0x89, 0xc4, 0xf4, 0x74, 0x22, 0x81, 0xd0, 0xe3,
  0xe3, 0xd3, 0xd3, 0xc3, 0x43, 0x2e, 0x67, 0xbf,
  0x75, 0x7f, 0xff, 0x66, 0xa0, 0xd9, 0x6c, 0x36,
  0x9b, 0xcd, 0x6c, 0x56, 0x92, 0x24, 0x49, 0x92,
  0x00, 0x38, 0x8e, 0xe3, 0x38, 0x0e, 0xa1, 0x6a,
  0xb5, 0x5a, 0xad, 0x56, 0x3f, 0x9e, 0x70, 0x58,
  0xc0, 0xc2, 0xed, 0x2d, 0x03, 0x07, 0x0a, 0xa1,
  0x58, 0x2c, 0x16, 0x9b, 0x9c, 0x04, 0x68, 0x34,
  0x1a, 0x8d, 0x66, 0x33, 0x10, 0xe8, 0xc9, 0x00,
  0x4d, 0xd3, 0x34, 0x4d, 0xe7, 0xf3, 0xb2, 0x2c,
  0xcb, 0xbf, 0x6f, 0x56, 0xc3, 0x8e, 0xf4, 0x67,
  0xa1, 0x28, 0x8a, 0xa2, 0x28, 0x00, 0x3e, 0x9f,
  0xcf, 0x47, 0xd3, 0xeb, 0xeb, 0x76, 0xeb, 0xc6,
  0x46, 0xcf, 0x49, 0x5c, 0x2e, 0x97, 0xcb, 0xe5,
  0x32, 0x80, 0xa6, 0x69, 0x9a, 0xa6, 0xfd, 0xfd,
  0x44, 0x96, 0x65, 0x59, 0x96, 0x35, 0xfc, 0x71,
  0xf8, 0x40, 0x73, 0x83, 0xea, 0x37, 0xa0, 0xd5,
  0x6a, 0xb5, 0x5a, 0x2d, 0x80, 0x6c, 0x36, 0x9b,
  0xcd, 0x66, 0x01, 0x0a, 0x85, 0x42, 0xa1, 0x50,
  0xc0, 0x99, 0x02, 0x60, 0x59, 0x8e, 0xf3, 0xfb,
  0x01, 0x02, 0x01, 0x8e, 0x1b, 0x1b, 0x73, 0x8a,
  0x9a, 0x61, 0x18, 0x86, 0x61, 0x00, 0x10, 0x42,
  0x08, 0x21, 0x87, 0xdb, 0x4b, 0xd2, 0x59, 0x7a,
  0xe1, 0x70, 0x38, 0x1c, 0x0e, 0x03, 0x94, 0x4a,
  0xa5, 0x52, 0xa9, 0xf4, 0x31, 0xef, 0x67, 0x60,
  0xe0, 0x7d, 0x84, 0x24, 0x49, 0x92, 0x24, 0x9d,
  0x48, 0x11, 0x04, 0x42, 0x24, 0x89, 0x6b, 0xc7,
  0x31, 0x8c, 0xfb, 0x3d, 0x1e, 0x8f, 0xc7, 0xe3,
  0xf9, 0x7c, 0x46, 0x06, 0x05, 0x35, 0xe8, 0x8b,
  0xa9, 0x54, 0x2a, 0xa5, 0x28, 0xb6, 0x20, 0x00,
  0x00, 0x8a, 0xa2, 0x28, 0xea, 0x0f, 0xa3, 0xf1,
  0x39, 0xf2, 0xdd, 0xe8, 0x91, 0x60, 0xa7, 0xcc,
  0xb2, 0x22, 0x91, 0x48, 0x24, 0x12, 0x41, 0x08,
  0xd7, 0x44, 0x32, 0x99, 0x4c, 0xce, 0xcc, 0x7c,
  0x7d, 0x42, 0x9e, 0xe7, 0x79, 0x9e, 0x77, 0x78,
  0x3c, 0x1e, 0x8f, 0xc7, 0xe3, 0x1f, 0x73, 0x55,
  0x55, 0x55, 0x55, 0xc5, 0xb9, 0x44, 0x6f, 0xbf,
  0x01, 0x6f, 0x06, 0x54, 0x55, 0x55, 0x5f, 0x5e,
  0xd6, 0xd6, 0x6e, 0x6e, 0x6e, 0x6f, 0xaf, 0xaf,
  0xef, 0xee, 0xbe, 0x3f, 0x76, 0x9f, 0x01, 0x42,
  0xb6, 0xce, 0xa5, 0xa5, 0x9f, 0x56, 0x32, 0xc2,
  0x08, 0xff, 0x0b, 0x7e, 0x01, 0xa0, 0xaf, 0x82,
  0x1b, 0x07, 0x0a, 0xd5, 0xef, 0x00, 0x00, 0x00,
  0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
  0x82};
