#ifndef LIST_H_
#define LIST_H_
#include <stddef.h>

#define list_entry(LIST_ELEM, STRUCT, MEMBER)           \
        ((STRUCT *) ((uint8_t *) &(LIST_ELEM)->next     \
                     - offsetof (STRUCT, MEMBER.next)))

struct list_elem{
    struct list_elem* next;
    struct list_elem* prev;
};

struct list{
    struct list_elem head;
    struct list_elem tail;
};

void list_push_back(struct list* list,struct list_elem* elem);

struct list_elem* list_pop_front(struct list* list);

#endif