#include <stdlib.h>
#include <string.h>
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
    int s;

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
        else
            return -1;
    }

    /* We found it */
    *p = malloc(sizeof(struct node));

    (*p)->key = key;
    (*p)->value = value;
    (*p)->lnode = NULL;
    (*p)->rnode = NULL;

    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;

    return 0;
}

int delete(struct tree *t, const char *key){
    int s;

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

            return 0;
        }
    }

    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;

    /* No match found*/
    return -1;
}

int lookup(struct tree* t,const char *key, void** value){
    int s;

    if((s = pthread_mutex_lock(&(t->mutex)))) return s;

    struct node *n = t->root;
    int cmp;

    while(n != NULL){
        cmp = strcmp(n->key, key);
        if(cmp > 0) { n = n->rnode; continue; }
        if(cmp < 0) { n = n->lnode; continue; }
        if(cmp == 0){ *value = (n->value); return 0; }
    }

    if((s = pthread_mutex_unlock(&(t->mutex)))) return s;

    return -1;
}
