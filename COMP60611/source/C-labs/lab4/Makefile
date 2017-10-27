CC = gcc
OMP_FLAG = -fopenmp
CFLAGS = -std=c99 -O1 -c ${OMP_FLAG}
LFLAGS = -lm

.SUFFIXES : .o .c

.c.o:
	${CC} ${CFLAGS} -o $@ $*.c

aquad: aquad.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

aquad_par: aquad_par.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}
clean:
	rm *.o aquad aquad_par 
