#include <assert.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <mutex>

// thread_shared_data_t
struct shared_data {
  uint64_t thread_count;
};

// thread_private_data_t
struct private_data {
  uint64_t thread_number;  // rank
  std::shared_ptr<shared_data> Shared_data;
};

/**
 * @brief ...
 */
void greet(void* data);
int create_threads(std::shared_ptr<shared_data> shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = std::thread::hardware_concurrency();
  if (argc == 2) {
    std::string thread_count_str(argv[1]);
    uint64_t thread_count = std::stoi(thread_count_str);
    if (thread_count == 0) {
      std::cerr << "Error: invalid thread count\n";
      return 11;
    }
  }

  // create shared_data object
  shared_data Shared_data;
  Shared_data.thread_count = thread_count;

  std::shared_ptr<shared_data> shared_data_ptr = std::make_shared<shared_data>();
  if (shared_data_ptr) {
    shared_data_ptr->thread_count = thread_count;

    std::chrono::high_resolution_clock::time_point start_time =
        std::chrono::high_resolution_clock::now();

    error = create_threads(shared_data_ptr);

    std::chrono::high_resolution_clock::time_point finish_time =
        std::chrono::high_resolution_clock::now();
    double elapsed_time =
        std::chrono::duration_cast<std::chrono::duration<double>>(
            finish_time - start_time)
            .count();

    std::cout << "Execution time: " << elapsed_time << "\n";
  } else {
    std::cerr << "Error: could not allocate shared data\n";
    return 12;
  }
  return error;
}  // end procedure

std::mutex Mutex;

int create_threads(std::shared_ptr<shared_data> shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count - 1 do
  std::vector<std::thread> threads;
for (uint64_t thread_number = 0; thread_number < shared_data->thread_count;
     ++thread_number) {
  // Create a private_data object and assign values
  private_data* data = new private_data();
  data->thread_number = thread_number;
  data->Shared_data = shared_data;

  // Create a thread and add it to the vector
  threads.emplace_back(greet, data);
}

  // print "Hello from main thread"
  std::cout << "Hello from main thread\n" << std::endl;;

  for (auto& thread : threads) {
    // join the thread
    //threads.emplace_back(std::move(thread));
    thread.join();
  }

  return error;
}

// procedure greet:
void greet(void* data) {
  // assert(data);
  private_data* Private_data = static_cast<private_data*>(data);
  std::shared_ptr<shared_data> shared_data = Private_data->Shared_data;

  // try to lock the mutex
  std::unique_lock<std::mutex> lock(Mutex);

  // print "Hello from secondary thread"
  std::cout << "Hello from secondary thread " << Private_data->thread_number
            << " of " << shared_data->thread_count << "\n";


  // access the shared_data object
  // do nothing

  // release the lock
  lock.unlock();
}