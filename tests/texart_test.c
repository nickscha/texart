/* texart.h - v0.1 - public domain data structures - nickscha 2025

A C89 standard compliant, single header, nostdlib (no C Standard Library) Text Art Generator (TEXART).

This Test class defines cases to verify that we don't break the excepted behaviours in the future upon changes.

LICENSE

  Placed in the public domain and also MIT licensed.
  See end of file for detailed license information.

*/
#include "../texart.h"    /* Text Art Generator */
#include "../deps/test.h" /* Simple Testing framework    */
#include "../deps/perf.h" /* Simple Performance profiler */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int load_ppm(
    char *filename,
    unsigned char **out_pixels,
    unsigned int *out_width,
    unsigned int *out_height,
    unsigned int *out_stride)
{
  FILE *f;
  char header[3];
  int c;
  unsigned int width, height, maxval;
  size_t pixels_size;
  unsigned char *pixels;
  size_t i;
  int val;

  f = fopen(filename, "rb"); /* open in binary mode for P6 */
  if (!f)
    return -1;

  /* Read header */
  if (fscanf(f, "%2s", header) != 1)
  {
    fclose(f);
    return -2;
  }

  /* Skip comments */
  do
  {
    c = fgetc(f);
    if (c == '#')
    {
      while ((c = fgetc(f)) != '\n' && c != EOF)
      {
        /* skip line */
      }
    }
  } while (c == '#');
  ungetc(c, f);

  if (fscanf(f, "%u %u %u", &width, &height, &maxval) != 3)
  {
    fclose(f);
    return -3;
  }
  if (maxval != 255)
  {
    fclose(f);
    return -4; /* only 8-bit supported */
  }

  /* consume single whitespace after header */
  fgetc(f);

  pixels_size = (size_t)width * height * 3;
  pixels = (unsigned char *)malloc(pixels_size);
  if (!pixels)
  {
    fclose(f);
    return -5;
  }

  if (strcmp(header, "P3") == 0)
  {
    /* ASCII PPM */
    for (i = 0; i < pixels_size; i++)
    {
      if (fscanf(f, "%d", &val) != 1)
      {
        free(pixels);
        fclose(f);
        return -6;
      }
      if (val < 0)
        val = 0;
      if (val > 255)
        val = 255;
      pixels[i] = (unsigned char)val;
    }
  }
  else if (strcmp(header, "P6") == 0)
  {
    /* Binary PPM */
    if (fread(pixels, 1, pixels_size, f) != pixels_size)
    {
      free(pixels);
      fclose(f);
      return -7;
    }
  }
  else
  {
    free(pixels);
    fclose(f);
    return -8; /* unknown format */
  }

  fclose(f);

  *out_pixels = pixels;
  *out_width = width;
  *out_height = height;
  *out_stride = 3; /* RGB */

  return 0;
}

int main(int argc, char *argv[])
{
  unsigned int x, y;

  char *density_map = "@#W$9876543210?!abc;:+=-,._      ";
  unsigned int density_map_size = (unsigned int)strlen(density_map);

  unsigned char *pixels;
  unsigned int pixels_width, pixels_height, pixels_stride;
  char *texart_buffer;
  unsigned int texart_buffer_capacity;
  unsigned int texart_buffer_size = 0;

  if (argc < 2)
  {
    return 1;
  }

  if (load_ppm(argv[1], &pixels, &pixels_width, &pixels_height, &pixels_stride) != 0)
  {
    fprintf(stderr, "Failed to load PPM file.\n");
    return 1;
  }

  texart_buffer_capacity = pixels_width * pixels_height;
  texart_buffer = malloc(texart_buffer_capacity);

  texart(
      pixels, pixels_width, pixels_height, pixels_stride,
      density_map, density_map_size,
      1,
      texart_buffer, texart_buffer_capacity, &texart_buffer_size);

  for (y = 0; y < pixels_height; ++y)
  {
    for (x = 0; x < pixels_width; ++x)
    {
      unsigned int index = (y * pixels_width + x);
      printf("%c", texart_buffer[index]);
    }
    printf("\n");
  }

  return 0;
}

/*
   -----------------------------------------------------------------------------
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
