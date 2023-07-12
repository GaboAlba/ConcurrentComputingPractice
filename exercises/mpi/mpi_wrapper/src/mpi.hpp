// Copyright 2023 Gabriel Alba Romero <gabriel.alba@ucr.ac.cr>

#ifndef EXERCISES_MPI_MPI_WRAPPER_SRC_MPI_HPP_
#define EXERCISES_MPI_MPI_WRAPPER_SRC_MPI_HPP_

#define OMPI_SKIP_MPICXX 1

#include <mpi.h>
#include <stdexcept>
#include <string>
#include <vector>
#include "mpiError.hpp"

class mpiStream;

/**
 * @class Mpi
 * @brief Wrapper class for MPI in C++
 */
class Mpi {
 public:
  /**
   * @brief Constructor
   * @param argc Number of command-line arguments.
   * @param argv Command-line arguments.
   * @throw MpiError if MPI environment init fails.
   */
  Mpi(int argc, char* argv[]) {
    // Initialize MPI environment
    int init;
    MPI_Initialized(&init);
    if (!init) {
      if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        throw mpiError("MPI Init unsuccessful");
      }
    }

    // Assign process number
    MPI_Comm_rank(MPI_COMM_WORLD, &processNumber);

    // Assign process count
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    
    // Assign hostname
    int nameLength;
    char p_hostname[MPI_MAX_PROCESSOR_NAME] = {'\0'};
    MPI_Get_processor_name(p_hostname, &nameLength);
    hostname = std::string(p_hostname, nameLength);
  }

  /**
   * @brief Destructor
   */
  ~Mpi() {
    int final;
    MPI_Finalized(&final);
    if (!final) {
      MPI_Finalize();
    }
  }

  /**
   * @brief Return the current process Rank
   * @return Process Rank
   */
  int getProcessNumber() const {
    return processNumber;
  }

  /**
   * @brief Return the total number of processes.
   * @return Process Count
   */
  int getProcessCount() const {
    return processCount;
  }

  /**
   * @brief Method to get the hostname
   * @return Hostname.
   */
  std::string getHostname() const {
    return hostname;
  }

  /**
   * @brief Method to send a value of generic data type to a specific process
   * @param data Data to send
   * @param destination The destination process number
   * @param tag The message identifier
   * @tparam DataType Generic data type
   * @throw MpiError
   */
  template <typename DataType>
  void send(const DataType data, int destination, int tag = 0) {
    if (MPI_Send(&data, 1, map(data), destination, tag, MPI_COMM_WORLD)
                                                         != MPI_SUCCESS) {
      throw mpiError("MPI_Send unsuccessful");
    }
  }

  /**
   * @brief Send an array of "n" values to another process
   * 
   * @tparam DataType Generic data type
   * @param data Array pointer of data to send
   * @param count The number of values to send
   * @param destination The destination process number
   * @param tag The message identifier
   * @throw MpiError
   */
  template <typename DataType>
  void send(const DataType* data, int count, int destination, int tag = 0) {
    if (MPI_Send(data, count, map(data[0]),
     destination, tag, MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw mpiError("MPI_Send unsuccessful");
    }
  }

  /**
   * @brief Send an array of values to another process
   * 
   * @tparam DataType Generic data type
   * @param data Vector of data to send
   * @param destination The destination process number
   * @param tag The message identifier
   * @throw MpiError
   */
  template <typename DataType>
  void send(const std::vector<DataType>& data, int destination, int tag = 0) {
    if (MPI_Send(data.data(), data.size(), map(data[0]),
     destination, tag, MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw mpiError("MPI_Send unsuccessful");
    }
  }

  /**
   * @brief Send a string to another process
   * 
   * @param str String to send
   * @param destination The destination process number
   * @param tag The message identifier
   * @throw MpiError
   */
  void send(const std::string& str, int destination, int tag = 0) {
    if (MPI_Send(str.c_str(), str.length() + 1, MPI_CHAR,
     destination, tag, MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw mpiError("MPI_Send unsuccessful.");
    }
  }

  /**
   * @brief Method to receive a value from a specific process
   * @param data Data to be received
   * @param origin The source process number
   * @param tag The message identifier
   * @tparam DataType Generic data type
   * @throw MpiError
   */
  template <typename DataType>
  void receive(DataType& data, int origin, int tag = MPI_ANY_TAG) {
    if (MPI_Recv(&data, 1, map(data), origin, tag,
    MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw mpiError("MPI_Recv unsuccessful");
    }
  }

  /**
   * @brief Receive until it receives "n" datapoints from another process
   * 
   * @tparam DataType Generic data type
   * @param data Array pointer where the data will be stored
   * @param capacity The maximum number of data to receive
   * @param origin The process number from where to receive
   * @param tag The message identifier
   * @throw MpiError
   */
  template <typename DataType>
  void receive(DataType* values, int capacity, int fromProcess
                    = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values, capacity, map(values[0]), fromProcess,
     tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw mpiError("MPI_Recv unsuccessful");
    }
  }

  /**
   * @brief Receive until it receives "n" datapoints from another process vector
   * 
   * @tparam DataType Generic data type.
   * @param data Vector where the data will be stored.
   * @param capacity The maximum number of data to receive.
   * @param origin The process number from where to receive
   * @param tag The message identifier
   * @throw MpiError
   */
  template <typename DataType>
  void receive(std::vector<DataType>& data, int capacity,
   int origin = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    data.resize(capacity);
    if (MPI_Recv(data.data(), capacity, map(data[0]),
     origin, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw mpiError("MPI_Recv unsuccessful");
    }
  }

  /**
   * @brief Wait until it receives at most capacity values from another process.
   * 
   * @tparam DataType Generic data type.
   * @param data String where the data will be stored.
   * @param capacity The maximum number of values to receive.
   * @param origin The process number from where to receive
   * @param tag The message tag (default: MPI_ANY_TAG).
   * @throw MpiError
   */
  void receive(std::string& data, int origin =
                           MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    MPI_Status status;
    int receivedCount;
    MPI_Probe(origin, tag, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &receivedCount);

    std::vector<char> buffer(receivedCount);
    if (MPI_Recv(buffer.data(), receivedCount, MPI_CHAR, origin,
           tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      throw mpiError("MPI_Recv unsuccessful");
    }
    data = std::string(buffer.data(), receivedCount);
  }

  /**
   * @brief Get the current process Rank
   * @return Process Rank
   */
  int rank() const {
    return processNumber;
  }

  /**
   * @brief Get the total process count
   * @return Process Count
   */
  int size() const {
    return processCount;
  }

  /**
  * @brief Operator to create an MpiStream object
  * @param processNumber Process number for MpiStream.
  * @return MpiStream object
  */
  mpiStream operator[](int processNumber);

 private:
  int processNumber;
  int processCount;
  std::string hostname;

 public:
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long) {
                              return MPI_UNSIGNED_LONG_LONG; }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }
};

#endif  // EXERCISES_MPI_MPI_WRAPPER_SRC_MPI_HPP_
