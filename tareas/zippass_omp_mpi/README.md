# ZIPPASS_PTHREAD

For information regarding the design of the system: [DesignREADME](design/README.md)

## Problem Description
-----
-----
As part of the Parallel and Concurrent Computing class we have been provided with several zip files with encrypted text files inside which we need to read. 

For this problem, a solution using a multi-threaded "brute force" algorithm must be accomplished. Apart from this, the algorithm should be efficient enough to run all test cases in an hour or less. 

## User Manual
-------
-------
### How to run:

In order to run this program a couple of steps must be followed first. 

1. Right click on `./tareas/zippass_serial` folder, and click:
    * For Linux: `Open in Terminal`
    * For VSCode: `Open in Integrated Terminal`
2. Input `make` into the terminal and press Enter, make sure that there are no errors highlighted in the output. Warnings are OK. 
3. If no errors are present, input `bin/zippass_serial <number of threads>`, where the number of threads is a positive integer and is optional (if it's not present it will default to the amount of CPUs your system has), into the terminal and press Enter.
4. Now you'll see a prompt stating `Please enter required information (use ; when finished): `. In there you will provide the following information for your files: 
    * `Alphabet`: All possible characters that the password can contain, e.g., `abcdef1234@#%`
    * `Length`: The maximum length of the password, e.g., `5` 
    * `Files`: ZIP files that you want to decrypt, e.g., `tests/zip_05/f01.zip`

For this you will need to input the information by replacing the following format with your data: 

**Be mindful of the Enter between the `length` and the `files`** 

<pre><code>Alphabet &ldsh;
Length &ldsh;
&ldsh;
File1 &ldsh;
File2 &ldsh;
.
.
.
FileN &ldsh;
; &ldsh;
</code></pre>
#### **Don't forget to press Enter after the ";" in the last line**
-------------
### What to expect

You will see each of the files being displayed into the terminal with their corresponding password, as long as it was able to be decrypted. For succesful password decryptions, the output at the end will be displayed as follows : 
```
<File1> <Password1>
<File2> <Password2>
.
.
.
<FileN> <PasswordN>
```

If the password was not decrypted, the Password field will be blank for the corresponding file.

------
### Reasons for not decrypting password

In case a file doesn't show a password it means the password was not able to be decrypted. This can happen for various reasons,  the most common are:
* The provided `alphabet` does not contain all of the characters that the password is composed of.
    * This can be solved by adding different characters to the current `alphabet`
* The provided `Length`  is smaller than the actual length of the password
    * This can be solved incrementing the `Length`. Be careful, ensure you go one by one, as longer passwords take considerably more time to decrypt. 
* The provided `File` does not have a password. 

## Credits
-------
-------
Thanks to @yman in StackOverflow for the implementation of the methods `charCount` and `lineator` used in `input_reader`.

Thanks to java2s.com for the implementation of the methods `qs` and `quickSort` used in `input_reader`

All other work is property of Gabriel Alba: <gabriel.alba@ucr.ac.cr>
