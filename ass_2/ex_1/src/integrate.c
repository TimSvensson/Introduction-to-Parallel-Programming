#include <stdio.h>

int main(int argc, char* argv[])
{
  if (args < 2) 
    {
      printf( "usage: %s -h", argv[0] );
      printf( "usage: %s [flags] [function]", argv[0] );
      printf( "\r\nflags:\n\r\t-n\tSpecify number of threads to be used, default 1." );
      printf( "\r\n\t-t\tNumber of trapezes to be used, default 1." );
      return(-1);
    }

  
}
