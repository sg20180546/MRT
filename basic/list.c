#include "list.h"
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>

void list_init(struct list* list){
    list->head.prev=nullptr;
    list->head.next=&list->tail;
    list->tail.prev=&list->head;
    list->tail.next=nullptr;
}

struct list_elem* list_begin(struct list* list){
    return list->head.next;
}


struct list_elem* list_next(struct list_elem* elem){
    return elem->next;
}

struct list_elem* list_end(struct list* list){
    return &list->tail;
}


void list_push_back(struct list* list,struct list_elem* elem){
    list_insert (list_end (list), elem);
}

struct list_elem* list_pop_front(struct list* list){

}

void list_insert(struct list_elem* before,struct list_elem* elem){
    elem->prev=before->prev;
    elem->next=before;
    before->prev->next=elem;
    before->prev=elem;    
}

void list_insert_ordered (struct list *list, struct list_elem *elem,
                     list_less_func *less, void *aux){
    struct list_elem* e;
    for(e=list_begin(list);e!=list_end(list);e=list_next(e)){
        if(less(elem,e,aux)){
            break;
        }
    }

    list_insert(e,elem);
}


struct list_elem* list_remove(struct list_elem* elem){
    elem->prev->next=elem->next;
    elem->next->prev=elem->prev;
    return elem->next;
}


size_t list_size(struct list* list){
    struct list_elem*e;
    size_t cnt=0;
    for(e=list_begin(list);e!=list_end(list);e=list_next(e)){
        cnt++;
    }
    return cnt;
}

struct list_elem* list_front(struct list* list){
  return list->head.next;
}  