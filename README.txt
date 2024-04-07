//cat instruc.txt | grep good > input.txt


Project-III-My-shell
Jeongtae Kim (jkim2065)

This is the custom implemented a simple command-line shell, mysh.
This assignment contains three different source codes, arraylist.c, mysh.c and tokenizer.c

arraylist.c: This file is used to implement a dynamic array. A dynamic array is an array whose size can be adjusted dynamically, meaning it can grow or shrink as needed. arraylist.c contains functions to create, manipulate, and manage dynamic arrays. In a shell program, it is necessary to tokenize text to handle user input effectively. ArrayList is useful for storing and managing these tokens. It allows the program to split the user's input into tokens and extract necessary information for processing.

mysh.c: This file contains the main code for the shell program mysh. mysh is a shell that takes user commands as input, interprets them, and executes them. It houses the main function and implements the core logic of the shell. It handles user input, interprets commands, and executes them.

tokenizer.c: This file is responsible for tokenizing user input. It splits the input string into tokens based on certain criteria and processes each token to extract the necessary information for executing commands. Tokenizing and processing tokens are needed in interpreting commands in a shell program.

All the test cases are written with explanation of each function.

This program gives interactive and batch modes
You can access to Batch mode by
./mysh [file name]
You can access to Interactive mode by
./mysh

Prompt format
=====================

Batch mode
Test 1
$ cat helloworld.sh
echo hello world
$ ./mysh testcase/helloworld.sh
hello world

Test 2
$ cat testcase/helloworld.sh | ./mysh
Welcom to mysh!
mysh> hello world

Interactive mode:
$ ./mysh
Welcom to mysh!
mysh> cd subdir
mysh> echo hello
hello
mysh> cd subsubdir
mysh> pwd
/home/runner/Project-III-My-shell/subdir/subsubdir
mysh> cd directory_DNE
mysh: cd: No such file or directory
mysh> cd ../..
mysh> exit
mysh: exiting

Built-in commands
------------------
cd is used to change the working directory used chdir()
cd subdir
cd subdir/subsubdir
cd ../..

pwd prints the current working directory to standard output used getcwd()
pwd

which prints the path that mysh would use if asked to start that program
which ls

exit ends mysh.
exit

Wildcards
------------------
We allow a single asterisk in a file name or in the last section of a path name.
Any file in the specified directory whose name begins with the characters before the asterisk and
ends with the characters after the asterisk is considered to match.

ls *.txt
ls R*t

Redirection
------------------
The tokens < and > are used to specify files for a program to use as standard input and standard
output, respectively. The token immediately following the < or > is understood as a path to the
file, and is not included in the argument list for the program.

echo Hello > testcase/output.txt
cat < testcase/output.txt

Pipes
-----------------
A pipe connects standard input from one program to the standard output from another, allowing
data to “flow” from one program to the next. mysh allows for a single pipe connecting two processes.

cd testcase
cat output.txt | grep 123 number.txt  > output.txt

Error handeling
-----------------
If mysh gets non-exist file or directoy or command, it will print error
mysh: [name]: No such file or directory