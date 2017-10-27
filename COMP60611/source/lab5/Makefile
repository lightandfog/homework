MPIDIR = /usr/lib64/openmpi-1.10/bin
#MPIDIR = /usr/local/bin
#MPIDIR = /usr/local/software/mpich2-install/bin
F90 = $(MPIDIR)/mpif90
OMP_FLAG = -fopenmp
FFLAGS = -O ${OMP_FLAG}
INCS = -I/local/COMP60611/utils/include/ia64/gfortran
LFLAGS = -L/local/COMP60611/utils/lib/ia64/gfortran -ltimers

.SUFFIXES : .o .f90

sblas_1: sblas_1.f90
	$(F90) $(FFLAGS) -o $@ $@.f90 ${INCS} ${LFLAGS}

sblas_2: sblas_2.f90
	$(F90) $(FFLAGS) -o $@ $@.f90 ${INCS} ${LFLAGS}

clean:	
	rm -f sblas_1 sblas_2
