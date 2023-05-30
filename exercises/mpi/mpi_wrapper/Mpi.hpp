#include <threads.h>
#include <semaphore.h>

class Mpi {
    
    public:
        int process_number;
        int process_count;
        char* process_hostname;
        bool any_process;
        bool any_tag;

    public:
        Mpi();

        ~Mpi();

        bool send();

        bool receive();

        bool broadcast();

        bool reduce();

        bool all_reduce();

        bool scatter();

        bool gather();

        double wall_time();

        pthread_barrier_t wait_barrier();

        int getProcessNumber();

        int getProcessCount();

        char* getHostname();


    

}