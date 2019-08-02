#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "search_tree.h"

/* Return errno if failed, zero if succeed */
int initialize(struct tree *t){
    int ret;
    t->root = NULL;
    ret = pthread_mutex_init(&(t->mutex), NULL);
    return ret;
}

/* return zero on succeed */
/* return -1 on conflict */
/* otherwise return a postive value indicate the erron */
int add(struct tree *t, const char *key, void *value){
    int s, ret = 0;

    if((s = pthread_mutex_lock(&(t->mutex)))) return s;

    struct node **p = &(t->root);
    struct node *n = t->root;
    int cmp;

    while(n != NULL){
        cmp = (strcmp(n->key, key));
        if(cmp > 0)
            p = &(n->rnode),n = n->rnode;
        else if(cmp < 0)
            p = &(n->lnode),n = n->lnode;
        else{
            ret = -1; goto ret;
        }
    }

    /* We found it */
    *p = malloc(sizeof(struct node));

    (*p)->key = key;
    (*p)->value = value;
    (*p)->lnode = NULL;
    (*p)->rnode = NULL;

ret:
    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;
    return ret;
}

int delete(struct tree *t, const char *key){
    int s , ret = 0;

    if((s = pthread_mutex_lock(&(t->mutex)))) return s;

    struct node **p = &(t->root);
    struct node *n  = t->root;
    int cmp;

    while(n != NULL){
        cmp = strcmp(n->key, key);        
        if(cmp > 0)
            p = &(n->rnode), n = n->rnode;
        else if(cmp < 0)
            p = &(n->lnode), n = n->lnode;
        else{
            /* Find the maximum node at left-subtree */
            struct node **prep = &(n->lnode);
            struct node *rep = n->lnode;
            while(rep != NULL && rep->rnode != NULL) 
                prep = &(rep->rnode), rep = rep->rnode;
            /* left-subtree is empty ? try first element of right-subtree */
            if(rep == NULL) prep = &(n->rnode), rep = n->rnode;

            if(rep == NULL)
                /* Even the Right-subtree is empty, just delete this node and go */
                free(n), *p = NULL;
            else{
                /* Detach rep */
                *prep = rep->lnode;
                /* Point to the replacement */
                rep->lnode = n->lnode;
                rep->rnode = n->rnode;

                free(n); *p = rep;
            }

            goto ret;
        }
    }
    ret = -1;

ret:
    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;
    return ret;
}

int lookup(struct tree* t,const char *key, void** value){
    int s, ret = -1;

    if((s = pthread_mutex_lock(&(t->mutex)))) return s;

    struct node *n = t->root;
    int cmp;

    while(n != NULL){
        cmp = strcmp(n->key, key);
        if(cmp > 0) { n = n->rnode; continue; }
        if(cmp < 0) { n = n->lnode; continue; }
        if(cmp == 0){ *value = (n->value); ret = 0; goto ret; }
    }

    /* No match found */
ret:
    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;
    return ret;
}
