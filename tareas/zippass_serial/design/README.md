# ZIPPass_Serial Design

For more information about the system: [MainReadme](../README.md)

## Overview of the system
In the case of the design of this system, the approach detailed in the UML chart was chosen thinking in both scalability and maintainibility. The system is divided in the following way: 
* 4 subroutine modules
* 2 data structures

In the case of the subroutine modules they are divided in the following way: 
1. Zippass_serial: This module is dedicated to creating the data structures, receiving the input data and printing out the result of each decrypted file 
2. Password_logic: This module is dedicated to generating new passwords and inputting them into the zip_operations module.
3. Input_reader: This module is dedicated to processing and validating the information inputted by the user 
4. Zip_operations: This module tests a generated password against the file that wants to be decrypted.

In the case of the data structures they are divided in the following way: 
1. publicData: Data that is shared among all the files that were inputted
2. privateData: Data that is unique to a single file,  like it's name and password. It also contains the publicData as part of itself
![Overview of The System](UML_overview.drawio.svg)

------------
## Flowchart of the design
To understand better how the system works, a flowchart with sample data is provided. It details the whole decision process and data flow of the system. It can be summarized in the following way:
1. Data is inputted into our `main()` function
2. This data is validated to be composed of only ASCII characters. If not, go back to main
3. The system determines the lines and number of lines in the input
4. The lines are saved in an array of lines
5. These lines' format is validated for a series of tests
6. If the data is valid it generates a different data structure for each file
7. The first file will be inputted to the deciphering function. 
8. This function will generate and try new passwords until the password is found or no more passwords can be tested.
9. If there are more files it will repeat from Step 7, if not the system will end it's processing 
![Detailed Flow Chart](UML_flow.drawio.svg)