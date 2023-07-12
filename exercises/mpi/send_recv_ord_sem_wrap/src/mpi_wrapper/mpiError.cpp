// Copyright 2023 Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#include "mpi.hpp"
#include "mpiError.hpp"

mpiError::mpiError(const std::string& message) : std::runtime_error(message) {
}

mpiError::mpiError(const std::string& message, const Mpi& mpi)
    : std::runtime_error(mpi.getHostname() + ":"
    + std::to_string(mpi.getProcessNumber()) + ": " + message) {
}

mpiError::mpiError(const std::string& message,
                  const Mpi& mpi, const int threadNumber)
                  : std::runtime_error(mpi.getHostname() + ":" +
                  std::to_string(mpi.getProcessNumber()) + "." +
                  std::to_string(threadNumber) + ": " + message) {
}
