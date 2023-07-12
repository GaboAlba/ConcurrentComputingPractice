// Copyright 2023 Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef MPI_STREAM_HPP
#define MPI_STREAM_HPP

#include <string>
#include "mpi.hpp"

/**
 *
 * @class MpiStream
 * @brief Class that provides a stream interface for sending and receiving data using MPI.
 */
class mpiStream {
 public:
  /**
   * @brief Constructor
   * @param processNumber Process number associated with the stream
   * @param mpi Mpi Object
   */
  mpiStream(int processNumber, Mpi& mpi)
  : processNumber_(processNumber), mpi_(mpi) {}
  /**
   * 
   * @brief Insertion operator to send data through the stream.
   * @tparam Data type to send.
   * @param data Value to send.
   */
  template <typename DataType>
  mpiStream& operator<<(const DataType& data) {
    mpi_.send(data, processNumber_);
    return *this;
  }
  /**
   *
   * @brief Extraction operator to receive data through the stream.
   * @tparam T Data type to receive.
   * @param data Received data
   */
  template <typename DataType>
  mpiStream& operator>>(DataType& data) {
    mpi_.receive(data, processNumber_);
    return *this;
  }

 private:
    int processNumber_;
    Mpi& mpi_;
};

mpiStream Mpi::operator[](int processNumber) {
    if (processNumber < 0 || processNumber >= processCount) {
      throw std::out_of_range("Process number out of range.");
    }
    return mpiStream(processNumber, *this);
  }

#endif  // MPI_STREAM_HPP
