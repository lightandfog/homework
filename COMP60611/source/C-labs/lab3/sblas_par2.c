#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void vector_update(double a[], double b[], double x[], int limit);
void matrix_update(double **l, double **y, int limit);
void vector_add(double a[], double b[], double x[], int limit);
double vec_sum(double x[], int limit); 
void trimv(double **l, double **y, double a[], int limit); 

/* SBLAS code */

void *main() {

  int n = 1024*60;
  int m = 512;
  int nsteps = 1000;
  int k, i;

  //double ll[m][m], **l;
  double **l;
  double yy[m][8], **y;
  double a[n], b[n], x[n];
  
  double res;
  double vector_start, sum_start, trimv_start, vec_update_start, matrix_update_start, total_start;

  double vec_update_time = 0.0;
  double vector_time = 0.0;
  double sum_time = 0.0;
  double matrix_update_time = 0.0;
  double trimv_time = 0.0;
  double total_time = 0.0;
  
  // allocate (lower triangular) matrix as a contibuous block and set up pointers to allow l[][] access 
  l = (double **) malloc((unsigned) m*sizeof(double*));  
  for (i=0; i <= m-1; i++) {
    l[i] = (double *)malloc((unsigned) m * sizeof(double));
    //l[i] = ll[i];
  }
  // same for y in this parallel version!
  y = (double **) malloc((unsigned) m*sizeof(double*));  
  for (i=0; i <= m-1; i++) {
    y[i] = yy[i];
  }

  // Loop over whole program to get reasonable execution time
  // (simulates a time-steping code)

  total_start = omp_get_wtime();

  for (k=1; k<=nsteps; k++ )  {
    vec_update_start = omp_get_wtime();
    vector_update(a, b, x, n);
    vec_update_time = vec_update_time + (omp_get_wtime() - vec_update_start);

    // vector addition operation

    vector_start = omp_get_wtime();
    vector_add(a, b, x, n);
    vector_time = vector_time + (omp_get_wtime() - vector_start);
      
    // sum operation

    sum_start = omp_get_wtime();
    res = vec_sum(x, n);
    sum_time = sum_time +(omp_get_wtime() - sum_start);

    // 'Update' lower triangular matrix and y vector

    matrix_update_start = omp_get_wtime();
    matrix_update(l, y, m);
    matrix_update_time = matrix_update_time +(omp_get_wtime() - matrix_update_start);

    // triangular matrix times vector operation

    trimv_start = omp_get_wtime();
    trimv(l, y, a, m);
    trimv_time = trimv_time + (omp_get_wtime() - trimv_start);

    // end time-step loop
  }

  total_time = omp_get_wtime() - total_start;
    
  printf( "vector update time = %f seconds \n \n", vec_update_time);
  printf( "x[0] = %f \n", x[0]);
  printf( "x[n-1] = %f \n", x[n-1]);
  printf( "vector add time = %f seconds \n \n", vector_time);
  printf( "Sum of x() = %f \n", res);
  printf( "sum time = %f \n \n", sum_time);
  printf( "matrix initialisation time = %f seconds \n \n", matrix_update_time);
  printf( "y[0][0] = %f \n", y[0][0]);
  printf( "y[m-1][0] = %f \n", y[m-1][0]);
  printf( "trimv time = %f seconds \n \n", trimv_time);
  printf( "Total time = %f seconds \n \n", total_time);
}

void vector_update(double a[], double b[], double x[] ,int limit) {
  int i;
#pragma omp parallel for			\
  schedule (static)
  for (i=0; i<limit; i++ )  {
    x[i] = 0.0;
    a[i] = 3.142;
    b[i] = 3.142;
  }
}

void matrix_update(double **l, double **y, int limit) {
  int i, j;
#pragma omp parallel for			\
  schedule (static,1)
  for (i=0; i<limit; i++ ) {
    y[i][0] = 0.0;
    for (j=0; j<=i; j++ ) { 
      l[i][j] = 2.0;
    }
  }
}

void vector_add(double a[], double b[], double x[], int limit) {
  int i;
#pragma omp parallel for		\
  schedule (static)
  for (i=0; i<limit; i++ ) {
    x[i] = a[i] + b[i];
  }
}

double vec_sum(double x[], int limit) {
  int i;
  double sum = 0.0;
#pragma omp parallel for			\
  reduction(+:sum) schedule (static)
  for (i=0; i<limit; i++ ) {
    sum = sum + x[i];
  }
  return sum;
}

void trimv(double **l, double **y, double a[], int limit) {
  int i, j;
#pragma omp parallel for			\
  schedule (static,1)
  for (i=0; i<limit; i++) {
    for (j=0; j<=i; j++) { 
      y[i][0] = y[i][0] + l[i][j]*a[j];
    }
  }
}

