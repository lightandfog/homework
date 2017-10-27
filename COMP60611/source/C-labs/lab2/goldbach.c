#include <stdio.h>
#include <math.h>
#include <omp.h>

/*  Serial Goldbach Code  */

int goldbach(int evenno);
int isprime (int num);

void *main()
{
  int i;
  int n=20000;
  int numpairs[n];
  double start, end, tick;

  for (i=1; i < n+1; i++)
     {
     numpairs[i] = 0;
     }
/* Find number of Goldbach pairs for the first n even numbers */

  start = omp_get_wtime();
  for (i=1; i < n+1; i++)
     {
     numpairs[i] = goldbach(2*i);
     }
  end = omp_get_wtime();
  tick = omp_get_wtick();
  printf("Work took %f seconds \n", (end - start));
  printf("tick = %E \n 1/tick = %E \n", tick, 1.0/tick);
  /* Output selected results */

  for (i=1; i < n+1; i=i+n/10)
     {
     printf("%d %d \n", 2*i, numpairs[i]);
     }
}

int goldbach(int evenno)
{
  int j;
  int pairs = 0;
  /* Test all pairs which sum to evenno for primality*/

  for (j = 2; j <= evenno/2; j++)
     {
     if ((isprime(j) == 1) && (isprime(evenno-j) == 1)) pairs++;
     }
  return pairs;
}

int isprime (int num)
{
  int i, prime, upper_limit;
  /* Crude primality test: check all integers less than sqare root of number
     isprime = 1 <=> prime = .true.
     isprime = 0 <=> prime = .false. */
  
  prime = 1;
  i = 2;
  upper_limit = (int) sqrt( (float) num);  
  do {
     if (num%i == 0) prime = 0;
     i++;
     }
  while ( (i <= upper_limit) && prime == 1);
  return prime;
}
