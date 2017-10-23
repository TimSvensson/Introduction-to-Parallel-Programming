#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("I am %d of %d\n", rank, size);
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);
	printf("Hello world from processor %s, rank %d" " out of %d processors\n", processor_name, rank, size);
	MPI_Finalize();
	
	return 0; 

}
