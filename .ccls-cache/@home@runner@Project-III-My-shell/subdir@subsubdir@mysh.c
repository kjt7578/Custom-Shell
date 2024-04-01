//BUG : 
//NEXT : Make Batch mode

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // For strtok() and strcmp()
#include <unistd.h> // For fork(), pid_t
#include <sys/wait.h> // For waitpid() and associated macros

#ifndef DEBUG
#define DEBUG 0
#endif

char SHELL_NAME[50] = "mysh";
int EXIT = 0;

int interactMode();
char *readLine();
char **splitLine(char *line);
int executeCommand(char **args);
int numOfBuiltin();
int myShellLaunch(char **args);

//BUILTIN COMMANDS
int myShell_cd(char **args);
int myShell_exit();

char *builtin_cmd[] = {"cd", "exit"};  // TO BE ADDED MORE
int (*builtin_func[]) (char **) = {&myShell_cd, &myShell_exit}; // Array of function pointers for call from execShell



int interactMode(){
  printf("Welcome to my shell!\n");
  char *line;
  char **args;
  while(EXIT == 0){
    printf("%s> ", SHELL_NAME);
    fflush(stdout);
    line = readLine();
    if(DEBUG) printf("READ LINE (LINES): %s\n",line);
    args = splitLine(line);
    executeCommand(args); // 이 부분을 myShellLaunch에서 executeCommand로 변경
    if(DEBUG) printf("READ LINE (TOKEN) : %s\n",line);    
    free(line);
    free(args);
  }
  printf("%s: exiting\n", SHELL_NAME);
  return 1;
}

char *readLine()
{
    char *line = (char *)malloc(sizeof(char) * 1024);
    if (!line){
        perror("Buffer Allocation Error - readLine\n");
        exit(EXIT_FAILURE);}

    int pos = 0;
    ssize_t bufsize = 1024;
    ssize_t bytes_read;
    char c;

    while (1){
        bytes_read = read(STDIN_FILENO, &c, 1); 
        // read one bytes
        if (bytes_read == -1){
            perror("read - readLine\n");
            free(line);
            exit(EXIT_FAILURE);}
        else if (bytes_read == 0 || c == '\n'){ 
          // End and replace to \0 If EOF or New Line
            line[pos] = '\0';
            return line;
        }
        else{
            line[pos] = c;
        }
        pos++;

        // reallocate if buffer exceeded
        if (pos >= bufsize){
            bufsize += 1024;
            line = realloc(line, bufsize * sizeof(char));
            if (!line){
                perror("Buffer Allocation Error - readLine\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **splitLine(char *line){
  char **tokens = (char **)malloc(sizeof(char *) * 64);
  char *token;
  char delim[10] = " \t\n\r\a";  // define delimiter
  int pos = 0, bufsize = 64;
  if (!tokens){
    perror("Buffer Allocation Error. - splitLine 1\n");
    exit(EXIT_FAILURE);}

  token = strtok(line, delim); // search first delimiter
  while (token != NULL){
      tokens[pos] = token;
      pos ++;
      if (pos >= bufsize){  // realloc
        bufsize += 64;
        tokens = realloc(tokens, bufsize * sizeof(char *)); // 이 부분을 수정
        if (!tokens){
        perror("Buffer Allocation Error. - splitLine 2\n");
        exit(EXIT_FAILURE);}
      }
      token = strtok(NULL, delim);  // Keep searching delimiter
    }
    tokens[pos] = NULL;
    return tokens;
}

int executeCommand(char **args){
  int ret;
  if(args[0] == NULL){
    return 1; // no command
  }
  for (int i=0; i < numOfBuiltin(); i++){
    if(strcmp(args[0], builtin_cmd[i]) == 0) // Check if user function matches builtin function name      
      return (*builtin_func[i])(args); // Call respective builtin function with arguments
  }
  ret = myShellLaunch(args);
  return ret;
}

int numOfBuiltin() // Function to return number of builtin commands
{
  return sizeof(builtin_cmd)/sizeof(char *);
}

int myShellLaunch(char **args){ // 왜 있는지 모르겠음 체크바람.
  pid_t pid, wpid;
  int status;
  pid = fork();
  if (pid == 0)
  {
    // The Child Process
    if (execvp(args[0], args) == -1){
      perror("myShell: ");
    }
  exit(EXIT_FAILURE);
  }
  else if (pid < 0){
    //Forking Error
    perror("myShell: ");
  }
  else{// The Parent Process
  do{
      wpid = waitpid(pid, &status, WUNTRACED);
    } 
    while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}


//BUILTIN COMMAND
// Builtin command definitions
int myShell_cd(char **args){
  if (args[1] == NULL){
    printf("myShell: expected argument to \"cd\"\n");
  } 
  else{
    if (chdir(args[1]) != 0){
      perror("cd");
    }
  }
  return 1;
}

int myShell_exit(){
  EXIT = 1;
  return 0;
}

int main(void) {
  //check interactive mode or batch mode
  //if(isatty(1))
  //else
  interactMode();
}