#ifndef HEADER_GUARD_SEARCH_TREE
#define HEADER_GUARD_SEARCH_TREE

#include <pthread.h>

struct node{
    const char *key;
    void *value;
    struct node *lnode, *rnode;
};

struct tree{
    struct node *root;
    pthread_mutex_t mutex;
};

int initialize(struct tree*);
int add(struct tree*, const char*, void*);
int delete(struct tree*, const char*);
int lookup(struct tree*, const char*, void**);

#endif /* ifndef HEADER_GUARD_SEARCH_TREE */
