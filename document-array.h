#ifndef DAH
#define DAH

#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "lib/utils.h"
#include "lib/file.h"

#ifndef DEBUG
        #define DEBUG 0
#endif

/**/

int document_array_9n(unsigned char* T, int_t* SA, int_da* DA, uint_t n, uint_t d);

/**/

int document_array_check(unsigned char *T, int_t *SA, int_da *DA, uint_t n, uint_t d);

int document_array_write(int_da *DA, int_t n, char* c_file, const char* ext);

int_t document_array_read(int_da** DA, char* c_file, const char* ext);

/**/

#endif
