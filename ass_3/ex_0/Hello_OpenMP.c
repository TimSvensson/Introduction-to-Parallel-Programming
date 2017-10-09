#include <omp.h>
#include <stdio.h>

int main(void) {
	omp_set_num_threads(42);
	int tid = 0;

	#pragma omp parallel private(tid)
	{
		tid = omp_get_thread_num();
		if(tid % 3 == 0)
			printf("Hello, OpenMP World! from thread %d\n", tid);
	}
	printf("tid = %d at the end of the program\n", tid);
	return 0;
}
