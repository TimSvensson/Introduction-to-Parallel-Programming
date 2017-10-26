#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int rank;
  int size;

  MPI_Init(&argc,
	   &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char name[BUFSIZ];
  int length;
  MPI_Get_processor_name(name, &length);

  //printf("I am %d of %d\n", rank, size);
  printf("I am %d of %d (%s)\n", rank, size, name);

  MPI_Finalize();

  return 0;
}
