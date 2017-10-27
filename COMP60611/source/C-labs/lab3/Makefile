CC = gcc
OMP_FLAG = -fopenmp
CFLAGS = -std=c99 -O1 -c ${OMP_FLAG}
LFLAGS = -lm

.SUFFIXES : .o .c

.c.o:
	${CC} ${CFLAGS} -o $@ $*.c

sblas:sblas.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

sblas_par:sblas_par.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

sblas_par2:sblas_par2.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

sblas_parb:sblas_parb.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

sblas_parbbac:sblas_parbbac.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}


sblas_parb2:sblas_parb2.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}
clean:
	rm *.o sblas sblas_par sblas_par2 *.o

