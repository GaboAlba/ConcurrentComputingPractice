// Copyright 2023 Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef EXERCISES_MPI_MPI_WRAPPER_SRC_MPIERROR_HPP_
#define EXERCISES_MPI_MPI_WRAPPER_SRC_MPIERROR_HPP_

#include <stdexcept>
#include <string>

class Mpi;

/**
 * @class MpiError
 * @brief Exception class for MPI errors.
 */
class mpiError : public std::runtime_error {
 public:
    /**
     * @brief Constructs an MpiError object
     * @param message Error message
     */
    explicit mpiError(const std::string& message);

    /**
     * @brief Creates an mpiError object for a specific exception and MPI instance
     * @param message Error associated with the exception.
     * @param mpi Mpi object 
     */
    mpiError(const std::string& message, const Mpi& mpi);

    /**
     * @brief Creates an mpiError object for a specific exception, MPI 
     *        information, and thread number.
     * @param message Error associated with the exception.
     * @param mpi Mpi Object
     * @param threadNumber Thread were exception happened
     */
    mpiError(const std::string& message,
            const Mpi& mpi, const int threadNumber);
};

#endif  // EXERCISES_MPI_MPI_WRAPPER_SRC_MPIERROR_HPP_
