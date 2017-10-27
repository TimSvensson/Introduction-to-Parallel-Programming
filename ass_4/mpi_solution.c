#include <stdio.h>
#include <mpi.h>

int rank, world;

int work()
{
    return(rank);
}

int main(int argc, char** argv)
{
    MPI_Init(NULL, NULL);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    printf("I am %d in world of size %d\n", rank, world);

    // Do work
    printf("Doing work\n");
    int work_result = work();
    int result[world];
    
    // Collect work
    if (rank == 0)
    {
	result[0] = work_result;

	printf("MPI_Recv\n");
	for (int i = 1; i < world; i++)
	{
	    MPI_Recv(&result[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	// Present work
	for (int i = 0; i < world; i++)
	{
	    printf("Rank %d, result %d\n", i, result[i]);
	}
    }
    else
    {
	printf("rank %d, MPI_Send\n", rank);
	MPI_Send(&work_result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
