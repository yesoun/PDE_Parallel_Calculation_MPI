# PDE_Parallel_Calculation_MPI

Programming Details
For an n × n problem, define the maximum size of the array to be n + 2 × n + 2, and then run
your loop from 1 . . . n. This will leave the boundaries unchanged.

The Assignment
Write a distributed program, using MPI, that will solve the PDE described above. Since the
calculation over the array is quite parallel, break up the array into“stripes” and have one processor
calculate each stripe. The elements on the boundaries between each stripe need the new
values that were calculated in the other stripe in order to complete their calculations. Thus,
each stripe will need to coordinate with its adjacent stripes. This will require a communication
between the stripes.
The exact size of the array and the number of threads should be entered on the command line.
For example:
pde 64 4
would solve the problem of a 64 × 64 plate, using 4 threads.
Use the boundary conditions of 100 on three sides, 0 on the other side, and an ǫ of 0.05 .
