#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include "utils.h"



/*******************************************************************/
int file_chdir(char* dir);

FILE* file_open(char *c_file, const char * c_mode);
int file_close(FILE* f_in);

size_t file_size(FILE* f_in);

char* file_load(FILE* f_in) ;
char** file_load_multiple(char* c_file, size_t *k, size_t *n) ;

int file_text_write(unsigned char *str, size_t n, char* c_file, const char* ext);
int file_text_int_write(int_t *str_int, size_t n, char* c_file, const char* ext);

int_t file_text_read(unsigned char** str, char* c_file, const char* ext);
int_t file_text_int_read(int_t** str, char* c_file, const char* ext);

void mkdir(const char* c_file);

/*******************************************************************/



#endif
