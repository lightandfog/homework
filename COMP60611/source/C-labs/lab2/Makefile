CC = gcc
OMP_FLAG = -fopenmp
CFLAGS = -std=c99 -O1 -c ${OMP_FLAG}
LFLAGS = -lm

.SUFFIXES : .o .c

.c.o:
	${CC} ${CFLAGS} -o $@ $*.c

mandel:mandel.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

mandel_par:mandel_par.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

mandel_par2:mandel_par2.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

goldbach:goldbach.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

goldbach_par:goldbach_par.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

goldbach_par2:goldbach_par2.o
	${CC} ${OMP_FLAG} -o $@ $@.o ${LFLAGS}

clean:
	rm *.o mandel mandel_par mandel_par2 goldbach goldbach_par

