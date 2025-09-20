/* texart.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Text Art Generator (TEXART).

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#ifndef TEXART_H
#define TEXART_H

/* #############################################################################
 * # COMPILER SETTINGS
 * #############################################################################
 */
/* Check if using C99 or later (inline is supported) */
#if __STDC_VERSION__ >= 199901L
#define TEXART_INLINE inline
#elif defined(__GNUC__) || defined(__clang__)
#define TEXART_INLINE __inline__
#elif defined(_MSC_VER)
#define TEXART_INLINE __inline
#else
#define TEXART_INLINE
#endif

#define TEXART_API static

#include "stdio.h"

/* Linear mapping */
TEXART_API TEXART_INLINE int texart_map(int value, int in_min, int in_max, int out_min, int out_max)
{
  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

TEXART_API TEXART_INLINE void texart(
    unsigned char *pixels,
    unsigned int pixels_width,
    unsigned int pixels_height,
    unsigned int pixels_stride,
    char *density_map,
    unsigned int density_map_size,
    unsigned int density_reverse,
    char *texart_out,
    unsigned int texart_out_capacity,
    unsigned int *textart_out_size)
{
  unsigned int x, y;

  if (!pixels || !density_map || !texart_out || texart_out_capacity < pixels_width * pixels_height)
  {
    return;
  }

  *textart_out_size = 0;

  for (y = 0; y < pixels_height; ++y)
  {
    for (x = 0; x < pixels_width; ++x)
    {
      unsigned int index = (y * pixels_width + x) * pixels_stride;

      unsigned char r = pixels[index + 0];
      unsigned char g = pixels[index + 1];
      unsigned char b = pixels[index + 2];
      int avg = (int)((unsigned int)(r + g + b) / 3);
      int out_min = 0;
      int out_max = (int)density_map_size - 1;

      if (density_reverse)
      {
        out_min = out_max;
        out_max = 0;
      }

      texart_out[(*textart_out_size)++] = density_map[texart_map(avg, 0, 255, out_min, out_max)];
    }
  }
}

#endif /* TEXART_H */

/*
   ------------------------------------------------------------------------------
   This software is available under 2 licenses -- choose whichever you prefer.
   ------------------------------------------------------------------------------
   ALTERNATIVE A - MIT License
   Copyright (c) 2025 nickscha
   Permission is hereby granted, free of charge, to any person obtaining a copy of
   this software and associated documentation files (the "Software"), to deal in
   the Software without restriction, including without limitation the rights to
   use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is furnished to do
   so, subject to the following conditions:
   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
   ------------------------------------------------------------------------------
   ALTERNATIVE B - Public Domain (www.unlicense.org)
   This is free and unencumbered software released into the public domain.
   Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
   software, either in source code form or as a compiled binary, for any purpose,
   commercial or non-commercial, and by any means.
   In jurisdictions that recognize copyright laws, the author or authors of this
   software dedicate any and all copyright interest in the software to the public
   domain. We make this dedication for the benefit of the public at large and to
   the detriment of our heirs and successors. We intend this dedication to be an
   overt act of relinquishment in perpetuity of all present and future rights to
   this software under copyright law.
   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
   WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
   ------------------------------------------------------------------------------
*/
