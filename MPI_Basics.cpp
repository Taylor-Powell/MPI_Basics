/* -----------------------------------------------------------
 Taylor Powell                              April 20, 2022
 Basic MPI Program
 -----------------------------------------------------------*/

#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
    int node, nproc;
    MPI_Init(&argc, &argv);
    
    MPI_Comm_rank(MPI_COMM_WORLD, &node);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    std::cout << "This is node " << node << "\n";
    
    MPI_Finalize();
    return 0;
}
