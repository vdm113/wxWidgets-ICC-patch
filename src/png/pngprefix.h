/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP __pragma(ivdep)
#elif !defined(MY_MACRO_PRAGMA_IVDEP)
#   define MY_MACRO_PRAGMA_IVDEP
#endif

#define png_sRGB_table wx_png_sRGB_table
#define png_sRGB_base wx_png_sRGB_base
#define png_sRGB_delta wx_png_sRGB_delta
#define png_zstream_error wx_png_zstream_error
#define png_free_buffer_list wx_png_free_buffer_list
#define png_fixed wx_png_fixed
#define png_user_version_check wx_png_user_version_check
#define png_malloc_base wx_png_malloc_base
#define png_malloc_array wx_png_malloc_array
#define png_realloc_array wx_png_realloc_array
#define png_create_png_struct wx_png_create_png_struct
#define png_destroy_png_struct wx_png_destroy_png_struct
#define png_free_jmpbuf wx_png_free_jmpbuf
#define png_zalloc wx_png_zalloc
#define png_zfree wx_png_zfree
#define png_default_read_data wx_png_default_read_data
#define png_push_fill_buffer wx_png_push_fill_buffer
#define png_default_write_data wx_png_default_write_data
#define png_default_flush wx_png_default_flush
#define png_reset_crc wx_png_reset_crc
#define png_write_data wx_png_write_data
#define png_read_sig wx_png_read_sig
#define png_read_chunk_header wx_png_read_chunk_header
#define png_read_data wx_png_read_data
#define png_crc_read wx_png_crc_read
#define png_crc_finish wx_png_crc_finish
#define png_crc_error wx_png_crc_error
#define png_calculate_crc wx_png_calculate_crc
#define png_flush wx_png_flush
#define png_write_IHDR wx_png_write_IHDR
#define png_write_PLTE wx_png_write_PLTE
#define png_compress_IDAT wx_png_compress_IDAT
#define png_write_IEND wx_png_write_IEND
#define png_write_gAMA_fixed wx_png_write_gAMA_fixed
#define png_write_sBIT wx_png_write_sBIT
#define png_write_cHRM_fixed wx_png_write_cHRM_fixed
#define png_write_sRGB wx_png_write_sRGB
#define png_write_iCCP wx_png_write_iCCP
#define png_write_sPLT wx_png_write_sPLT
#define png_write_tRNS wx_png_write_tRNS
#define png_write_bKGD wx_png_write_bKGD
#define png_write_hIST wx_png_write_hIST
#define png_write_tEXt wx_png_write_tEXt
#define png_write_zTXt wx_png_write_zTXt
#define png_write_iTXt wx_png_write_iTXt
#define png_set_text_2 wx_png_set_text_2
#define png_write_oFFs wx_png_write_oFFs
#define png_write_pCAL wx_png_write_pCAL
#define png_write_pHYs wx_png_write_pHYs
#define png_write_tIME wx_png_write_tIME
#define png_write_sCAL_s wx_png_write_sCAL_s
#define png_write_finish_row wx_png_write_finish_row
#define png_write_start_row wx_png_write_start_row
#define png_combine_row wx_png_combine_row
#define png_do_read_interlace wx_png_do_read_interlace
#define png_do_write_interlace wx_png_do_write_interlace
#define png_read_filter_row wx_png_read_filter_row
#define png_read_filter_row_up_neon wx_png_read_filter_row_up_neon
#define png_read_filter_row_sub3_neon wx_png_read_filter_row_sub3_neon
#define png_read_filter_row_sub4_neon wx_png_read_filter_row_sub4_neon
#define png_read_filter_row_avg3_neon wx_png_read_filter_row_avg3_neon
#define png_read_filter_row_avg4_neon wx_png_read_filter_row_avg4_neon
#define png_read_filter_row_paeth3_neon wx_png_read_filter_row_paeth3_neon
#define png_read_filter_row_paeth4_neon wx_png_read_filter_row_paeth4_neon
#define png_write_find_filter wx_png_write_find_filter
#define png_read_IDAT_data wx_png_read_IDAT_data
#define png_read_finish_IDAT wx_png_read_finish_IDAT
#define png_read_finish_row wx_png_read_finish_row
#define png_read_start_row wx_png_read_start_row
#define png_read_transform_info wx_png_read_transform_info
#define png_do_read_filler wx_png_do_read_filler
#define png_do_read_swap_alpha wx_png_do_read_swap_alpha
#define png_do_write_swap_alpha wx_png_do_write_swap_alpha
#define png_do_read_invert_alpha wx_png_do_read_invert_alpha
#define png_do_write_invert_alpha wx_png_do_write_invert_alpha
#define png_do_strip_channel wx_png_do_strip_channel
#define png_do_swap wx_png_do_swap
#define png_do_packswap wx_png_do_packswap
#define png_do_rgb_to_gray wx_png_do_rgb_to_gray
#define png_do_gray_to_rgb wx_png_do_gray_to_rgb
#define png_do_unpack wx_png_do_unpack
#define png_do_unshift wx_png_do_unshift
#define png_do_invert wx_png_do_invert
#define png_do_scale_16_to_8 wx_png_do_scale_16_to_8
#define png_do_chop wx_png_do_chop
#define png_do_quantize wx_png_do_quantize
#define png_do_bgr wx_png_do_bgr
#define png_do_pack wx_png_do_pack
#define png_do_shift wx_png_do_shift
#define png_do_compose wx_png_do_compose
#define png_do_gamma wx_png_do_gamma
#define png_do_encode_alpha wx_png_do_encode_alpha
#define png_do_expand_palette wx_png_do_expand_palette
#define png_do_expand wx_png_do_expand
#define png_do_expand_16 wx_png_do_expand_16
#define png_handle_IHDR wx_png_handle_IHDR
#define png_handle_PLTE wx_png_handle_PLTE
#define png_handle_IEND wx_png_handle_IEND
#define png_handle_bKGD wx_png_handle_bKGD
#define png_handle_cHRM wx_png_handle_cHRM
#define png_handle_gAMA wx_png_handle_gAMA
#define png_handle_hIST wx_png_handle_hIST
#define png_handle_iCCP wx_png_handle_iCCP
#define png_handle_iTXt wx_png_handle_iTXt
#define png_handle_oFFs wx_png_handle_oFFs
#define png_handle_pCAL wx_png_handle_pCAL
#define png_handle_pHYs wx_png_handle_pHYs
#define png_handle_sBIT wx_png_handle_sBIT
#define png_handle_sCAL wx_png_handle_sCAL
#define png_handle_sPLT wx_png_handle_sPLT
#define png_handle_sRGB wx_png_handle_sRGB
#define png_handle_tEXt wx_png_handle_tEXt
#define png_handle_tIME wx_png_handle_tIME
#define png_handle_tRNS wx_png_handle_tRNS
#define png_handle_zTXt wx_png_handle_zTXt
#define png_check_chunk_name wx_png_check_chunk_name
#define png_handle_unknown wx_png_handle_unknown
#define png_chunk_unknown_handling wx_png_chunk_unknown_handling
#define png_do_read_transformations wx_png_do_read_transformations
#define png_do_write_transformations wx_png_do_write_transformations
#define png_init_read_transformations wx_png_init_read_transformations
#define png_push_read_chunk wx_png_push_read_chunk
#define png_push_read_sig wx_png_push_read_sig
#define png_push_check_crc wx_png_push_check_crc
#define png_push_crc_skip wx_png_push_crc_skip
#define png_push_crc_finish wx_png_push_crc_finish
#define png_push_save_buffer wx_png_push_save_buffer
#define png_push_restore_buffer wx_png_push_restore_buffer
#define png_push_read_IDAT wx_png_push_read_IDAT
#define png_process_IDAT_data wx_png_process_IDAT_data
#define png_push_process_row wx_png_push_process_row
#define png_push_handle_unknown wx_png_push_handle_unknown
#define png_push_have_info wx_png_push_have_info
#define png_push_have_end wx_png_push_have_end
#define png_push_have_row wx_png_push_have_row
#define png_push_read_end wx_png_push_read_end
#define png_process_some_data wx_png_process_some_data
#define png_read_push_finish_row wx_png_read_push_finish_row
#define png_push_handle_tEXt wx_png_push_handle_tEXt
#define png_push_read_tEXt wx_png_push_read_tEXt
#define png_push_handle_zTXt wx_png_push_handle_zTXt
#define png_push_read_zTXt wx_png_push_read_zTXt
#define png_push_handle_iTXt wx_png_push_handle_iTXt
#define png_push_read_iTXt wx_png_push_read_iTXt
#define png_do_read_intrapixel wx_png_do_read_intrapixel
#define png_do_write_intrapixel wx_png_do_write_intrapixel
#define png_colorspace_set_gamma wx_png_colorspace_set_gamma
#define png_colorspace_sync_info wx_png_colorspace_sync_info
#define png_colorspace_sync wx_png_colorspace_sync
#define png_colorspace_set_chromaticities wx_png_colorspace_set_chromaticities
#define png_colorspace_set_endpoints wx_png_colorspace_set_endpoints
#define png_colorspace_set_sRGB wx_png_colorspace_set_sRGB
#define png_colorspace_set_ICC wx_png_colorspace_set_ICC
#define png_icc_check_length wx_png_icc_check_length
#define png_icc_check_header wx_png_icc_check_header
#define png_icc_check_tag_table wx_png_icc_check_tag_table
#define png_icc_set_sRGB wx_png_icc_set_sRGB
#define png_colorspace_set_rgb_coefficients wx_png_colorspace_set_rgb_coefficients
#define png_check_IHDR wx_png_check_IHDR
#define png_do_check_palette_indexes wx_png_do_check_palette_indexes
#define png_fixed_error wx_png_fixed_error
#define png_safecat wx_png_safecat
#define png_format_number wx_png_format_number
#define png_warning_parameter wx_png_warning_parameter
#define png_warning_parameter_unsigned wx_png_warning_parameter_unsigned
#define png_warning_parameter_signed wx_png_warning_parameter_signed
#define png_formatted_warning wx_png_formatted_warning
#define png_app_warning wx_png_app_warning
#define png_app_error wx_png_app_error
#define png_chunk_report wx_png_chunk_report
#define png_ascii_from_fp wx_png_ascii_from_fp
#define png_ascii_from_fixed wx_png_ascii_from_fixed
#define png_check_fp_number wx_png_check_fp_number
#define png_check_fp_string wx_png_check_fp_string
#define png_muldiv wx_png_muldiv
#define png_muldiv_warn wx_png_muldiv_warn
#define png_reciprocal wx_png_reciprocal
#define png_reciprocal2 wx_png_reciprocal2
#define png_gamma_significant wx_png_gamma_significant
#define png_gamma_correct wx_png_gamma_correct
#define png_gamma_16bit_correct wx_png_gamma_16bit_correct
#define png_gamma_8bit_correct wx_png_gamma_8bit_correct
#define png_destroy_gamma_table wx_png_destroy_gamma_table
#define png_build_gamma_table wx_png_build_gamma_table
#define png_safe_error wx_png_safe_error
#define png_safe_warning wx_png_safe_warning
#define png_safe_execute wx_png_safe_execute
#define png_image_error wx_png_image_error
