#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

void print_integrate_info(int num_threads, int num_trapezes, double b_upper, double b_lower);
void print_wrong_arguments(char* file_name);

void* integrate(void *args);
double trapezes(double base, double h1, double h2);
double y(double x);
void fill_edges(double *edges, double bound_upper, double bound_lower, int number_trapezes);

pthread_mutex_t print_lock;

typedef struct thread_container {
  double integration_result;
  double* trapezes;
  int trapezes_start_index;
  int trapezes_number_to_integrate;
  double integration_step_size;
} tc_s;

tc_s* init_thread_container(double* ts, int tsi, int tnti, double iss)
{
  tc_s* tc = (tc_s*) malloc(sizeof(tc_s));

  tc->integration_result = 0;
  tc->trapezes = ts;
  tc->trapezes_start_index = tsi;
  tc->trapezes_number_to_integrate = tnti;
  tc->integration_step_size = iss;

  return(tc);
}

void destroy_thread_container(tc_s* tc)
{
  free(tc);
}

//
// main
//
int main(int argc, char* argv[])
{
  if ( argc != 3 && argc != 2 )
    {
      print_wrong_arguments(argv[0]);
      return(-1);
    }
  
  if ( strcmp(argv[1], "-h") == 0 )
    {
      printf("Printing help\r\n");
      print_wrong_arguments(argv[0]);
      return(0);
    }
  
  int number_of_threads = atoi(argv[1]);
  if ( number_of_threads <= 0 )
    {
      print_wrong_arguments(argv[0]);
      return(-1);
    }

  int number_of_trapezes = atoi(argv[2]);
  if ( number_of_trapezes <= 0 )
    {
      print_wrong_arguments(argv[0]);
      return(-1);
    }

  if (pthread_mutex_init(&print_lock, NULL) != 0)
    {
      printf("\r\n mutex init failed \r\n");
      return(-1);
    }

  //
  // Run tests and print result
  //
  
  // devide task on multiple threads
  if ( number_of_threads > number_of_trapezes )
    {
      number_of_threads = number_of_trapezes;
    }
  
  pthread_t threads[number_of_threads];
  
  // measurments
  clock_t start, stop;
  double ms;
  
  // variables for calculations
  double bound_upper = 1;
  double bound_lower = 0;
  int number_of_edges = number_of_trapezes + 1;
  
  double edges[number_of_edges];
  fill_edges(edges, bound_upper, bound_lower, number_of_trapezes);
  
  double step_size = ( bound_upper - bound_lower ) / number_of_trapezes;

  tc_s* tcs[number_of_threads];

  // initialize all tcs's
  int remaining_traps = number_of_trapezes;
  int traps_per_thread = (int) ceil( (double) number_of_trapezes / (double) number_of_threads);
  if ( traps_per_thread < 1 )
    {
      traps_per_thread = 1;
    }
  
  for (int i = 0; i < number_of_threads; i++)
    {
      int trap_start = number_of_trapezes - remaining_traps;
      int num_traps;

      if ( remaining_traps < 1 )
	{
	  num_traps = 0;
	}
      else
	{
	  if ( i != number_of_threads-1 )
	    {
	      num_traps = traps_per_thread;	
	    }
	  else
	    {
	      num_traps = number_of_trapezes - trap_start;
	    }
	}

      tcs[i] = init_thread_container(edges, trap_start, num_traps, step_size);
      remaining_traps = remaining_traps - num_traps;
    }
  
  
  // debug print
  /*
  pthread_mutex_lock(&print_lock);
  fprintf( stderr, "\r\nDEBUG OUTPUT\r\n\r\n" );
  
  fprintf( stderr, "threads:\r\n" );
  for (int i = 0; i < number_of_threads; i++)
    {
      fprintf( stderr, "\tthread %i at %p\r\n", i, &threads[i] );
    }
  
  fprintf( stderr, "trapezes\r\n" );
  fprintf( stderr, "\tnumber of trapezes: %i\r\n", number_of_trapezes );
  fprintf( stderr, "\tstep size %.2f\r\n", step_size );
  fprintf( stderr, "\ttrapezes per threads: %i\r\n", traps_per_thread );

  
  fprintf( stderr, "\tedges = { " );
  for ( int i = 0; i < number_of_edges; i++ )
    {
      fprintf( stderr, "%.2f, ", edges[i] );
    }
  fprintf( stderr, "}\r\n" );
  
  pthread_mutex_unlock(&print_lock);
  */

  //
  // Create threads and integrate
  //

    /*
    typedef struct thread_container {
    double integration_result;
    double* trapezes;
    int trapezes_start_index;
    int trapezes_number_to_integrate;
    double integration_step_size;
    } tc_s;
    */
  
  start = clock();
  for (int i = 0; i < number_of_threads; i++)
    {
      if ( tcs[i]->trapezes_number_to_integrate < 1 )
	{
	  continue;
	}
      
      void* args = tcs[i];
      
      // start a function that calculates the area and prints the result
      if ( pthread_create(&threads[i], NULL, integrate, args) != 0 )
	{
	  pthread_mutex_lock(&print_lock);
	  fprintf( stderr, "Failed to create thread %i at mem loc %p!\r\n", i, &threads[i] );
	  pthread_mutex_unlock(&print_lock);
	}
    }

  // wait for all threads to join
  for (int i = 0; i < number_of_threads; i++)
    {
      if ( pthread_join(threads[i], NULL) != 0 )
	{
	  pthread_mutex_lock(&print_lock);
	  printf( "Failed to join thread %i\r\n", i );
	  pthread_mutex_unlock(&print_lock);  
	}
    }
  stop = clock();
  
  // sum all results and present to user
  //pthread_mutex_lock(&print_lock);
  double result_sum = 0;
  //fprintf( stderr, "\r\nresults\r\n" );
  for (int i = 0; i < number_of_threads; i++)
    {
      result_sum = result_sum + tcs[i]->integration_result;
      //fprintf( stderr, "\tresult %i is %.2f\r\n", i, tcs[i]->integration_result );
    }
  //pthread_mutex_unlock(&print_lock);
  ms = 1000 * ((double) (stop - start)) / CLOCKS_PER_SEC;
  
  // print results
  pthread_mutex_lock(&print_lock);
  printf("result %.3f, trapezes %i, threads %i, ms %.3f\r\n", result_sum, number_of_trapezes, number_of_threads, ms);
  pthread_mutex_unlock(&print_lock);  

  // Destroy
  pthread_mutex_destroy(&print_lock);

  for (int i = 0; i < number_of_threads; i++)
    {
      destroy_thread_container(tcs[i]);
    }
  
  return(0);
  
}


void* integrate(void* args)
{
  /*
    typedef struct thread_container {
    double integration_result;
    double* trapezes;
    int trapezes_start_index;
    int trapezes_number_to_integrate;
    double integration_step_size;
    } tc_s;
  */
  tc_s* tc = args;

  /*
  pthread_mutex_lock(&print_lock);
  fprintf( stderr, "\r\nThread: %ld", pthread_self() );
  fprintf( stderr, "\r\nintegrate()\r\n" );
  fprintf( stderr, "\tresult at %p\r\n", &(tc->integration_result) );
  fprintf( stderr, "\ttrap start %i\r\n", tc->trapezes_start_index );
  fprintf( stderr, "\tnum traps %i\r\n", tc->trapezes_number_to_integrate );
  fprintf( stderr, "\tstep size %.2f\r\n", tc->integration_step_size );
  fprintf( stderr, "\t\t edges = { " );
  for (int i = 0; i < tc->trapezes_number_to_integrate + 1; i++)
    {
      fprintf( stderr, "%.2f, ", tc->trapezes[i + tc->trapezes_start_index] );
    }
  fprintf( stderr, "}\r\n" );
  pthread_mutex_unlock(&print_lock);
  */
  
  double area = 0;
  for (int i = 0; i < tc->trapezes_number_to_integrate; i++)
    {
      double x1 = tc->trapezes[i + tc->trapezes_start_index];
      double x2 = tc->trapezes[i+1 + tc->trapezes_start_index];

      double y1 = y(x1);
      double y2 = y(x2);

      area = area + trapezes(tc->integration_step_size, y2, y1);

      /*
      pthread_mutex_unlock(&print_lock);
      fprintf( stdout, "\t%.2f = y(%.2f), %.2f = y(%.2f), area = %.2f\r\n", y1, x1, y2, x2, area );
      pthread_mutex_unlock(&print_lock);
      */
    }
  
  tc->integration_result = area;

  return(NULL);
}


//
// Printing
//
void print_wrong_arguments(char* filename)
{
  printf( "usage: %s -h\r\n", filename );
  printf( "usage: %s [number of threads] [number of trapezes]\r\n", filename );
}

//
// Calculations
//
double trapezes(double base, double h1, double h2)
{
  double height_base;
  double height_triangle;
  
  if (h1 > h2)
    {
      height_triangle = h1 - h2;
      height_base = h2;
    }
  else
    {
      height_triangle = h2 - h1;
      height_base = h1;
    }

  double result = ( height_base * base ) + ( height_triangle * base / 2 );

  /*
  pthread_mutex_lock(&print_lock);
  fprintf( stderr, "\r\ntrapezes()\r\n" );
  fprintf( stderr, "\tbase %.2f h1 %.2f h2 %.2f\r\n", base, h1, h2 );
  fprintf( stderr, "\ttringel %.2f base %.2f\r\n", height_triangle, height_base );
  fprintf( stderr, "\tresult %.2f\r\n", result );
  pthread_mutex_unlock(&print_lock);
  */
  
  return(result);
}

double y(double x)
{
  return( 4 / (1 + x*x) );
}


void fill_edges(double edges[], double bound_upper, double bound_lower, int number_trapezes)
{
  double step_size = (bound_upper - bound_lower) / number_trapezes;

  for (int i = 0; i < number_trapezes+1; i++)
    {
      edges[i] = step_size * i;
    }
}

