/*
Copyright 2011 Google Inc. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Author: lode.vandevenne@gmail.com (Lode Vandevenne)
Author: jyrki.alakuijala@gmail.com (Jyrki Alakuijala)
*/

/*
Zopfli compressor program. It can output gzip-, zlib- or deflate-compatible
data. By default it creates a .gz file. This tool can only compress, not
decompress. Decompression can be done by any standard gzip, zlib or deflate
decompressor.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deflate.h"
#include "gzip_container.h"
#include "zlib_container.h"

/*
Loads a file into a memory array.
*/
static void LoadFile(const char* filename,
                     unsigned char** out, size_t* outsize) {
  FILE* file;

  *out = 0;
  *outsize = 0;
  file = fopen(filename, "rb");
  if (!file) return;

  fseek(file , 0 , SEEK_END);
  *outsize = ftell(file);
  rewind(file);

  *out = (unsigned char*)malloc(*outsize);

  if (*outsize && (*out)) {
    size_t testsize = fread(*out, 1, *outsize, file);
    if (testsize != *outsize) {
      /* It could be a directory */
      free(*out);
      *out = 0;
      *outsize = 0;
    }
  }

  assert(!(*outsize) || out);  /* If size is not zero, out must be allocated. */
  fclose(file);
}

/*
Saves a file from a memory array, overwriting the file if it existed.
*/
static void SaveFile(const char* filename,
                     const unsigned char* in, size_t insize) {
  FILE* file = fopen(filename, "wb" );
  assert(file);
  fwrite((char*)in, 1, insize, file);
  fclose(file);
}

typedef enum {
  OUTPUT_GZIP,
  OUTPUT_ZLIB,
  OUTPUT_DEFLATE
} OutputType;

/*
outfilename: filename to write output to, or 0 to write to stdout instead
*/
void CompressFile(const Options* options,
                  OutputType output_type,
                  const char* infilename,
                  const char* outfilename) {
  unsigned char* in;
  size_t insize;
  unsigned char* out = 0;
  size_t outsize = 0;
  LoadFile(infilename, &in, &insize);
  if (insize == 0) {
    fprintf(stderr, "Invalid filename: %s\n", infilename);
    return;
  }
  if (output_type == OUTPUT_GZIP) {
    GzipCompress(options, in, insize, &out, &outsize);
  } else if (output_type == OUTPUT_ZLIB) {
    ZlibCompress(options, in, insize, &out, &outsize);
  } else if (output_type == OUTPUT_DEFLATE) {
    unsigned char bp = 0;
    Deflate(options, 2 /* Dynamic block */, 1, in, insize, &bp, &out, &outsize);
  } else {
    assert(0);
  }
  if (outfilename) {
    SaveFile(outfilename, out, outsize);
  } else {
    size_t i;
    for (i = 0; i < outsize; i++) {
      /* Works only if terminal does not convert newlines. */
      printf("%c", out[i]);
    }
  }

  free(out);
  free(in);
}
