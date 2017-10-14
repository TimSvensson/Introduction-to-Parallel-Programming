#include <omp.h>
#include <stdio.h>
#define N 100
#define CHUNKSIZE 10

int main(int argc, char *argv[]) {

  int i, chunk;
  float a[N], b[N], c[N];

  /* Some initializations */
  for (i=0; i < N; i++)
    a[i] = b[i] = i * 1.0;
  chunk = CHUNKSIZE;

#pragma omp parallel shared(a,b,c,chunk) private(i)
  {

#pragma omp for schedule(dynamic,chunk) nowait
    for (i=0; i < N; i++)
      {
	c[i] = a[i] + b[i];
	printf("Thread %i wrote %.2f to index %i\r\n", omp_get_thread_num(), c[i], i);
      }
  }   /* end of parallel region */

  return(0);
}
