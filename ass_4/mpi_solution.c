#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#ifdef TIME
clock_t time_start, time_stop;
double cpu_time_used;
#endif

int rank, size;

bool is_prime(int number, int* prime_multiples, int arr_len)
{
	//printf("num=%d arr_len=%d\n", number, arr_len);
	for (int i = 0; i < arr_len; i++)
	{
		if (number % prime_multiples[i] == 0) return(false);
	}
	return(true);
}

void find_primes(int start, int stop, int* p_arr, int* p_arr_len, int* p_mul, int* p_mul_len )
{
	for (int number = start; number <= stop; number++)
	{
		if (is_prime(number, p_mul, *p_mul_len))
		{
			//if (rank == 0) printf("%d is a prime\n", number);
			p_arr[*p_arr_len] = number;
			(*p_arr_len)++;
		}
	}
}

void print_prime_array(int* p_arr, int p_len)
{
	printf("primes={");
	for (int i = 0; i < p_len; i++)
	{
		/*if (p_arr[i] != 0)*/ printf("%d,", p_arr[i]);
	}
	printf("}\n");
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Usage: %s num_prime\n", argv[0]);
		return(1);
	}
	
	int M = atoi(argv[1]);
	if (M == 0)
	{
		printf("num_primes must be a number > 0\n");
		return(1);
	}
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int sqrt_M = (int) sqrt((double) M); // NOT AN INDEX BUT THE ACTUAL VALUE
	
	int length = M - sqrt_M;
	int steps = length / size;
	if (steps < 1) steps = 1;
	
	int start = rank * steps + sqrt_M;
	int stop = start + steps - 1;
	if (stop >= M) stop = M - 1;
	if (rank == size-1)
	{
		stop = M - 1;
		steps = stop - start;
	}
	
#ifdef DEBUG
	char name[BUFSIZ];
	int len;
	MPI_Get_processor_name(name, &len);
	printf("rank=%d proc=%s length=%d steps=%d start=%d stop=%d\n",
	       rank, name, length, steps, start, stop);
	MPI_Barrier(MPI_COMM_WORLD);
#endif

	// FIND FIRST SQRT(M) PRIMES
	int* prime_multiples = (int*) calloc(sizeof(int), sqrt_M);
	int prime_multiples_length = 0;
	find_primes(2, sqrt_M, prime_multiples, &prime_multiples_length,
		    prime_multiples, &prime_multiples_length);

#ifdef DEBUG
	if (rank == 0) print_prime_array(prime_multiples, prime_multiples_length);
	MPI_Barrier(MPI_COMM_WORLD);
#endif
	
#ifdef TIME
	MPI_Barrier(MPI_COMM_WORLD);
	time_start = clock();
#endif
	
	// FIND ALL PRIMES
	int* pp_arr = (int*) calloc(sizeof(int), steps);
	for (int i = 0; i < steps; i++) pp_arr[i] = 0;
	int pp_len = 0;
	find_primes(start, stop, pp_arr, &pp_len, prime_multiples, &prime_multiples_length);
	
#ifdef DEBUG
	print_prime_array(pp_arr, pp_len);
	MPI_Barrier(MPI_COMM_WORLD);
#endif
	
	// GATHER RESULTS
	int* result;
	int r_index = 0;
	if (rank == 0)
	{
		result = (int*) malloc(length * sizeof(int));
		for (int i = 0; i < length; i++) result[i] = 0;
	}
	
	if (rank == 0)
	{
		for (int i = 0; i < pp_len; i++) result[i] = pp_arr[i];
		r_index = pp_len;
		
		for (int i = 1; i < size; i++)
		{
			int tmp_len;
			MPI_Recv(&tmp_len, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			int tmp[steps];
			MPI_Recv(tmp, tmp_len, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

#ifdef DEBUG
			printf("rank=%d i=%d tmp_len=%d\n", rank, i, tmp_len);
			print_prime_array(tmp, tmp_len);
#endif

			for (int j = 0; j < tmp_len; j++)
			{
				result[r_index] = tmp[j];
				r_index++;
			}
		}
	}
	else
	{

#ifdef DEBUG
		printf("rank=%d pp_len=%d\n", rank, pp_len);
#endif

		MPI_Send(&pp_len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(pp_arr, pp_len, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
#ifdef TIME
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank==0)
	{
		time_stop = clock();
		cpu_time_used = ((double) (time_stop - time_start)) / CLOCKS_PER_SEC;
		printf("%f\n", cpu_time_used);
	}
#endif

	MPI_Finalize();

#ifdef DEBUG
	if (rank == 0) 
	{
		printf("Final print:\n");
		print_prime_array(result, r_index);
		
		printf("Sanity check.\n");
		for (int i = 0; i < r_index; i++)
		{
			if (is_prime(result[i], prime_multiples, prime_multiples_length) == false)
			{
				printf("FUCKING SHIT! %d is not a prime, index %d\n", result[i], i);
			}
		}
	}
#endif
	if (rank == 0) free(result);
	free(prime_multiples);
	free(pp_arr);
	return(0);
}


/*int main(int argc, const char * argv[]) {
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
*/
