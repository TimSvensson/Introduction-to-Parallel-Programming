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


int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc == 1) {
        cout << "Usage: ./main 'number'.\n";
    }else {
        
        int threadCount = thread::hardware_concurrency();
        //int chunks = threadCount/2;
        int max = stoi(argv[1]);
        int squareOfMax = ceil(sqrt(max)) + 1;
        
        
        vector<int> listUnfiltered(max), listRemaining(max - squareOfMax);
        iota(listUnfiltered.begin(), listUnfiltered.end(), 1);
        
        // Loops through a list of integers and sets any non-prime to zero
        for (int k = 2; k*k <= squareOfMax; k++) {
            for (int i = k*k; i <= squareOfMax; i++) {
                if (listUnfiltered[i - 1] % k == 0  )  {
                    listUnfiltered[i - 1 ] = 0;
                }
            }
            cout << "Finished checking for multiples of " << k << "\n";
        }
        
        // Filter out zeroes from above list, use this list to compare the other lists
        vector<int> primeMultiples = filterList(listUnfiltered, squareOfMax);
        printList(primeMultiples, "Filtered list: ");
        
        // Fill a list of remaining ints starting from +1 of the filtered list's last number
        iota(listRemaining.begin(), listRemaining.end(), (primeMultiples.back() + 1));
        
        // Checks the remaining list for multiples of the filtered primes and sets these to zero
        for (int i = 0; i < listRemaining.size(); i++) {
            for (int y = 0; y < primeMultiples.size(); y++) {
                if (listRemaining[i] % primeMultiples[y] == 0) {
                    listRemaining[i] = 0;
                }
            }
        }
        
        
        divideList(max, squareOfMax, threadCount, primeMultiples);
        
        
    }
    
    return 0;
}
