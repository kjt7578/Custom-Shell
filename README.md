
# Custom Shell Implementation in C

This project was initiated to deepen the understanding of system programming by implementing a simple command-line shell in C. The goal is to explore how basic shell functionalities are implemented and how C operates within a Unix-like system.

## Table of Contents
- [Project Overview](#project-overview)
- [Source Files](#source-files)
- [How to Run](#how-to-run)
- [Prompt Format & Test Cases](#prompt-format--test-cases)
- [Built-in Commands](#built-in-commands)
- [Wildcards](#wildcards)
- [Redirection](#redirection)
- [Pipes](#pipes)
- [Error Handling](#error-handling)

## Project Overview

The custom shell, `mysh`, mimics basic Unix shell functionalities, supporting both interactive and batch mode command execution. It allows users to run commands, handle input/output redirection, and connect processes using pipes.

### Key Features:
- **Interactive and batch mode** command execution
- Built-in commands: `cd`, `pwd`, `which`, `exit`
- **Input/output redirection**: `<`, `>`
- **Piping** support (`|`)
- Wildcard (`*`) matching for files

## Source Files

This project contains three main source files:

- **arraylist.c**: 
  Implements a dynamic array, which adjusts its size dynamically to handle tokens from user input. It is useful for storing and managing the tokens split from the input.
  
- **mysh.c**: 
  The main shell implementation. It handles user input, interprets commands, and executes them. This file contains the core logic of the shell, including error handling, process management, and command execution.
  
- **tokenizer.c**: 
  Responsible for tokenizing the user input. It splits the input string into tokens that `mysh.c` can interpret and execute.

## How to Run

This program supports two modes of execution: **Interactive mode** and **Batch mode**.

1. **Interactive Mode**:
   Run the shell directly to enter interactive mode:
   ```bash
   ./mysh
   ```

2. **Batch Mode**:
   Provide a file containing commands as input to run them in batch mode:
   ```bash
   ./mysh [filename]
   ```

### Example:
```bash
$ ./mysh
Welcome to mysh!
mysh> cd subdir
mysh> echo hello
hello
mysh> exit
mysh: exiting
```

## Prompt Format & Test Cases

### Batch Mode Example
```bash
$ cat testcase/helloworld.sh
echo hello world
$ ./mysh testcase/helloworld.sh
hello world
```

### Interactive Mode Example
```bash
$ ./mysh
Welcome to mysh!
mysh> cd subdir
mysh> pwd
/home/runner/Project-III-My-shell/subdir
mysh> exit
mysh: exiting
```

## Built-in Commands

- **cd**: Change the working directory (`chdir()` function).
   ```bash
   cd subdir
   cd ..
   ```
- **pwd**: Print the current working directory (`getcwd()` function).
   ```bash
   pwd
   ```
- **which**: Print the path of the program that would be executed.
   ```bash
   which ls
   ```
- **exit**: Terminate the shell.

## Wildcards

Supports wildcard (`*`) for file name matching:
```bash
ls *.txt
ls R*t
```

## Redirection

Supports input (`<`) and output (`>`) redirection:
```bash
echo Hello > testcase/output.txt
cat < testcase/output.txt
```

## Pipes

Supports piping between two processes:
```bash
cat output.txt | grep 123 > filtered.txt
```

## Error Handling

Displays error messages for invalid commands, files, or directories:
```bash
mysh: [name]: No such file or directory
```


