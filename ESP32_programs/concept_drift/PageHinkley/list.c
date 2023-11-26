#include <stdlib.h>
#include <stdio.h>
#define T float
#define PRTSYM "%g "
#define SCFSYM "%g"
#define ulong unsigned long


typedef struct Item_{
    T value;
    struct Item_ *next;
} Item;

typedef struct{
    Item *first, *last;
    ulong length;
} List;

void list_append(List *self, T item){
    Item *b = (Item*) malloc(sizeof(Item));
    b->value = item;
    b->next = NULL;

    if (self->last == NULL){
        self->first = b;
    }else{
        self->last->next = b;
    }

    self->last = b;
    self->length++;
}

void list_pop(List *self, ulong index){
    if (index >= self->length || index < 0)
        return;
    else if (self->length == 1){
        free(self->first);
        self->first = NULL;
        self->last = NULL;
    }

    Item *prev = NULL, *it = self->first;
    for (; index > 0; index--){
        prev = it;
        it = it->next;
    }

    if (prev == NULL)
        self->first = it->next;
    else if (it->next == NULL)
        self->last = prev;
    else
        prev->next = it->next;
    
    free(it);
    self->length--;
}

T list_get(List *self, ulong index){
    Item *it = self->first;
    for (; index > 0; index--)
        it = it->next;

    return it->value;
}

void list_print(List *self){
    Item *it = self->first;
    while(it != NULL){
        printf(PRTSYM, it->value);
        it = it->next;
    }
}

List* newList(){
    List *l = (List*) malloc(sizeof(List));
    l->first = NULL;
    l->last = NULL;
    l->length = 0;

    return l;
}