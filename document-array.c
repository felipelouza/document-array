#include "document-array.h"

#define chr(i) (cs==sizeof(int_text)?((int_text*)T)[i]:((unsigned char *)T)[i])

/*******************************************************************/
int document_array_LF(unsigned char* T, int_t* SA, int_da* DA, uint_t n, unsigned int SIGMA, int cs, unsigned char separator, uint_t d){

	uint_t  i, tmp;

	int_t *A = SA;
	int_da *B = DA;

	int_t* C = (int_t*) malloc(SIGMA*sizeof(int_t));
  if(C==NULL) return -1;
	for(i=0; i<SIGMA;i++) C[i]=0;

	//compute BWT in A[1,n]
	for (i=0; i<n; i++){
		A[i]=(A[i]>0)?T[A[i]-1]:0;	
		C[A[i]]++;
	}

	//compute Counter-array
	int_t sum=0;
	for(i=0; i<SIGMA;i++){
		sum += C[i];
		C[i]=sum-C[i];
	}

	//compute LF in A[1,n]
	for(i=0; i<n;i++){
		A[i] = C[A[i]]++;
	}

	uint_t	pos = 0;
	int_t	doc = d;

	for (i=n-1; i>0; i--){

		tmp = A[pos]; //tmp = LF[i]
		A[pos]=i; //SA[pos] = i
		B[pos]=doc;//DA[pos] = doc

		if(tmp<=d){
			tmp=doc;
			doc--;
		}

		pos=tmp;
	}

	A[pos]=0;
	B[pos]=0;

  free(C);

return 0;
}

int document_array_9n(unsigned char* T, int_t* SA, int_da* DA, uint_t n, uint_t d){

  return document_array_LF(T, SA, DA, n, 256, sizeof(char), 1, d);//separator=1
}

/*******************************************************************/

int document_array_check_13n(unsigned char *T, int_t *SA, int_da *DA, uint_t n, int cs, unsigned char separator, uint_t d){

	uint_t i;
	int_da count=d;
	int_da* R = (int_da*) malloc(n*sizeof(int_da));

	for(i=n-1; i>0;i--){
		if(chr(i)==separator) count--;
		R[i]=count;
	}
	R[0]=0;

	for(i=0; i<n;i++){
		if(DA[i]!=R[SA[i]]){
			free(R);
			return 0;
		}
	}

	free(R);	
	
return 1;
}

int document_array_check(unsigned char *T, int_t *SA, int_da *DA, uint_t n, uint_t d){

  return document_array_check_13n(T, SA, DA, n, sizeof(char), 1, d);
}

/*******************************************************************/

int document_array_write(int_da *DA, int_t n, char* c_file, const char* ext){

        FILE *f_out;
        char *c_out = malloc((strlen(c_file)+strlen(ext)+3)*sizeof(char));
        
        sprintf(c_out, "%s.%s", c_file, ext);
        f_out = file_open(c_out, "wb");
        
				int_t i;
				for(i=0; i<n; i++){//writes DA 
	        fwrite(&DA[i], sizeof(int_da), 1, f_out);
				}

        file_close(f_out);
        free(c_out);

return 1;
}


/*******************************************************************/

int_t document_array_read(int_da** DA, char* c_file, const char* ext){

        FILE *f_in;
        char *c_in = malloc((strlen(c_file)+strlen(ext))*sizeof(char));

        sprintf(c_in, "%s.%s", c_file, ext);
        f_in = file_open(c_in, "rb");

	fseek(f_in, 0L, SEEK_END);
	size_t size = ftell(f_in);
	rewind(f_in);
	
	int_t n = size/sizeof(int_da);

        *DA = (int_da*) malloc(n*sizeof(int_da));
        fread(*DA, sizeof(int_da), n, f_in);

        file_close(f_in);
        free(c_in);

return n;
}

/*******************************************************************/
