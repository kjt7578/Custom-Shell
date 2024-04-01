#include "arraylist.h"

// ArrayList 초기화 및 메모리 할당
int al_init(ArrayList* list, unsigned capacity) {
    list->items = (char**) malloc(capacity * sizeof(char*));

    if (list->items == NULL)
        return 0;

    list->length = 0;
    list->capacity = capacity;
    return 1;
}

// 새 ArrayList 생성
ArrayList *al_create(unsigned capacity) {
    ArrayList* list = malloc(sizeof(ArrayList));
    if (list == NULL)
        return NULL;

    if (al_init(list, capacity)) {
      return list;
    }

    free(list);
    return NULL;
}

// ArrayList 길이 반환
unsigned al_length(ArrayList *list) {
    return list->length;
}

// ArrayList에 요소 추가
int al_push(ArrayList *list, char* str) {
    char* newString = malloc(strlen(str) + 1);
    strcpy(newString, str);

    // 배열이 가득 찼는지 확인
    if (list->length == list->capacity) {
        // 용량 증가
        unsigned newCapacity = list->capacity * 2;

        char** newItems = realloc(list->items, newCapacity * sizeof(char*));

        if (newItems == NULL)
            return 0;

        list->capacity = newCapacity;
        list->items = newItems;
    }

    list->items[list->length++] = newString;
    return 1;
}

// ArrayList 출력
void al_print(ArrayList* list) {
    printf("---\n");
    printf("Length: %u\n", list->length);
    printf("Capacity: %u\n", list->capacity);
    for (int i = 0; i < list->length; i++) {
        printf("a[%d]: \t%p\t\t *a[%d]: \t;%s;\n", i, list->items[i], i, al_get(list, i));
    }
    printf("---\n");
}

// ArrayList에서 특정 요소의 포함 여부 확인
int al_contains(ArrayList* list, char* str) {
    for (int i = 0; i < list->length; i++) {
        if(strcmp(al_get(list, i), str) == 0) {
            return 1;
        }
    }
    return 0;
}

// ArrayList에서 요소 제거
int al_remove(ArrayList *list, int index) {
    if (index < 0 || index >= list->length)
        return 0;

    free(list->items[index]);

    for (int i = index; i < list->length - 1; i++) {
        list->items[i] = list->items[i + 1]; 
    }

    list->length--;
    return 1;
}

// ArrayList 메모리 해제
void al_destroy(ArrayList *list) {
    for(int i = 0; i < list->length; i++) {
        free(list->items[i]);
    }
    free(list->items);
    free(list);
}

// ArrayList에서 특정 인덱스의 요소 가져오기
char* al_get(ArrayList* list, unsigned index) {
    if(index >= list->length) {
        fprintf(stderr, "ERROR: Cannot access element %u of ArrayList when length is %u\n", index, list->length);
        return NULL;
    } else {
        return list->items[index];
    }
}