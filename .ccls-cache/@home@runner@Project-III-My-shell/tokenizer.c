#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "arraylist.h"
#include <string.h>
#include <glob.h>
#include <fcntl.h>
#include "tokenizer.h"

#define MAX_ARGS 100

char* arguments[MAX_ARGS];

//Use wildcards to search for file names.
void expandTokens(ArrayList* arr, char* token){
    glob_t result;
    int i;
    int argc = 0;

    for(int i = 0; i < strlen(token); i++){
        if(token[i] == '/'){
            break;
        }
    }

  if (chdir(token) != 0){
      //perror("chdir");
  }

    if(glob(token, GLOB_NOCHECK | GLOB_TILDE, NULL, &result) == 0){
        for(i = 0; i < result.gl_pathc && argc < MAX_ARGS - 1;  i++){
            arguments[(argc)++] = strdup(result.gl_pathv[i]);
        }
    }

    for(int i = 0; i < argc; i++){
        al_push(arr, arguments[i]);
    }

    globfree(&result);
    for(int i = 0; i < argc; i++){
        free(arguments[i]);
    }
}

// Check if token include wildcars
int hasWildcard(char* token){
    int hasWildcard = 0;
    for(int i = 0; i < strlen(token); i++){
        if(token[i] == '*'){
            hasWildcard = 1;
            break;
        }
    }
    return hasWildcard;
}

// Check if token include pipe
int hasPipe(char* token){
    int hasPipe = 0;
    for(int i = 0; i < strlen(token); i++){
        if(token[i] == '|'){
            hasPipe = 1;
            break;
        }
    }
    return hasPipe;
}

// Split the command line into tokens.
int tokenizer(ArrayList* arr, char* line){
    int lineSize = strlen(line);
    int i = 0;
    int left = 0;
    int isPrevTokenRedirect = 0;
    int isPrevWildcard = 0;
    int isLineValid = 1;

    while(i <= lineSize){
        if(line[i] == ' ' && line[left] == ' '){
            i++;
            left = i;
        }
        else if(i == lineSize && line[left] == ' ' && line[i] == ' '){
            i++;
            left = i;
        }
        else if(line[i] == ' ' && line[left] != ' '){
            int size = i - left + 1;
            char* str = malloc(sizeof(char)* size);
            memcpy(str, &line[left], size);
            str[i - left] = '\0';

            if(hasWildcard(str) == 0){
                al_push(arr, str);
                isPrevTokenRedirect = 0;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 0){
                expandTokens(arr, str);
                isPrevWildcard = 1;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 1){
                fprintf(stderr, "mysh: Invalid job: Cannot have wildcard after redirection or pipe.\n");
                expandTokens(arr, str);
                isLineValid = 0;
            }

            i++;
            left = i;
            free(str);
        }
        else if((line[i] == '<' || line[i] == '>' || line[i] == '|') && (line[left] != ' ' && line[left] != '<' && line[left] != '>' && line[left] != '|')){
            int size = i - left + 1;
            char* str = malloc(sizeof(char)* size);
            memcpy(str, &line[left], size);
            str[i - left] = '\0';

            if(hasWildcard(str) == 0){
                al_push(arr, str);
                isPrevTokenRedirect = 0;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 0){
                expandTokens(arr, str);
                isPrevWildcard = 1;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 1){
                fprintf(stderr, "mysh: Invalid job: Cannot have wildcard after redirection or pipe.\n");
                expandTokens(arr, str);
                isLineValid = 0;
            }

            free(str);

            int size2 = 2;
            char* str2 = malloc(sizeof(char)* size2);
            memcpy(str2, &line[i], size2);
            str2[1] = '\0';
            isPrevTokenRedirect = 1;

            if(isPrevWildcard == 1 && hasPipe(str2) == 1){
                fprintf(stderr, "mysh: Invalid job: Cannot have pipe after wildcard\n");
                isLineValid = 0;
            }
            else if(isPrevWildcard == 1 && hasPipe(str2) == 0){
                isPrevWildcard = 0;
            }

            al_push(arr,str2);
            i++;
            left = i;
            free(str2);
        }
        else if(line[i] == '<' || line[i] == '>' || line[i] == '|'){
            int size = 2;
            char* str = malloc(sizeof(char) * size);
            memcpy(str, &line[i], size);
            str[1] = '\0';
            isPrevTokenRedirect = 1;

            if(isPrevWildcard == 1 && hasPipe(str) == 1){
                fprintf(stderr, "mysh: Invalid job: Cannot have pipe after wildcard\n");
                isLineValid = 0;
            }
            else if(isPrevWildcard == 1 && hasPipe(str) == 0){
                isPrevWildcard = 0;
            }

            al_push(arr,str);
            i++;
            left = i;
            free(str);
        }
        else if(i == lineSize - 1 && line[left] != ' ' && line[i] != ' '){
            int size = i - left + 2;
            char* str = malloc(sizeof(char)* size);
            memcpy(str, &line[left], size);
            str[i - left + 1] = '\0';

            if(hasWildcard(str) == 0){
                al_push(arr, str);
                isPrevTokenRedirect = 0;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 0){
                expandTokens(arr, str);
                isPrevWildcard = 1;
            }
            else if(hasWildcard(str) == 1 && isPrevTokenRedirect == 1){
                fprintf(stderr, "mysh: Invalid job: Cannot have wildcard after redirection or pipe.\n");
                expandTokens(arr, str);
                isLineValid = 0;
            }

            i++;
            left = i;
            free(str);
        }
        else{
            i++;
        }
    }

    return isLineValid;
}