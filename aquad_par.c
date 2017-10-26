#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

struct interval {
  double left;
  double right;
  double epsilon;
};
typedef struct interval Interval; 

void *main(int argc, char *argv[])
{
  double left, right, result, eps;

  double func1(double);
  double func2(double);
  double func3(double);
  double Quad(double (*func)(double), double, double, double);

  double start, stop;

  start = omp_get_wtime();
  left = 0.0;
  right = 1.0;
  eps = 1.0E-7;
  result = Quad(&func1, left, right, eps);
  stop = omp_get_wtime();

  printf("Result for func1 = %g\n", result); 
  printf("Time taken for func1 %f\n",stop-start);

  start = omp_get_wtime();
  left = 0.0;
  right = 1.0;
  eps = 1.0E-7;
  result = Quad(&func2, left, right, eps);
  stop = omp_get_wtime();

  printf("Result for func2 = %g\n", result); 
  printf("Time taken for func2 %f\n",stop-start);

  start = omp_get_wtime();
   left = 0.0;
  right = 1.0;
  eps = 1.0E-7;
  result = Quad(&func3, left, right, eps);
  stop = omp_get_wtime();

  printf("Result for func3 = %g\n", result); 
  printf("Time taken for func3 %f\n",stop-start);
}

double Quad(double (*func)(double), double left, double right, double epsilon)
{
// *** Adaptive Quadrature 
//
  int stacksize=1000;
  int stackpointer = 1;

  double stackops(int, Interval *, double (*func)(double));

  Interval stack[stacksize];

  double result;  


  stack[stackpointer].left = left;
  stack[stackpointer].right = right;
  stack[stackpointer].epsilon = epsilon; 

  result = 0.0;

  result = stackops(stackpointer, stack, func);

  return result;
}


double stackops(int stackpointer, Interval *stack, double (*func)(double)) {
//#include <math.h>

  double result, abserror, l, r, m, est1, est2, eps;
  int stacksize = 1000;
  int sign = 0;
  result = 0.0;
  
  #pragma omp parallel private(l,r,eps,m,est1,est2,abserror) shared(stackpointer,sign,stack,result)
  {
  while (stackpointer >= 1 || sign != 0 ) 
  //while (stackpointer >= 1 )
  {
    // pop next interval off stack
     //printf("1--\n");
     int i = 2;
     int s2 = 0;
     while(i == 2)
     {
         #pragma omp critical
         {
         if(stackpointer >= 1)
         {
             l = stack[stackpointer].left;
             r = stack[stackpointer].right;
             eps = stack[stackpointer].epsilon;
             stackpointer--;
            
             sign ++;
             i --;
         }
         }
         if( i == 2 && stackpointer < 1)
         {
            s2 = 1;
            break;
         }
         //printf("2---\n");
     }
     if(s2 == 1)
     {
        //printf("3--\n");
        continue;
     }
    //#pragma omp critical
    //{
    //l = stack[stackpointer].left;
    //r = stack[stackpointer].right;
    //eps = stack[stackpointer].epsilon;
    //stackpointer--;
    //}
    // compute estimates

    m = 0.5 * (l + r);
    est1 = 0.5 * (r - l) * (func(l) + func(r));
    est2 = 0.5 * ((m - l) * (func(l) + func(m)) + (r - m) * (func(m) + func(r)));
    abserror = fabs(est2 - est1) / 3.0;

    // check for desired accuracy: push both halves onto
    // the stack if not accurate enough
    if (abserror <= eps) 
    {
      #pragma omp critical
      {
      result += est2;
      sign = sign - 1;
      //break;
      }
    } 
    else 
    {
      if (stackpointer+2 > stacksize) {
        printf("Stack too small, track doubling stacksize\n");
        exit(0);
      }
      #pragma omp critical
      {
      stackpointer++;
      stack[stackpointer].left = l;
      stack[stackpointer].right = m;
      stack[stackpointer].epsilon = eps * 0.5;

      stackpointer++;
      stack[stackpointer].left = m;
      stack[stackpointer].right = r;
      stack[stackpointer].epsilon = eps * 0.5;
      sign = sign - 1;
      }
    }
    //printf("sign:%d\n",sign);
  }
  } 
  printf("sign:%d\n",sign);
  return result; 
}

double func1(double x)
{
  double q;
  int n;

  n = 2000;
  q = 1000.0;

  for (int i=0;i<n;i++) {
    q = q - x;
  }
  if (q == 1.0) printf("Avoid compiler optimisation",q);

  return x*x;
}


double func2(double x)
{
  double q;
  int n;

  n = 20000;
  q = 1000.0;

  for (int i=0;i<n;i++) {
    q = q - x;
  }
  if (q == 1.0) printf("Avoid compiler optimisation",q);

  return x*x;
}

double func3(double x)
{
  double q;
  int n;

  n = 200000;
  q = 1000.0;

  for (int i=0;i<n;i++) {
    q = q - x;
  }
  if (q == 1.0) printf("Avoid compiler optimisation",q);

  return x*x;
}
