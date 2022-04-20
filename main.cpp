/* -----------------------------------------------------------
 Taylor Powell                              April 20, 2022
 Basic MPI Program - Manager-Worker Setup
 -----------------------------------------------------------*/

#include <iostream>
#include <mpi.h>

int main(int argc, char** argv)
{
    int node, nproc, Ntasks = 1000;
    MPI_Init(&argc, &argv);    
    MPI_Comm_rank(MPI_COMM_WORLD, &node);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Status status;

    if (nproc < 2) {
        std::cout << "This application should run at least 2 processes.\n";
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    if (node == 0) {
        int sender, nWorkers = nproc - 1, Nsent = 0;
        double x, sum = 0.0;

        for (int i = 1; i <= (nWorkers < Ntasks ? nWorkers : Ntasks); i++) {
            x = (Nsent + 1) * 1.0;
            MPI_Send(&x, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            Nsent++;
        }
        for (int i = 0; i < Ntasks; i++) {
            MPI_Recv(&x, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sum += x;
            sender = status.MPI_SOURCE;
            if (Nsent < Ntasks) {
                x = (Nsent + 1) * 1.0;
                MPI_Send(&x, 1, MPI_DOUBLE, sender, 0, MPI_COMM_WORLD);
                Nsent++;
            }
            else
                MPI_Send(MPI_BOTTOM, 0, MPI_DOUBLE, sender, 1, MPI_COMM_WORLD);
        }
        std::cout << "The total is " << sum << "\n";
    }
    else {
        if (node < Ntasks) {
            double x;
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            while (status.MPI_TAG == 0) {
                MPI_Send(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(&x, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        }
    }    
    MPI_Finalize();
    return 0;
}




