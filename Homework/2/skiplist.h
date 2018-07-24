/*
 *  Skip List Header File
 *  Author: Yang Zhou, Jianxiong Cai
 *  Last Modification on 6th March 2018
 *  HW2, CS110 Computer Architecture, ShanghaiTech University
 *  Spring 2017
 */


#ifndef SKIPLIST_H
#define SKIPLIST_H

/*
 *  Struct for Skip List
 *    Node Structure:
 *      - void * pointer to key
 *      - void * pointer to value
 *      - skip_list_node ** pointer to the first of several forward nodes.
 *          (PS: You can also understand forward as next)
 *    Tree Structure:
 *      - int: current height of the list
 *      - int: maximum height limit
 *      - skip_list_node * (node) pointer to the header node.
 *      - 3 function pointers to compare/alloc/del function
 *
 *    Important Notes:
 *      For the header node, you must use a dummy node (with key = NULL, value = NULL) to serve as
 *      the header.
 *      You can not use a NULL pointer to indicate this is an empty list on this assignment.
 *      The initial level of a empty list is 1.
 *      You are required to implement allocInt, deleteInt, which is used for allocating memory for int and copying its value.
 */

typedef struct skip_list * list;
typedef struct skip_list_node * node;

typedef struct skip_list_node {
  void *val;
  void *key;
  struct skip_list_node **forward;
} skip_list_node;

typedef struct skip_list {
  int level;                                /* the current height of the skip_list. empty tree get a height of 1 (dummy node) */
  int height_limit;                         /* the maximum height limit of the skip_list */
  node header;                              /* point to the header (a dummy node) */
  int (*cmp)(const void *a, const void *b); /* compare function pointer */
  void *(*alloc)(const void *);             /* alloc function pointer */
  void (*del)(void *);                /* del function pointer */
} skip_list;

/*

NOTICE: Please check for updates of associated files regularly.

WARNING: DO NOT MODIFY THIS HEADER FILE! We will not be using your header file during
grading so any modifications to pass compilation or simplify implementation will
NOT work. Please strictly abide by the instructions.

Instructions on Homework 2:


In this homework, you will implement a Skip List using C89.


The basics of skip list are readily available on the internet. Any ambiguity on the
instructions should be discussed on Piazza or during the discussion sessions. The
instructions in this header file will be constantly updated so please pay attention to
any notifications on source file updates.


We will use linked nodes to implement the Skip List. This Skip List will also offer generic
programming functionality by using general pointers (void *). This will be discussed later.


The structure of the node has been laid out for you. The fields in the struct should be of
interest to you:

  - "key" is a general purpose pointer holding the address of the value. Note that the
    value should be dynamically allocated to avoid use-after-free scenarios while using
    stack data.

  - "value": same as "key"

  - "skip_list_node" is a pointer pointing to the first of next several forward nodes. If the
    node is the last one of the tree, the pointer should be NULL.

  - "cmp", "alloc" and "del" are three function pointers allowing for generics.
    "cmp" is a function tasked with comparing the values that two general purpose pointers point
    to. "alloc" is a function tasked with allocating the memory for a given type and transferring
    the data to the allocated memory.

Note that a dummy header is used to serve as the first element of a Skip List. Once a list is
created, a node (with key = NULL, value = NULL) will be created and serve as the header. Besides,
the initial height of an empty list is always 1.


You will have to implement the following functions: (refer to the comments before the function definition)
  - "createSkipList": A function to create an empty tree. A dummy node is created as the header.

  - "insertNode": A function to insert a new node with key & value determined by "key" into a list.

  - "searchNode": A function to search for the value corresponding to the key.

  - "deleteNode": A function to remove a node with value given by "key" from a list.

  - "freeSkipList": A function to free all the dynamically allocated memory used by the tree.

  - "compareInt": The "cmp" function for comparing integer values.

  - "deleteInt": The "del" function for freeing memory allocated for integers.

  - "allocInt": The "alloc" function for allocating memory for integers (you should allocing memory and copy the value).

Additional Requirements:

  - The comparison function with inputs a and b will return -1 if a < b, return 0 if a==b and
    return 1 if a > b.

  - You may not use any header file apart from this head file and <stdlib.h>. Any other library
    is unnecessary to complete the homework and will not be linked during compilation.

  - No memory leaks are allowed. We will deduct your scores for each and every memory leak found.


Compilation will be conducted using gcc with the compile flags given on the homework website. We
will not be using your copy of Makefile so your attempts to pass compilation through modifying
Makefile will not work.

*/

/*
 * Create an empty list
 *      height_limit: maximum height of the list allowed
 *      cmp/alloc/del: three function pointers for comparing/allocing memory/deleting memory.
 * Return Value:
 *      return a pointer to skip_list (list)
 */
list createSkipList(int height_limit,
                         int (*cmp)(const void *a, const void *b),
                         void *(*alloc)(const void *),
                         void (*del)(void *));
/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 */
void *searchNode(list s, void *key);

/*
 * A function to insert a new node with key & value determined by "key" into a list.
 * Return value:
 *      If the key does not exist before, insert the new key & value and return 0
 *      If the key already exists before, replace the value with the new one, and return 1
 *      return -1 on any kind of failure
 */
int insertNode(list s, void *key, void *val);

/*
 * A function to remove a node with value given by "key" from a list.
 * Return value:
 *      return 0 on success
 *      return -1 on failure
 */
int deleteNode(list s, void *key);

/*
 * A function to free all the dynamically allocated memory used by the tree.
 */
void freeSkipList(list s);


int compareInt(const void *a, const void *b);

void deleteInt(void *a);

void *allocInt(const void *a);


#endif
