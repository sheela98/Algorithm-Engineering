#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {
    // using MPI with C++ bindings
    MPI::Init(argc, argv); // processes are independent until MPI initialization
    // now processes can collaborate, send/receive messages or synchronize
    // until reaching MPI::Finalize().
    int numP = MPI::COMM_WORLD.Get_size(); // get the number of processes
    int myId = MPI::COMM_WORLD.Get_rank(); // get the ID of the process

    // every process prints Hello
    std::cout << "Process " << myId << " of " << numP << ": Hello, world!\n";
    MPI::Finalize(); // terminate MPI
}
