CC = gcc
CFLAGS += -Wall 
#CFLAGS += -g -O0
CFLAGS += -D_FILE_OFFSET_BITS=64 -m64 -O3 -fomit-frame-pointer -Wno-char-subscripts 

LFLAGS = -lm -lrt -ldl

INPUT = dataset/input.txt
ALG = 1

LIBOBJ = \
	lib/utils.o\
	lib/file.o\
	lib/suffix-array.o\
	external/gsacak.o\
	external/malloc_count/malloc_count.o\
	document-array.o
	
##
M64 = 0
DEBUG = 0
STEP_TIME = 0
##

DEFINES = -DDEBUG=$(DEBUG) -DM64=$(M64) -DSTEP_TIME=$(STEP_TIME) 

CFLAGS += $(DEFINES)

all: main 

clean:
	\rm -f *.o ../*.o ../external/*.o lib/*o main 

main: main.c ${LIBOBJ} 
	$(CC) -o main main.c ${LIBOBJ} $(CFLAGS) $(LFLAGS) 

run:
	./main $(INPUT) -A $(ALG)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main $(INPUT) -A $(ALG)
