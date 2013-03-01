
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deflate.h"
#include "gzip_container.h"
#include "zlib_container.h"

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
                  const char* outfilename);
