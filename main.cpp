/** 
   Author: Yassine Maalej (Github: yesoun) 
   email: maalej.yessine@gmail.com && maal4948@vandals.uidaho.edu
   Date: Frebuary 2016
   Class: CS541 Advanced Operating Systems
   Insitution: University Of Idaho

Write a distributed program, using MPI, that will solve the PDE described above. Since the calculation over the array is quite parallel, break up the array into“stripes” and have one processor
calculate each stripe. The elements on the boundaries between each stripe need the new values that were calculated in the other stripe in order to complete their calculations. Thus, each stripe will need to coordinate with its adjacent stripes. This will require a communication between the stripes. The exact size of the array and the number of threads should be entered on the command line.
For example:
	pde 64 4
would solve the problem of a 64*64 plate, using 4 threads.
Use the boundary conditions of 100 on three sides, 0 on the other side, and a ep of 0.05.
*/
#include <pthread.h>
#include <stdio.h>
# include <cmath>
# include <cstdlib>
# include <cstring>
# include <ctime>
# include <fstream>
# include <iomanip>
# include <iostream>
# include <mpi.h>
#include <stdlib.h>

using namespace std;

int main ( int argc, char *argv[] );

/** the time stamps of mesages */
void time_stamps ( )
{
 # define TIME_SIZE 40
  /* buffer of time stams of size 40 */
  static char time_buffer[TIME_SIZE];
  const struct tm *tm_ptr;
  size_t len;
  time_t now;

  now = time ( NULL );
  tm_ptr = localtime ( &now );

  len = strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  cout << time_buffer << "\n_value";

  return;
# undef TIME_SIZE
}

/** calculate val */
double function_calculate_value ( double x_value )
{
  double pi;
  double value;
  pi = 3.14159;
  value = 50.0 / ( pi * ( 2500.0 * x_value * x_value + 1.0 ) );
  return value;
}


/**
   Compilation : mpic++ hmwrk_3_main.cpp -o hmwrk_3_main
   execution   : mpiexec -np 6 --hostfile .mpi_hostfile ./hmwrk_3_main
   Setup :
   names of the machines : cs-mpix.cs.uidaho.edu, 
   cp /var/opt/openmpi/.mpi_hostfile $HOME
   Check for the existing .bashrc in home directory:
   less ~/.bashrc
   If it exists:
   Copy contents of /var/opt/openmpi/.bashrc to existing
   If does not exist:
   cp /var/opt/openmpi/.bashrc $HOME
   cp /var/opt/openmpi/examples/cpi.c $HOME
*/

int main ( int argc, char *argv[] )
{
  double a_value;
  double b_value;
  int my_id =0;
  int p_num =0;
  int master=0;
  double total_value;
  double wtime;
  double x_value;
  a_value =  0.0;
  MPI_Status status;
  int tag_value;
  int target_value;
  b_value = 10.0;
  int i_counter;
  int id_counter;
  double my_a_value;
  int n_value;
  n_value = 10000000;
  double my_b_value;
  int my_n_value;
  double my_total;
  int p;
  int q;
  int source;
  double exact_value;
  exact_value = 0.49936338107645674464;
  double error_value;
  
  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &my_id );
  MPI_Comm_size ( MPI_COMM_WORLD, &p_num );

  if ( id_counter == 0 )
  {
    my_n_value = n_value / ( p - 1 );
    n_value = ( p - 1 ) * my_n_value;

    wtime = MPI_Wtime ( );

    time_stamps ( );
    cout << "The value of PDE using MPI. " << endl;
    cout << "  of function_calculate_value(x_value) endl";
    cout << " value of A = " << a_value << endl;
    cout << " value of B = " << b_value << endl;
    cout << " the value of N = " << n_value << endl ;
    cout << " the exact value= " << setw(24) << setprecision(16) << exact_value << endl;
    cout << "\n_value";

  }

  source = 0;
  /** int MPI_Bcast( void *buffer, int count, MPI_Datatype datatype, int root, 
               MPI_Comm comm ) */
  MPI_Bcast ( &my_n_value, 1, MPI_INT, source, MPI_COMM_WORLD );
  if ( id_counter == 0 )
  {
    for ( q = 1; q <= p - 1; q++ )
    {
      my_a_value = ( ( double ) ( p - q     ) * a_value   
             + ( double ) (     q - 1 ) * b_value ) 
             / ( double ) ( p     - 1 );

      target_value = q;
      tag_value = 1;
      /** int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
             MPI_Comm comm)*/
      MPI_Send ( &my_a_value, 1, MPI_DOUBLE, target_value, tag_value, MPI_COMM_WORLD );

      my_b_value = ( ( double ) ( p - q - 1 ) * a_value   
             + ( double ) (     q     ) * b_value ) 
             / ( double ) ( p     - 1 );

      target_value = q;
      tag_value = 2;
      /** int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag,
             MPI_Comm comm)*/
      MPI_Send ( &my_b_value, 1, MPI_DOUBLE, target_value, tag_value, MPI_COMM_WORLD );
    }
    total_value = 0.0;
    my_total = 0.0;
  }
  else
  {
    source = 0;
    tag_value = 1;
    /** int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status *status) */ 
    MPI_Recv ( &my_a_value, 1, MPI_DOUBLE, source, tag_value, MPI_COMM_WORLD, &status );

    source = 0;
    tag_value = 2;
    /** int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
             MPI_Comm comm, MPI_Status *status) */ 
    MPI_Recv ( &my_b_value, 1, MPI_DOUBLE, source, tag_value, MPI_COMM_WORLD, &status );

    my_total = 0.0;
    for ( i_counter = 1; i_counter <= my_n_value; i_counter++ )
    {
      x_value = ( ( double ) ( my_n_value - i_counter     ) * my_a_value 
          + ( double ) (        i_counter - 1 ) * my_b_value )
          / ( double ) ( my_n_value     - 1 );
      my_total = my_total + function_calculate_value( x_value );
    }

    my_total = ( my_b_value - my_a_value ) * my_total / ( double ) ( my_n_value );

    cout << "  Process " << id_counter << " contributed MY_TOTAL = " 
         << my_total << "\n_value";
  }
  /* int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm) **/
  MPI_Reduce ( &my_total, &total_value, 1, MPI_DOUBLE, MPI_SUM, master, MPI_COMM_WORLD );
  if ( id_counter == 0 )
  {
    error_value = fabs ( total_value - exact_value );
    wtime = MPI_Wtime ( ) - wtime;

    cout << "\n_value";
    cout << "  Estimate = " << setw(24) << setprecision(16) << total_value << "\n_value";
    cout << "  Error = " << error_value << "\n_value";
    cout << "  Time = " << wtime << "\n_value";
  }
//Terminates MPI execution environment
  MPI_Finalize ( );
  if ( id_counter == 0 )
  {
    cout << "\n_value";
    cout << "QUAD_MPI:\n_value";
    cout << "  Normal end of execution.\n_value";
    cout << "\n_value";
    time_stamps ( );
  }
  return 0;
}




