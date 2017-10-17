#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "timer.h"

void merge(int a[], int temp[], int low, int high, int mid) {
  int i, j, k;
  i = low; j = mid+1; k = low;
  while((i<=mid)&&(j<=high)){
    if(a[i]<a[j]){
      temp[k]=a[i]; k++; i++;
    } else {
      temp[k]=a[j]; k++; j++;
    }
  }
  while(i<=mid){
    temp[k]=a[i]; k++; i++;
  }
  while(j <= high){
    temp[k] = a[j]; k++; j++;
  }
  for(i = low; i < k; i++) {
    a[i] = temp[i];
  }
}

void mergesort(int a[], int temp[], int low, int high) {
  int mid;
  if (low < high)
    {
      mid = (low+high)/2;              // find the midpoint

#pragma omp task
      mergesort(a, temp, low, mid);    // sort the first half
#pragma omp task
      mergesort(a, temp, mid+1, high); // sort the second half

#pragma omp taskwait
      merge(a, temp, low, high, mid);  // merge them together into one sorted list
    }
}

int main(int argc, char **argv) {
  int LEN;
  double time;
  
  LEN = 100000000;
  int N = 16;
  // Command line argument: array length
  if (argc > 1 ) N = atoi(argv[1]);  
  int i, *x,*temp;
  x = (int *)malloc(sizeof(int)*LEN);
  temp = (int *)malloc(sizeof(int)*LEN);
  if (x == NULL || temp == NULL) {
    printf("Out of memory");
    exit(0);
  }

  //Fill the array to be sorted with random numbers
  for (i = 0; i < LEN; i++) 
    x[i] = rand() % LEN;
  
#ifdef DEBUG
  printf("before sort:\n");
  for (i = 0; i < LEN; i++) printf("%d ", x[i]);
  printf("\n");
#endif
  
  initialize_timer ();  //We are just timing the sort
  start_timer();
#pragma omp parallel firstprivate(x, temp) num_threads(N)
#pragma omp single
  {
    mergesort(x, temp, 0, (LEN-1));
  }
  stop_timer();
  time = elapsed_time();
  
#ifdef DEBUG
  printf("after sort:\n");
  for (i = 0; i < LEN; i++) printf("%d ", x[i]);
  printf("\n");
#endif
  
  printf("%lf\n", time);
  return 0;
}
