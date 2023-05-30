#include "Mpi.hpp"
#include <stdio.h>

int main(int argc, char* argv[]) {
    try {
        Mpi mpi(argc, argv);
        std::cout << "Hello from main thread of process " << mpi.getProcessNumber()
      << " of " << mpi.getProcessCount() << " on " << mpi.getHostname()
      << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "error: " << std::endl;
    }
}