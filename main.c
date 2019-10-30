/*
 * Document Array Construction
 *
 * Author: Felipe A. Louza
 * contact: louza@ufu.br
 * 29/10/2019
 *
 */

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

#include "lib/utils.h"
#include "lib/file.h"
#include "lib/suffix-array.h"
#include "document-array.h"
#include "external/gsacak.h"
#include "external/malloc_count/malloc_count.h"

#ifndef DEBUG
  #define DEBUG 0 
#endif


/*******************************************************************/


void usage(char *name){
  printf("document-array:\n\tUsage: %s [options] FILE \n\n",name);
  puts("Computes the Document-array of FILE");
  puts("Output:\tDocument-array (and Suffix-array)\n");
  puts("Available options:");
//puts("\t-A a\tpreferred algorithm to use (available 1)");
  puts("\t-d D\tuse the first D documents of the INPUT");
  puts("\t-v\tverbose output");
  puts("\t-o\toutput computed arrays to disk (INPUT.da and INPUT.sa)");
  puts("Debug options:");
  puts("\t-c\tcheck output (for debug)");
  puts("\t-p P\tprint the output arrays DA[1,P] and SA[1,P] (for debug)");
  puts("\t-h\tthis help message");
  exit(EXIT_FAILURE);
}

/*******************************************************************/

int main(int argc, char** argv){

  #if STEP_TIME
    time_t t_start=0;
    clock_t c_start=0;
  #endif

  time_t t_total=0;
  clock_t c_total=0;

  extern char *optarg;
  extern int optind, opterr, optopt;

  int c=0, verbose=0, time=0, check=0, print=0, output=0, stats=0;
  //input options
  char *c_file=NULL;

  size_t  d=0; //number of documents
  int ALG=1;//Algorithm

  while ((c=getopt(argc, argv, "vthp:d:A:cosbf")) != -1) {
    switch (c)
    {
      case 'v':
        verbose++; break;
      case 't':
        time++; break;
      case 'p':
        print=(size_t)atoi(optarg); break;
      case 'h':
        usage(argv[0]); break;       // show usage and stop
      case 'd':
        d=(size_t)atoi(optarg); break;
      case 'A':
        ALG=(size_t)atoi(optarg); break;
      case 'c':
        check++; break;
      case 'o':
        output++; break;
      case 's':
        stats++; break;
      case '?':
        exit(EXIT_FAILURE);
    }
  }
  free(optarg);
  
  if(optind+1==argc) {
    c_file=argv[optind++];
  }
  else  usage(argv[0]);

  if(verbose>0) {
    puts("Command line:");
    int i;
    for(i=0;i<argc;i++)
      printf(" %s",argv[i]);
    puts("");
  }

  /********/
  unsigned char *str = NULL;
  size_t i, n=0;

  // reading the input as a collection of documents
  unsigned char **R;

  //disk access
  R = (unsigned char**) file_load_multiple(c_file, &d, &n);
  if(!R){
    fprintf(stderr, "Error: less than %zu strings in %s\n", d, c_file);
    return 0;
  }

  //concatenate strings
  str = cat_char(R, d, &n);

  printf("d = %zu\n", d);
  printf("N = %zu bytes\n", n);
  printf("sizeof(int) = %zu bytes\n", sizeof(int_t));


  #if DEBUG
    printf("R:\n");
    for(i=0; i<d; i++)
      printf("%" PRIdN ") %s (%zu)\n", i, R[i], strlen((char*)R[i]));
  #endif

  //free memory
  for(i=0; i<d; i++)
    free(R[i]);
  free(R);

  /********/
  //suffix array
  int_t *SA = NULL;
  SA = (int_t*) malloc(n*sizeof(int_t));
  for(i=0; i<n; i++) SA[i]=0;

  //document array
  int_da *DA = NULL;
  DA = (int_da*) malloc(n*sizeof(int_da));
  for(i=0; i<n; i++) DA[i]=0;
  /********/

  switch(ALG){

    /****/
    case 1:  
      time_start(&t_total, &c_total);
      #if STEP_TIME
        printf("## gSACA-K ##\n"); 
        time_start(&t_start, &c_start);
      #endif
      gsacak((unsigned char*)str, (uint_t*)SA, NULL, NULL, n);
      #if STEP_TIME
        fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
        printf("## DOCUMENT_ARRAY_9n ##\n"); 
      #endif
      document_array_9n((unsigned char*)str, SA, DA, n, d);
      #if STEP_TIME
        fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
      #endif
      printf("TOTAL:\n");
      fprintf(stderr,"%.6lf\n", time_stop(t_total, c_total));
      break;
    /****/

    default: break;
  }

  // validate  
  if(check){

    if(SA){
      printf("SA: ");
      if(!suffix_array_check((unsigned char*)str, SA, n, sizeof(char), 1)) fprintf(stderr,"isNotSorted!!\n");
      else printf("isSorted!!\n");
    }
    if(DA){
      printf("DA: ");
      if(!document_array_check(str, SA, DA, n, d)) fprintf(stderr, "isNotDA!!\n");
			else printf("isDA!!\n");
    }
  }

  if(print){ 
    printf("########\n");
    printf("i) DA\t");
    if(SA) printf("SA\t");
    printf("suffixes\n");
    for(i=0; i<min(n,print); i++){
      printf("%zu) %" PRIdN "\t", i, DA[i]);
      if(SA) printf("%" PRIdN "\t", SA[i]);
      int_t j;
      for(j=SA[i]; j<(int_t) min(SA[i]+10,n); j++){
        char c = (str[j]==0)?'#':(str[j]>1?str[j]-1:'$');
        printf("%c", c);
      }
      if(j==SA[i]+10) printf("...");
      printf("\n");
    }
    printf("########\n");
  }

  if(output){
    char c_out[255];
    FILE *f_out = NULL;
    printf("OUTPUT:\n");
    if(DA){
      sprintf(c_out, "%s.da", c_file);
      printf("%s\n", c_out);
      f_out = file_open(c_out, "wb");
      fwrite(DA, sizeof(int_t), n, f_out);
      file_close(f_out);
    }
    if(SA){
      sprintf(c_out, "%s.sa", c_file);
      printf("%s\n", c_out);
      f_out = file_open(c_out, "wb");
      fwrite(SA, sizeof(int_t), n, f_out);
      file_close(f_out);
    }
    {
      sprintf(c_out, "%s.bin", c_file);
      printf("%s\n", c_out);
      f_out = file_open(c_out, "wb");
      fwrite(str, sizeof(unsigned char), n, f_out);
      file_close(f_out);
    }
  }

  free(str);
  if(SA) free(SA);
  if(DA) free(DA);

return 0;
}

