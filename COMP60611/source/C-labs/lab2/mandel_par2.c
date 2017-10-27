#include <stdio.h>
#include <complex.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

double halfpi_series();
int monte_numinside(int nlow, int nhigh, double complex c[]);

//  Serial Mandel Code
void *main() {
  double r1, r2, halfpi, pi, area, err;
  double start, end, tot_start, tot_end;
  int i,numinside;
  int npoints = 300000; // number of random points
  int nlow, nhigh;
  double complex c[npoints];

  // GENERATE RANDOM NUMBERS

  srand(54321);
  for (i=0; i < npoints; i++) 
     {
       r1 = (double) rand()/RAND_MAX;
       r2 = (double) rand()/RAND_MAX;
       c[i]=(-2.0+2.5*r1)+(1.125*r2)*I;
      }

  // Total timer
  tot_start = omp_get_wtime();

  // CALCULATE PI/2 FROM GREGORY'S FORMULA

  start = omp_get_wtime();
  halfpi = halfpi_series();  
  end = omp_get_wtime();
  printf("halfpi_series time = %f seconds \n", end-start);


  //  CALCULATE AREA OF MANDELBROT SET BY MONTE CARLO SAMPLING

  start = omp_get_wtime();
  nlow = 0;
  nhigh = npoints-1;
  numinside = monte_numinside(nlow, nhigh, c);
  end = omp_get_wtime();
  printf("monte time = %f seconds \n", end-start);

  tot_end = omp_get_wtime();
  printf("Total time = %f seconds \n", tot_end - tot_start);

  // OUTPUT RESULTS
  area = 2.0*2.5*1.125 * ((double) numinside/(double) (npoints));
  err = area/sqrt((double) (nhigh - nlow + 1));
  pi = 2.0*halfpi;
  printf("Pi/2 = %f \n",halfpi);
  printf("Pi = %f \n", pi);
  printf("Area of Mandelbrot set = %f +/- %f \n",area, err);
}

double halfpi_series() {
  double sum, sign, denom;
  int i;
  int nterms = 100000000;

  sum = 0.0;
  sign = 1.0;
  denom = 1.0;
  for (i = 0; i < nterms; i++) {
    sum = sum + sign/denom;
    sign = -sign;
    denom = denom + 2.0;
  }
  return 2.0 * sum;
}
 
int monte_numinside(int nlow, int nhigh, double complex c[])
{
  int i, j, n_inside;
  int maxiter = 10000;
  double complex z;
  double z_abs;
  n_inside = 0;
  
#pragma omp parallel for		       \
  private (i, j, z, z_abs) shared (c, maxiter) \
  reduction (+:n_inside) schedule (static)
  for (i = nlow; i < nhigh; i++) {      
    z = 0.0 + 0.0*I;
    j=0;
    do {
      z = z*z + c[i];
      z_abs = cabs(z);
      j++;
    }
    while (j < (maxiter+1) && z_abs <= 2.0);
    if (j = maxiter+1  && z_abs <= 2.0) n_inside = n_inside + 1;
  }
  return n_inside;
}

