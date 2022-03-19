#ifndef MISC_UTIL_H
#define MISC_UTIL_H

#define FALSE 0
#define TRUE !FALSE

#define Stack 

#ifndef BS_TYPE
#define BS_TYPE void*
#endif //BS_TYPE

typedef struct _BasicNode {
    void* data;
    struct _BasicNode* link;
} BasicNode;

typedef struct _BasicStack {
    BasicNode* top;
    int size;
} BasicStack;

#endif //MISC_UTIL_H