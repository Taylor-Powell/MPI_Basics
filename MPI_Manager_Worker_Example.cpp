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
 
    // Default status variable for the send/receive commands
    MPI_Status status;
 
    // Requires the process to have two nodes at minimum, the manager plus one worker
    if (nproc < 2) {
        std::cout << "This application should run at least 2 processes.\n";
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
 
    // Manager node process
    if (node == 0) {
        int sender, nWorkers = nproc - 1, Nsent = 0;
        double x, sum = 0.0;
        
        // Send the intitial data to the workers
        // ternary operator ensures tasks are sent to MIN(nWorkers, Ntasks)
        for (int i = 1; i <= (nWorkers < Ntasks ? nWorkers : Ntasks); i++) {
            x = (Nsent + 1) * 1.0;
            MPI_Send(&x, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
            Nsent++;
        }
     
        // Now that initital tasks are distributed, start receiving data and sending new
        // batches until all tasks are distributed
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
 
    // Worker node processes
    else {
        // Check to make sure this node is not higher than number of tasks
        if (node < Ntasks) {
            double x;
            MPI_Recv(&x, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            // Until signal to stop is received, keep going
            while (status.MPI_TAG == 0) {
                MPI_Send(&x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(&x, 1, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        }
    }    
    MPI_Finalize();
    return 0;
}




