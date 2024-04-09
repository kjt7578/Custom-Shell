#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char** items;       // Array pointer
    unsigned length;    // Num of current element
    unsigned capacity;  // Array size
} ArrayList;

#ifndef DEBUG
#define DEBUG 0
#endif

int al_init(ArrayList* list, unsigned capacity);
ArrayList *al_create(unsigned capacity);
unsigned al_length(ArrayList *list);
int al_push(ArrayList *list, char* str);
void al_print(ArrayList* list);
int al_contains(ArrayList* list, char* str);
int al_remove(ArrayList *list, int index);
void al_destroy(ArrayList *list);
char* al_get(ArrayList* list, unsigned index);

#endif // ARRAYLIST_H