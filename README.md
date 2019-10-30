# document-array

This code implements an algorithm \[1\] that computes the **document-array** (DA) for a string collection. 

## Introduction

Our algorithm receives the suffix array (SA) of the concatenated string \[2\] and reuses its space to store the LF-array. Then SA is reconstructed during the computation of DA.

## Build requirements

An ANSI C Compiler (e.g. GNU GCC)

## API

```c
/** @brief computes the document array of string s[0, n-1] given its SA
 *
 *  @param T  input concatenated string, using separators s[i]=1 and with s[n-1]=0
 *  @param SA Suffix array 
 *  @param DA Document array
 *  @param n  string length
 *  @param d  number of documents
 *  @return -1 if an error occured, otherwise returns 0
 */
int document_array_9n(unsigned char* T, int_t* SA, int_da* DA, uint_t n, uint_t d);

```

## Example

**Compilation:**

```sh
make
```

**Available options:**

```sh
-d D	use the first D documents of the INPUT
-v	verbose output
-o	output computed arrays to disk (INPUT.da and INPUT.sa)
-c	check output (for debug)
-p P	print the output arrays LA[1,P] and SA[1,P] (for debug)
-h	this help message
```

_Notes:_
- Supported extensions are _.txt_, _.fasta_ and _.fastq_.

**Run a test:**

```c
/main dataset/input.txt -p 10 -c
```

**Output:**

```c
d = 3
N = 32 bytes
sizeof(int) = 4 bytes
TOTAL:
CLOCK = 0.000059 TIME = 0.000000
0.000059
SA: isSorted!!
DA: isDA!!
########
i) DA	SA	suffixes
0) 3	31	#
1) 0	6	$abacabad$...
2) 1	15	$banaanana...
3) 2	30	$#
4) 0	5	a$abacabad...
5) 2	29	a$#
6) 2	24	aanana$#
7) 2	19	aananaanan...
8) 1	7	abacabad$b...
9) 1	11	abad$banaa...
########
malloc_count ### exiting, total: 25,032, peak: 21,512, current: 1,024
```

## References

\[1\] Louza, F. A., A Simple Algorithm for Computing the Document Array. CoRR abs/1812.09094 (2018)

\[2\] Louza, F. A., Gog, S., Telles, G. P., Inducing enhanced suffix arrays for string collections. Theor. Comput. Sci., vol. 678, pp. 22-39, 2017, [Elsevier](http://www.sciencedirect.com/science/article/pii/S0304397517302621).

## Thanks

Thanks to Giovanni Manzini, Travis Gagie and Nicola Prezza for helpful discussions

