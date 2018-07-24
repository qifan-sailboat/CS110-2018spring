#ifndef HW2_SKIP_LIST_CHECK_H
#define HW2_SKIP_LIST_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

/* print the whole int skip list */
void printIntSkiplist(list s){
    int i = 0;
    for (i = 0; i < s->level; i++){
        node curr = s->header->forward[i];
        while (curr != NULL){
            printf("key: %d,value: %d\n",*(int *)(curr->key), *(int *)(curr->val));
            curr = curr->forward[i];
        }
        printf("------\n");
    }
}

#endif
