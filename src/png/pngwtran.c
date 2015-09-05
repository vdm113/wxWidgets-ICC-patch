/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll)
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP
#endif


/* pngwtran.c - transforms the data in a row for PNG writers
 *
 * Last changed in libpng 1.6.0 [February 14, 2013]
 * Copyright (c) 1998-2013 Glenn Randers-Pehrson
 * (Version 0.96 Copyright (c) 1996, 1997 Andreas Dilger)
 * (Version 0.88 Copyright (c) 1995, 1996 Guy Eric Schalnat, Group 42, Inc.)
 *
 * This code is released under the libpng license.
 * For conditions of distribution and use, see the disclaimer
 * and license in png.h
 */

#include "pngpriv.h"

#ifdef PNG_WRITE_SUPPORTED

#ifdef PNG_WRITE_TRANSFORMS_SUPPORTED
/* Transform the data according to the user's wishes.  The order of
 * transformations is significant.
 */
void /* PRIVATE */
png_do_write_transformations(png_structrp png_ptr, png_row_infop row_info)
{
   png_debug(1, "in png_do_write_transformations");

   if (png_ptr == NULL)
      return;

#ifdef PNG_WRITE_USER_TRANSFORM_SUPPORTED
   if (png_ptr->transformations & PNG_USER_TRANSFORM)
      if (png_ptr->write_user_transform_fn != NULL)
         (*(png_ptr->write_user_transform_fn)) /* User write transform
                                                 function */
             (png_ptr,  /* png_ptr */
             row_info,  /* row_info: */
                /*  png_uint_32 width;       width of row */
                /*  png_size_t rowbytes;     number of bytes in row */
                /*  png_byte color_type;     color type of pixels */
                /*  png_byte bit_depth;      bit depth of samples */
                /*  png_byte channels;       number of channels (1-4) */
                /*  png_byte pixel_depth;    bits per pixel (depth*channels) */
             png_ptr->row_buf + 1);      /* start of pixel data for row */
#endif

#ifdef PNG_WRITE_FILLER_SUPPORTED
   if (png_ptr->transformations & PNG_FILLER)
      png_do_strip_channel(row_info, png_ptr->row_buf + 1,
         !(png_ptr->flags & PNG_FLAG_FILLER_AFTER));
#endif

#ifdef PNG_WRITE_PACKSWAP_SUPPORTED
   if (png_ptr->transformations & PNG_PACKSWAP)
      png_do_packswap(row_info, png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_PACK_SUPPORTED
   if (png_ptr->transformations & PNG_PACK)
      png_do_pack(row_info, png_ptr->row_buf + 1,
          (png_uint_32)png_ptr->bit_depth);
#endif

#ifdef PNG_WRITE_SWAP_SUPPORTED
   if (png_ptr->transformations & PNG_SWAP_BYTES)
      png_do_swap(row_info, png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
   if (png_ptr->transformations & PNG_SHIFT)
      png_do_shift(row_info, png_ptr->row_buf + 1,
          &(png_ptr->shift));
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
   if (png_ptr->transformations & PNG_SWAP_ALPHA)
      png_do_write_swap_alpha(row_info, png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
   if (png_ptr->transformations & PNG_INVERT_ALPHA)
      png_do_write_invert_alpha(row_info, png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_BGR_SUPPORTED
   if (png_ptr->transformations & PNG_BGR)
      png_do_bgr(row_info, png_ptr->row_buf + 1);
#endif

#ifdef PNG_WRITE_INVERT_SUPPORTED
   if (png_ptr->transformations & PNG_INVERT_MONO)
      png_do_invert(row_info, png_ptr->row_buf + 1);
#endif
}

#ifdef PNG_WRITE_PACK_SUPPORTED
/* Pack pixels into bytes.  Pass the true bit depth in bit_depth.  The
 * row_info bit depth should be 8 (one pixel per byte).  The channels
 * should be 1 (this only happens on grayscale and paletted images).
 */
void /* PRIVATE */
png_do_pack(png_row_infop row_info, png_bytep row, png_uint_32 bit_depth)
{
   png_debug(1, "in png_do_pack");

   if (row_info->bit_depth == 8 &&
      row_info->channels == 1)
   {
      switch ((int)bit_depth)
      {
         case 1:
         {
            png_bytep sp, dp;
            int mask, v;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            mask = 0x80;
            v = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0; i < row_width; i++)
            {
               if (*sp != 0)
                  v |= mask;

               sp++;

               if (mask > 1)
                  mask >>= 1;

               else
               {
                  mask = 0x80;
                  *dp = (png_byte)v;
                  dp++;
                  v = 0;
               }
            }

            if (mask != 0x80)
               *dp = (png_byte)v;

            break;
         }

         case 2:
         {
            png_bytep sp, dp;
            int shift, v;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            shift = 6;
            v = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0; i < row_width; i++)
            {
               png_byte value;

               value = (png_byte)(*sp & 0x03);
               v |= (value << shift);

               if (shift == 0)
               {
                  shift = 6;
                  *dp = (png_byte)v;
                  dp++;
                  v = 0;
               }

               else
                  shift -= 2;

               sp++;
            }

            if (shift != 6)
               *dp = (png_byte)v;

            break;
         }

         case 4:
         {
            png_bytep sp, dp;
            int shift, v;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

            sp = row;
            dp = row;
            shift = 4;
            v = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0; i < row_width; i++)
            {
               png_byte value;

               value = (png_byte)(*sp & 0x0f);
               v |= (value << shift);

               if (shift == 0)
               {
                  shift = 4;
                  *dp = (png_byte)v;
                  dp++;
                  v = 0;
               }

               else
                  shift -= 4;

               sp++;
            }

            if (shift != 4)
               *dp = (png_byte)v;

            break;
         }

         default:
            break;
      }

      row_info->bit_depth = (png_byte)bit_depth;
      row_info->pixel_depth = (png_byte)(bit_depth * row_info->channels);
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth,
          row_info->width);
   }
}
#endif

#ifdef PNG_WRITE_SHIFT_SUPPORTED
/* Shift pixel values to take advantage of whole range.  Pass the
 * true number of bits in bit_depth.  The row should be packed
 * according to row_info->bit_depth.  Thus, if you had a row of
 * bit depth 4, but the pixels only had values from 0 to 7, you
 * would pass 3 as bit_depth, and this routine would translate the
 * data to 0 to 15.
 */
void /* PRIVATE */
png_do_shift(png_row_infop row_info, png_bytep row,
    png_const_color_8p bit_depth)
{
   png_debug(1, "in png_do_shift");

   if (row_info->color_type != PNG_COLOR_TYPE_PALETTE)
   {
      int shift_start[4], shift_dec[4];
      int channels = 0;

      if (row_info->color_type & PNG_COLOR_MASK_COLOR)
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->red;
         shift_dec[channels] = bit_depth->red;
         channels++;

         shift_start[channels] = row_info->bit_depth - bit_depth->green;
         shift_dec[channels] = bit_depth->green;
         channels++;

         shift_start[channels] = row_info->bit_depth - bit_depth->blue;
         shift_dec[channels] = bit_depth->blue;
         channels++;
      }

      else
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->gray;
         shift_dec[channels] = bit_depth->gray;
         channels++;
      }

      if (row_info->color_type & PNG_COLOR_MASK_ALPHA)
      {
         shift_start[channels] = row_info->bit_depth - bit_depth->alpha;
         shift_dec[channels] = bit_depth->alpha;
         channels++;
      }

      /* With low row depths, could only be grayscale, so one channel */
      if (row_info->bit_depth < 8)
      {
         png_bytep bp = row;
         png_size_t i;
         unsigned int mask;
         png_size_t row_bytes = row_info->rowbytes;

         if (bit_depth->gray == 1 && row_info->bit_depth == 2)
            mask = 0x55;

         else if (row_info->bit_depth == 4 && bit_depth->gray == 3)
            mask = 0x11;

         else
            mask = 0xff;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
         for (i = 0; i < row_bytes; i++, bp++)
         {
            int j;
            unsigned int v, out;

            v = *bp;
            out = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (j = shift_start[0]; j > -shift_dec[0]; j -= shift_dec[0])
            {
               if (j > 0)
                  out |= v << j;

               else
                  out |= (v >> (-j)) & mask;
            }

            *bp = (png_byte)(out & 0xff);
         }
      }

      else if (row_info->bit_depth == 8)
      {
         png_bytep bp = row;
         png_uint_32 i;
         png_uint_32 istop = channels * row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
         for (i = 0; i < istop; i++, bp++)
         {

            const unsigned int c = i%channels;
            int j;
            unsigned int v, out;

            v = *bp;
            out = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (j = shift_start[c]; j > -shift_dec[c]; j -= shift_dec[c])
            {
               if (j > 0)
                  out |= v << j;

               else
                  out |= v >> (-j);
            }

            *bp = (png_byte)(out & 0xff);
         }
      }

      else
      {
         png_bytep bp;
         png_uint_32 i;
         png_uint_32 istop = channels * row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
         for (bp = row, i = 0; i < istop; i++)
         {
            const unsigned int c = i%channels;
            int j;
            unsigned int value, v;

            v = png_get_uint_16(bp);
            value = 0;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (j = shift_start[c]; j > -shift_dec[c]; j -= shift_dec[c])
            {
               if (j > 0)
                  value |= v << j;

               else
                  value |= v >> (-j);
            }
            *bp++ = (png_byte)((value >> 8) & 0xff);
            *bp++ = (png_byte)(value & 0xff);
         }
      }
   }
}
#endif

#ifdef PNG_WRITE_SWAP_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_write_swap_alpha(png_row_infop row_info, png_bytep row)
{
   png_debug(1, "in png_do_write_swap_alpha");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This converts from ARGB to RGBA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               png_byte save = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save;
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This converts from AARRGGBB to RRGGBBAA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               png_byte save[2];
               save[0] = *(sp++);
               save[1] = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save[0];
               *(dp++) = save[1];
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This converts from AG to GA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               png_byte save = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save;
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This converts from AAGG to GGAA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               png_byte save[2];
               save[0] = *(sp++);
               save[1] = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = save[0];
               *(dp++) = save[1];
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }
   }
}
#endif

#ifdef PNG_WRITE_INVERT_ALPHA_SUPPORTED
void /* PRIVATE */
png_do_write_invert_alpha(png_row_infop row_info, png_bytep row)
{
   png_debug(1, "in png_do_write_invert_alpha");

   {
      if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This inverts the alpha channel in RGBA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=3; dp = sp;
               *(dp++) = (png_byte)(255 - *(sp++));
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This inverts the alpha channel in RRGGBBAA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=6; dp = sp;
               *(dp++) = (png_byte)(255 - *(sp++));
               *(dp++) = (png_byte)(255 - *(sp++));
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }

      else if (row_info->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      {
         if (row_info->bit_depth == 8)
         {
            /* This inverts the alpha channel in GA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               *(dp++) = *(sp++);
               *(dp++) = (png_byte)(255 - *(sp++));
            }
         }

#ifdef PNG_WRITE_16BIT_SUPPORTED
         else
         {
            /* This inverts the alpha channel in GGAA */
            png_bytep sp, dp;
            png_uint_32 i;
            png_uint_32 row_width = row_info->width;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
            for (i = 0, sp = dp = row; i < row_width; i++)
            {
               /* Does nothing
               *(dp++) = *(sp++);
               *(dp++) = *(sp++);
               */
               sp+=2; dp = sp;
               *(dp++) = (png_byte)(255 - *(sp++));
               *(dp++) = (png_byte)(255 - *(sp++));
            }
         }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
      }
   }
}
#endif
#endif /* PNG_WRITE_TRANSFORMS_SUPPORTED */

#ifdef PNG_MNG_FEATURES_SUPPORTED
/* Undoes intrapixel differencing  */
void /* PRIVATE */
png_do_write_intrapixel(png_row_infop row_info, png_bytep row)
{
   png_debug(1, "in png_do_write_intrapixel");

   if ((row_info->color_type & PNG_COLOR_MASK_COLOR))
   {
      int bytes_per_pixel;
      png_uint_32 row_width = row_info->width;
      if (row_info->bit_depth == 8)
      {
         png_bytep rp;
         png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 3;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 4;

         else
            return;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            *(rp)     = (png_byte)((*rp       - *(rp + 1)) & 0xff);
            *(rp + 2) = (png_byte)((*(rp + 2) - *(rp + 1)) & 0xff);
         }
      }

#ifdef PNG_WRITE_16BIT_SUPPORTED
      else if (row_info->bit_depth == 16)
      {
         png_bytep rp;
         png_uint_32 i;

         if (row_info->color_type == PNG_COLOR_TYPE_RGB)
            bytes_per_pixel = 6;

         else if (row_info->color_type == PNG_COLOR_TYPE_RGB_ALPHA)
            bytes_per_pixel = 8;

         else
            return;

#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   if 0
#       pragma simd
#   endif
#endif /* VDM auto patch */
         for (i = 0, rp = row; i < row_width; i++, rp += bytes_per_pixel)
         {
            png_uint_32 s0   = (*(rp    ) << 8) | *(rp + 1);
            png_uint_32 s1   = (*(rp + 2) << 8) | *(rp + 3);
            png_uint_32 s2   = (*(rp + 4) << 8) | *(rp + 5);
            png_uint_32 red  = (png_uint_32)((s0 - s1) & 0xffffL);
            png_uint_32 blue = (png_uint_32)((s2 - s1) & 0xffffL);
            *(rp    ) = (png_byte)((red >> 8) & 0xff);
            *(rp + 1) = (png_byte)(red & 0xff);
            *(rp + 4) = (png_byte)((blue >> 8) & 0xff);
            *(rp + 5) = (png_byte)(blue & 0xff);
         }
      }
#endif /* PNG_WRITE_16BIT_SUPPORTED */
   }
}
#endif /* PNG_MNG_FEATURES_SUPPORTED */
#endif /* PNG_WRITE_SUPPORTED */
