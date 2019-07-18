/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      Chapter 7 Exercise 2: Implement malloc and free
*                   My code sucks :p
******************************************************************************/
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

struct meta_alloc{
    struct meta_alloc *prev;
    struct meta_alloc *next;
    size_t size;
};

struct meta_alloc mhead; /* head of occupied  mem linking list */
struct meta_alloc ahead; /* head of available mem linking list */

struct meta_alloc* get_meta(void* space){ return space - sizeof(struct meta_alloc); }
void*              get_space(const struct meta_alloc *meta){ return (void*)meta + sizeof(struct meta_alloc); }
void init(){
    if(mhead.prev == NULL && mhead.next == NULL) {
        mhead.prev = mhead.next = &mhead;
        mhead.size = 0;
    }
    if(ahead.prev == NULL && ahead.next == NULL) {
        ahead.prev = ahead.next = &ahead;
        ahead.size = 0;
    }
}

// quest memory and insertion it
struct meta_alloc* _quest_alloc(size_t size, struct meta_alloc* insert){
    // call sbrk to change the program break
    struct meta_alloc* sz = sbrk(size + sizeof(struct meta_alloc));
    // test if failed
    if((void*)sz == (void*)-1) { return (void*) -1; }
    // update program break, this is useful for free function

    // make the new metadata as the successor of specified pointer.
    sz->size = size;
    sz->prev = insert;
    sz->next = insert->next;
    insert->next->prev = sz;
    insert->next = sz;

    return sz;
}

void* malloc(size_t size){
    init();
    // search for available space, perform at least 1000 queries
    struct meta_alloc *ptr;
    for(ptr = ahead.next ; ptr != &ahead; ptr = ptr->next)
        if(size <= ptr->size){ break; /* find it */ } 

    /* deal with it */
    if(ptr == &ahead){ /* no match */
        struct meta_alloc* quest = _quest_alloc(size, &mhead);
        if(quest == (void*)-1) return errno = ENOMEM, NULL;
        return get_space(quest);
    }else{
        /* find a available space */
        /* test if we can break that space into two part. */
        /* if so, break it*/
        if(ptr->size >= size + 16 + 2 * sizeof(struct meta_alloc)){
            struct meta_alloc* new_a = (void*)ptr + sizeof(struct meta_alloc) + size;
            new_a->prev = ptr;
            new_a->next = ptr->next;
            ptr->next = new_a;
            new_a->size = ptr->size - size - 2 * sizeof(struct meta_alloc);
            ptr->size = size;
        }
        
        /* detach the target space from the available mem list */
        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;
        ptr->next = NULL;
        ptr->prev = NULL;

        /* insert it into occupied mem list */
        ptr->next = mhead.next;
        ptr->prev = &mhead;
        mhead.next->prev = ptr;
        mhead.next = ptr;

        /* done */
        return get_space(ptr);
    }
}

void free(void* ptr){
    init();
    struct meta_alloc* meta = get_meta(ptr);

    /* test if we can merge that block of memory with its successor */
    /* if so, merge it with its successor */
    struct meta_alloc* suc = (struct meta_alloc*)(ptr + meta->size);

    struct meta_alloc* seek;
    for(seek = ahead.next; seek != &ahead; seek = seek->next)
        if(seek == suc)
            break;

    /* find it? */
    if(seek == suc){
        /* datach successor from available list, add its size to current ptr */
        suc->next->prev = suc->prev;
        suc->prev->next = suc->next;
        suc->next = NULL;
        suc->prev = NULL;


        meta->size += suc->size + sizeof(struct meta_alloc);
    }

    /* detach this metadata from occupied list */
    meta->next->prev = meta->prev;
    meta->prev->next = meta->next;
    meta->next = NULL;
    meta->prev = NULL;

    /* add this metadata to available list */
    meta->next = ahead.next;
    meta->prev = &ahead;
    ahead.next->prev = meta;
    ahead.next = meta;
}

void print(struct meta_alloc* head){
    for(struct meta_alloc* s = head->next; s != head; s = s->next){
        printf("Meta pos=%10p prev=%10p next=%10p size=%5ld\n",s, s->prev, s->next, s->size);
    }
}

int main(int argc, const char *argv[]){

    // before start
    printf("Occuied  \n"); print(&mhead);
    printf("Available\n"); print(&ahead);

    // allocate 100 int
    int *arr = malloc(sizeof(int) * 100);
    printf("Occuied  \n"); print(&mhead);
    printf("Available\n"); print(&ahead);

    // print content 
    for(int i = 0;i < 100;i++) arr[i] = i;
    for(int i = 0;i < 100;i++) printf("%d ", arr[i]);
    printf("\n");

    // release
    free(arr);
    printf("Occuied  \n"); print(&mhead);
    printf("Available\n"); print(&ahead);

    // allocate 20 int for each ptr
    int* ptr[10];
    for(int i = 0;i < 10;i++)
        ptr[i] = malloc(sizeof(int) * 20);
    printf("Occuied  \n"); print(&mhead);
    printf("Available\n"); print(&ahead);

    // release it all
    for(int i = 9;i >= 0;i--)
        free(ptr[i]);
    printf("Occuied  \n"); print(&mhead);
    printf("Available\n"); print(&ahead);



    return 0;
}
