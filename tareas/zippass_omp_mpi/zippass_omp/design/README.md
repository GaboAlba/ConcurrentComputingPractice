# ZIPPass_Pthread Design

For more information about the system: [MainReadme](../../README.md)

## Overview of the system
In the case of the design of this system, the approach detailed in the UML chart was chosen thinking in both scalability and maintainibility. The system is divided in the following way: 
* 7 subroutine modules
* 5 data structures

In the case of the subroutine modules they are divided in the following way: 
1. **Zippass_pthread**: This module is dedicated to receiving the input data and the amount of threads to run and printing out the result of each decrypted file.
2. **Choice_selection**: This module contains the basic calls to both serial and parallel algorithms to choose from.
3. **Password_logic**: This module is dedicated to generating new passwords and inputting them into the zip_operations module.
4. **Input_reader**: This module is dedicated to processing and validating the information inputted by the user.
5. **Zip_operations**: This module tests a generated password against the file that wants to be decrypted.
6. **Queue_Manager**: This module contains all the operations related to each file tester queue.
7. **Thread_Manager**: This module contains the logic behind the Password Generator and File Tester threads .

In the case of the data structures they are divided in the following way: 
1. publicData: Data that is shared and common among all the threads
2. FilesData: Data containing the file path, the found password and a boolean flag for each file inputted.
3. QueueData: Data that is proper to each testerThread queue like size, and the queue itself.
4. testerThreadData: Data that is owned exclusively by the testerThreads like threadNumber, it's queue.
5. pwdGenData: Data that is owned exclusively by the passwordGenerator like all queues and thread number.

![Overview of The System](UML_overview.drawio.svg)

------------
## Flow of the design
To understand better how the system works, a flowchart with sample data is provided. It details the whole decision process and data flow of the system. It can be summarized in the following way:
1. Data is inputted into our `main()` function
2. This data is validated to be composed of only ASCII characters. If not, go back to main
3. The system determines the lines and number of lines in the input
4. The lines are saved in an array of lines
5. These lines' format is validated for a series of tests
6. If the data is valid the different data structures are iniialized based on the data input
7. The PwdGenData, testerThreadData and numOfThreads are passed onto the `create_threads` function
8. `create_threads` generates one thread for the Password Generator and the remaining threads are assigned to the File Testers.
9. In parallel the threads will work in the following way:
    * ***The Password Generator Thread*** will start generating passwords and cycle through the testerThread queues, enqueueing a single password in each iteration. Once all passwords have been generated it will enqueue a void string indicating that all passwords have been generated and in that way signaling the end of the queue.
    * ***The File Tester Threads*** will dequeue a value from their own queue and test it in all files serially, this way testing a lot of passwords at the same time. Once a password is found for a file, a boolean flag is changed to `true` ,indicating that the password was deciphered for that file, and the password will be logged inside the file data structure. Once all files have been deciphered or all passwords have been tested it will return a success value.
10. Once all iterations are done, or all passwords are deciphewred this will trigger the print of the file paths and their passwords, in the same order they were inputted.

  ![Function Diagram](Basic_Functionality.svg)

--------------------------
## Memory Diagram

In order to understand the program better the memory diagram was created for this system for a test case of 3 threads.

![Memory Trace](MemRastreo.drawio.svg)

-------------------------------------

## Improvement over Serial Algorithm

As part of the testing, the time taken by both the serial algorithm and the parallel one were taken. This tests were run in an Ubuntu virtual machine with a 12 CPUs, Core i9-11900k with 24 GB DDR4 3200 MHz RAM memory and a Hard Drive XPG Gammix S50 Lite with a read speed of 3900 MB/s and  write speed of 3200 MB/s. 

As can be seen past 8 cores it's were the algorithm starts to become less efficient very fast. A deeper testing must be done to determine the exact point where this algorithm start to bottleneck. Given the results, we will ignore the case were the algorithm takes less than a second and will be treated as a different problem altogether. The speedup seems to be assymptotic to around 8 or 9, so the top efficiency is going to be around that amount of threads. Here a graph is presented showing the results of the comparison between threads and speedup and efficiency. After that, the tables with the detailed results is presented.

Apart from this, it can be seen how OMP has in general a decrease in performance and efficiency accross the board. This is due to the nature of "plug-n-play" libraries, which most of the times have certain inefficiencies.

### Summary of the results

![Comparison](Comparison.png)

### Results with 24 threads
|Test Case|~Serial(s)|~Parallel 24 threads(s) Pthreads|~Parallel 24 threads(s) OMP|Speedup Pthreads|Speedup OMP|Efficiency Pthreads|Efficiency OMP|
|---------|----------|--------------------------------|---------------------------|----------------|-----------|-------------------|--------------|
| **000** |   72.9   |          10.2                  |           11.62           |      7.15      |    6.27   |        0.30       |     0.26     |
| **001** |   56.5   |           8.2                  |           8.84            |      6.89      |    6.39   |        0.29       |     0.27     |
| **002** |  101.2   |          14.6                  |           15.58           |      6.93      |    6.50   |        0.29       |     0.27     |
| **003** |  131.4   |          19.4                  |           19.86           |      6.77      |    6.62   |        0.28       |     0.28     |
| **004** |   0.43   |          0.66                  |           0.64            |      0.65      |    0.65   |        0.03       |     0.03     |
| **005** |  173.8   |          17.7                  |           27.84           |      9.82      |    6.24   |        0.41       |     0.26     |
| **006** | 1171.4   |         159.0                  |           172.63          |      7.36      |    6.79   |        0.31       |     0.28     |
| **007** | 5530.8   |         810.0                  |           869.75          |      6.83      |    6.35   |        0.38       |     0.26     |
|         |          |                                |            **AVG**        |    **7.39**    |  **6.45** |      **0.32**     |   **0.27**   |



### Results with 12 threads
|Test Case|~Serial(s)|~Parallel 12 threads(s) Pthreads|~Parallel 12 threads(s) OMP|Speedup Pthreads|Speedup OMP|Efficiency Pthreads|Efficiency OMP|
|---------|----------|--------------------------------|---------------------------|----------------|-----------|-------------------|--------------|
| **000** |   72.9   |          10.5                  |            11.46          |     6.94       |    6.36   |   0.58            |    0.53      |
| **001** |   56.5   |           8.2                  |            8.97           |     6.89       |    6.30   |   0.57            |    0.53      |
| **002** |  101.2   |          14.8                  |            15.81          |     6.83       |    6.40   |   0.56            |    0.57      |
| **003** |  131.4   |          21.0                  |            20.77          |     6.25       |    6.33   |   0.52            |    0.53      |
| **004** |   0.43   |          0.68                  |            0.99           |     0.63       |    0.43   |   0.05            |    0.04      |
| **005** |  173.8   |          27.9                  |            27.3           |     6.22       |    6.37   |   0.52            |    0.53      |
| **006** | 1171.4   |         159.5                  |            174.21         |     7.34       |    6.72   |   0.61            |    0.56      |
| **007** | 5530.8   |         849.3                  |            851.93         |     6.51       |    6.49   |   0.54            |    0.54      |
|         |          |                                |         **AVG**           |   **6.71**     |  **6.42** |**0.56**           |  **0.54**    |


### Results with 8 threads
|Test Case|~Serial(s)|~Parallel 8 threads(s) Pthreads|~Parallel 8 threads(s) OMP|Speedup Pthreads|Speedup OMP|Efficiency Pthreads|Efficiency OMP|
|---------|----------|-------------------------------|--------------------------|----------------|-----------|-------------------|--------------|
| **000** |   72.9   |          11.6                 |           12.89          |       6.28     |    5.65   |          0.79     |     0.71     |
| **001** |   56.5   |           9.5                 |           9.79           |       5.95     |    5.77   |          0.74     |     0.72     |
| **002** |  101.2   |          17.0                 |           17.24          |       5.95     |    5.87   |          0.74     |     0.73     |
| **003** |  131.4   |          22.4                 |           22.66          |       5.87     |    5.80   |          0.73     |     0.73     |
| **004** |   0.43   |          0.61                 |           0.61           |       0.70     |    0.70   |          0.09     |     0.09     |
| **005** |  173.8   |          30.1                 |           30.65          |       5.77     |    5.67   |          0.72     |     0.71     |
| **006** | 1171.4   |         183.6                 |           190.55         |       6.38     |    6.15   |          0.80     |     0.77     |
| **007** | 5530.8   |         933.0                 |           986.76         |       5.93     |    5.61   |          0.74     |     0.70     |
|         |          |                               |            **AVG**       |     **6.01**   |  **5.78** |        **0.75**   |   **0.72**   |


### Results with 4 threads
|Test Case|~Serial(s)|~Parallel 4 threads(s) Pthreads|~Parallel 4 threads(s) OMP|Speedup Pthreads|Speedup OMP|Efficiency Pthreads|Efficiency OMP|
|---------|----------|-------------------------------|--------------------------|----------------|-----------|-------------------|--------------|
| **000** |   72.9   |          24.3                 |             26.35        |       3.00     |    2.76   |        0.75       |     0.69     |
| **001** |   56.5   |          20.2                 |             20.82        |       2.80     |    2.71   |        0.70       |     0.68     |
| **002** |  101.2   |          34.8                 |             37.18        |       2.91     |    2.72   |        0.73       |     0.68     |
| **003** |  131.4   |          46.0                 |             48.49        |       2.86     |    2.71   |        0.72       |     0.68     |
| **004** |   0.43   |          0.61                 |             0.75         |       0.70     |    0.57   |        0.18       |     0.14     |
| **005** |  173.8   |          61.6                 |             64.52        |       2.82     |    2.69   |        0.71       |     0.67     |
| **006** | 1171.4   |         362.3                 |             406.79       |       3.23     |    2.88   |        0.81       |     0.72     |
| **007** | 5530.8   |        1943.4                 |             2088.52      |       2.85     |    2.65   |        0.71       |     0.66     |
|         |          |                               |            **AVG**       |     **2.92**   |  **2.73** |      **0.73**     |   **0.68**   |