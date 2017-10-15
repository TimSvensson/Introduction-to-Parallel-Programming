#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void work(int value)
{
  printf("Thread %d, value %d\r\n", omp_get_thread_num(), value);
}

void printIntArr(int* arr, int size)
{
  printf("{ ");
  for (int i = 0; i < size; i++)
    printf("%d, ", arr[i]);
  printf("}\r\n");
}

int main(int argc, char* argv[])
{

  int N = 20;
  int T = 4;
  int* crnt;
  int* prev;
  int* swap;

  crnt = calloc(sizeof(int), N);
  prev = calloc(sizeof(int), N);

#pragma omp parallel
  for (int t = 0; t < T; t++)
    {
      #pragma omp for
      for (int i = 0; i < N; i++)
	{
	  crnt[i] = t*i + prev[i];
	  work(crnt[i]);
	}

      #pragma omp single
      {
      printf("prev ");
      printIntArr(prev, N);
      
      printf("crnt ");
      printIntArr(crnt, N);
      
      swap = crnt;
      crnt = prev;
      prev = swap;
      }
      
    }

  free(crnt);
  free(prev);
}

