#include <assert.h>
#include <stdlib.h>
#include "../src/skiplist.h"
#include "../src/skiplist_check.h"

/*
 * structure check: insertion + deletion + insertion
 */

void get_randomized_input(int * keys, int * values, int len ){
    int i = 0;
    srand(23);
    for (i = 0; i < len; i++){
        keys[i] = rand();
        values[i] = rand();
        printf("%d %d",keys[i],values[i]);
    }
}

int main(){
    int i = 0;
    int keys[100];
    int values[100];
    list s;

    /* input initialization */
    get_randomized_input(keys,values,100);
    puts("");

    /* set random seed */
    srand(33);

    /* create list */
    s = createSkipList(5,compareInt,allocInt,deleteInt);

    /* insertion first 50 elements*/
    for (i = 0; i < 50; i++){
        insertNode(s,&keys[i],&values[i]);
    }
    /* delete half */
    for (i = 0; i < 50; i+=2){
        deleteNode(s,&keys[i]);
    }
    /* insertion */
    for (i = 50; i < 100; i++){
        insertNode(s,&keys[i],&values[i]);
    }

    printIntSkiplist(s);

    /* free list */
    freeSkipList(s);

    return 0;
}
