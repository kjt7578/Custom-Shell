
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "arraylist.h"

void expandTokens(ArrayList* arr, char* token);
int hasWildcard(char* token);
int hasPipe(char* token);
int tokenizer(ArrayList* arr, char* line);

#endif