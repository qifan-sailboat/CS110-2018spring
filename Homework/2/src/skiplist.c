#include"skiplist.h"
#include<stdlib.h>
#define RAND_HALF (RAND_MAX)/2
/* Head files and defined consts*/


/*
* First, implement the createSkipList function
* We need to create a skiplist and a header

*/
list createSkipList(int height_limit,
                         int (*cmp)(const void *a, const void *b),
                         void *(*alloc)(const void *),
                         void (*del)(void *))
{
	skip_list_node * empty = (skip_list_node *) malloc(height_limit * sizeof(skip_list_node));
	/* Create the header for the skip list. */
	int i = 0;
	skip_list * EmptyList = (skip_list *) malloc(sizeof(skip_list)); 
	/* Create an empty list */
	/* For C90 standard, pointers must be initialized. */
	empty->val = NULL;
	/* Make the header's value becomes NULL*/
	empty->key = NULL;
	/* Make the header's key becomes NULL*/
	empty->forward = (skip_list_node **) malloc(sizeof(skip_list_node) * height_limit);
	/* Make the header's forward becomes NULL*/
	for (i=0;i<=height_limit;++i) empty->forward[i] = NULL;
	EmptyList->level = 1; 
	/* For an empty list, the level is 1. */
	EmptyList->height_limit = height_limit; 
	/* height_limit is given by the given parameter. */
	EmptyList->header = empty;
	/* Make the header as an empty node.*/
	EmptyList->cmp = *cmp;
	/* give the comparison function to skiplist */
	EmptyList->alloc = *alloc;
	/* give the memory allocation function to skiplist */
	EmptyList->del = *del;
	/* give the deletion function to skiplist */
	return EmptyList;
}
/* createSkipList ends */

/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 */


/* search a Node into the list */
/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 *		"Key" is the "location" of value.
 */
void *searchNode(list s, void *key)
{
	/* Level_Now is the variable used to record current level position.*/
	int i = 0;
	/* Loop index*/	
	node ptr;
	for (i=s->level-1; i >= 0; --i){
		for (ptr = s->header;ptr->forward[i] == NULL; ptr = ptr -> forward[i]){
			if ((s->cmp)(ptr -> forward[i] -> key, key) == 0) return ptr -> forward[i] -> val;
		}
	}
	return NULL;
}
/* searchNode part ends*/


/*
 * A function to insert a new node with key & value determined by "key" into a list.
 * Return value:
 *      If the key does not exist before, insert the new key & value and return 0
 *      If the key already exists before, replace the value with the new one, and return 1
 *      return -1 on any kind of failure
 */
int insertNode(list s, void *key, void *val)
{
	
	int node_level = 0;
	/*Initialize the random level for new node level*/
	int i = s -> level - 1;
	/*loop index*/
	node p = s->header;
	/*ptr to the next node*/
	node q = NULL;
	/*ptr to the previous node*/
	node *adj = NULL;
	/*array to store the positions to insert the node */
	skip_list_node * new = (skip_list_node *) malloc((node_level+1) * sizeof(skip_list_node));

	if (key == NULL || val == NULL) return -1;
	/*if key or val is NULL, it goes wrong*/
	while (node_level < s->height_limit - 1 && rand() < RAND_HALF) ++node_level;
	/*generate the level for the new node.*/
	adj = (node*) malloc((node_level+1) * sizeof(node));
	/*allocate memory for the array we used to store the nodes we need to change.*/

	for (;i >= 0;--i){
		/*first, from upper level to lower level*/
		/*update the nodes we need to adjust*/
		while ((q = p -> forward[i]) && (s -> cmp)(q -> key, key) == -1){
			p = q;
		}
		adj[i] = p;
	}
	
	if (q && (s -> cmp)(q -> key, key) == 0){
		q -> val = val;
		return 1;
	}

	if (node_level > s -> level)
		/*the new node's level is larger than the skiplist current level*/
		/*so we need to upgrade it */
	{
		for (i = s -> level;i < node_level; ++i){
			adj[i] = s -> header;
			/*the new node is largest*/
			/*so there must be some levels that has only this new element*/
			/*we need to link it to the header directly.*/
		}

		s -> level = node_level;
		/*change the current skiplist level to the node level*/
		/*since it is the largest one.*/
	}
	/*the next part is to deal with a new node*/
	
	new -> val = val;
	/*give new node the value*/
	new -> key = key;
	/*give new node the key*/
	new-> forward = (skip_list_node **) malloc(sizeof(skip_list_node) * (node_level+1));
	/*initialize the forward ptrs to next nodes*/

	for (i = node_level-1; i >= 0 ; --i){
		/*insert nodes to each level*/
		new -> forward[i] = adj[i] -> forward[i];
		/*link the new ptr to the next one*/
		adj[i] -> forward[i] = new;
		/*link the previous one to the new one*/
	}
	free(adj);
	/*prevent memory leak*/
	return 0;
	
}

/*
 * A function to remove a node with value given by "key" from a list.
 * Return value:
 *      return 0 on success
 *      return -1 on failure
 */
int deleteNode(list s, void *key)
{
	node p = s -> header;
	node q = NULL;
	node *adj;
	int i = 0;

	if (key == NULL) return -1;
	/*if key or val is NULL, it goes wrong*/
	adj = (node*) malloc((s -> level + 1) * sizeof(node));
	/*allocate memory for the array we used to store the nodes we need to change.*/

	/*now exciting part begins*/
	for (i = s -> level - 1;i >= 0;--i){
		/*firstly, like what search part & insert part do,*/
		/*we need to search from higher level to lower ones*/
		while ((q = p -> forward[i]) && ((s -> cmp) (q -> key, key) == -1)) p = q;
		/*if:*/
		/*1. q (p -> forward[i]) is not NULL, i.e. reach the end of the skiplist*/
		/*or*/
		/*2. q is smaller the target key*/
		/*continue looping*/
		adj[i] = p;
		/*when the loop stops, it means that we have arrived the right place at this level*/
		/*Therefore, record it.*/
	}

	if (!q || (q && (s -> cmp)(q -> key, key) != 0)) return -1;
	/*This line of code is used to report errors*/
	/*Those are possible errors:*/
	/*1. there is no such node, p and q are the closest *non-NULL* nodes beside the given key*/
	/*(i.e. (q && q -> key != key))*/
	/*q is not NULL and q is not equal to the key*/
	/*2. q is NULL, it means that we have reached the end of the sequence*/
	/*it means that the key is larger than any key in this skiplist*/
	/*(i.e. !q (q == NULL))*/

	else{
		/*this means that it does have the key we want.*/
		for (i = s -> level - 1; i >= 0; --i){
			/*we need to delete the node and make new links from upper levels to lower ones*/
			if (adj[i] -> forward[i] == q){
				/*if the node is the node we want to delete.*/
				adj[i] -> forward[i] = q -> forward[i];
				/*connect the previous node to the afterward node.*/
				if (s -> header -> forward[i] == NULL) s -> level = s -> level - 1;
				/*If the node happens to be the highest node,*/
				/*we need to deduct skiplevel on level down*/
			}
		}
	}
	free(q);
	/*prevent memory leak*/
	return 0;
	/*report that it is normal*/
}

/*
 * A function to free all the dynamically allocated memory used by the tree.
 */
void freeSkipList(list s)
{
	skip_list_node *current = s -> header -> forward[0];
	/* From head to the end free*/
	skip_list_node *mid = NULL;
	/*used as temp varibles tp store the pointer*/
	while (current != NULL)
	{
		mid = current -> forward[0];
		/*store the memory the pointer points*/
		free(current);
		/*free the memory*/
		current = mid;
		/*pass the pointer to the next*/
	}
	free(s->header);
	/*free the final pointer, header*/
}

/*
The comparison function with inputs a and b will return -1 if a < b, return 0 if a==b and
    return 1 if a > b.
*/
int compareInt(const void *a, const void *b)
{
	int diff = *(int*) a - *(int*) b;
	/*Make difference between a and b */
	if (diff > 0)
	{
		/*diff positive, a>b*/
		return 1;
	}else if (diff == 0){
		/*0, equal */
		return 0;
	}else{
		/*negative, a<b*/
		return -1;
	}
}

void deleteInt(void *a)
{
	free(a);
	/*Just free the pointer to a.*/
}

void *allocInt(const void *a)
{
	int *loc = (int*) malloc(sizeof(int));
	/*loc is the  location of the allocated memory*/
	*loc = *(int*) a;
	/*give the location value, *a*/
	return loc;
	/*return the location pointer*/
}

