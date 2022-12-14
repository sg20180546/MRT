#ifndef LIST_H_
#define LIST_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define list_entry(LIST_ELEM, STRUCT, MEMBER)           \
        ((STRUCT *) ((uint8_t *) &(LIST_ELEM)->next     \
                     - offsetof (STRUCT, MEMBER.next)))
#define nullptr NULL

struct list_elem{
    struct list_elem* next;
    struct list_elem* prev;
};

struct list{
    struct list_elem head;
    struct list_elem tail;
};

typedef bool list_less_func (const struct list_elem *a,
                             const struct list_elem *b,
                             void *aux);

void list_init(struct list* list);
struct list_elem* list_begin(struct list* list);
struct list_elem* list_next(struct list_elem* elem);
struct list_elem* list_end(struct list* list);

void list_push_back(struct list* list,struct list_elem* elem);

struct list_elem* list_pop_front(struct list* list);

void list_insert(struct list_elem* before,struct list_elem* elem);

void list_insert_ordered (struct list *list, struct list_elem *elem,
                     list_less_func *less, void *aux);

struct list_elem* list_remove(struct list_elem* elem);

size_t list_size(struct list* list);

struct list_elem* list_front(struct list* list);
#endif