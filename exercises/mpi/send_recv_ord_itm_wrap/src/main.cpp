#include <iostream>
#include <string>

#include "mpi_wrapper/mpi.hpp"

int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();
    std::string message;

    if (rank > 0) {
      message = "Cript-" + std::to_string(rank);
      mpi.send(message, 0);
    } else {
      for (int origin = 1; origin < count; origin++) {
        mpi.receive(message, origin);
        std::cout << origin << " sent " << message << std::endl;
      }
    }
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }
}