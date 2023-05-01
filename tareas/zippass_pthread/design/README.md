# ZIPPass_Serial Design

For more information about the system: [MainReadme](../README.md)

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

## NEED TO CHANGE
![Overview of The System](UML_overview.drawio.svg)

------------
## Flowchart of the design
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
10. Once all iterations are done this will trigger the print of the file paths and their passwords, in the same order they were inputted.

## NEED TO CHANGE
![Detailed Flow Chart](UML_flow.drawio.svg)

--------------------------
## Memory Diagram (NEED TO ADD)