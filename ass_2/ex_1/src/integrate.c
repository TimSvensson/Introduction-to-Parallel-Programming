#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_wrong_arguments();
void integrate(int number_of_threads, int number_of_trapezes);

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

  integrate(number_of_threads, number_of_trapezes);
  return(0);
}

void print_wrong_arguments(char* filename)
{
  printf( "usage: %s -h\r\n", filename );
  printf( "usage: %s [number of threads] [number of trapezes]\r\n", filename );
}

void print_integrate_info(int num_threads, int num_trapezes, double b_upper, double b_lower)
{
  printf(" == INTEGRATE INFO == \r\n");
  printf("  Number of threads:\t%i\r\n", num_threads);
  printf("  Number of trapezes:\t%i\r\n", num_trapezes);
  printf("  Upper bound:\t\t%f\r\n", b_upper);
  printf("  Lower bound:\t\t%f\r\n", b_lower);
}

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

  return( height_base * base + height_triangle * base / 2 );
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

void integrate(int number_of_threads, int number_of_trapezes)
{
  double bound_upper = 1;
  double bound_lower = 0;

  print_integrate_info(number_of_threads, number_of_trapezes, bound_upper, bound_lower);

  double area;

  double edges[number_of_trapezes+1];
  fill_edges(edges, bound_upper, bound_lower, number_of_trapezes);

  double step_size = (bound_upper - bound_lower) / number_of_trapezes;

  for (int i = 0; i < number_of_trapezes; i++)
    {
      double x1 = step_size * i;
      double x2 = step_size * (i+1);

      double y1 = y(x1);
      double y2 = y(x2);

      area += trapezes(step_size, y2, y1);
    }

  printf("\r\n\r\n AREA = %f \r\n\r\n", area);
}
