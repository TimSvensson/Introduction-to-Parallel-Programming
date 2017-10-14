#include <omp.h>
#include <stdio.h>

#define N 10

main(int argc, char *argv[])  {

  int   i, n, chunk;
  float a[N], b[N], result;

  /* Some initializations */
  n = N;
  chunk = N/3;
  result = 0.0;
  for (i=0; i < n; i++) {
    a[i] = i * 1.0;
    b[i] = i * 2.0;
  }

   #pragma omp parallel for      \
     default(shared) private(i)  \
     schedule(static,chunk)      \
     reduction(+:result)

    for (i=0; i < n; i++)
      {
	result = result + (a[i] * b[i]);
	printf("Thread %i, i %i, result %.0f\r\n", omp_get_thread_num(), i, result);
      }

  printf("Final result= %f\n",result);

}
