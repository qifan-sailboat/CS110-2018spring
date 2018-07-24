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
	skip_list_node empty;
	/* Create the header for the skip list. */
	skip_list * EmptyList = NULL; 
	/* Create an empty list */
	/* For C90 standard, pointers must be initialized. */
	empty.val = NULL;
	/* Make the header's value becomes NULL*/
	empty.key = NULL;
	/* Make the header's key becomes NULL*/
	empty.forward = (skip_list_node **) NULL;
	/* Make the header's forward becomes NULL*/
	EmptyList->level = 1; 
	/* For an empty list, the level is 1. */
	EmptyList->height_limit = height_limit; 
	/* height_limit is given by the given parameter. */
	EmptyList->header = &empty;
	/* Make the header as an empty node.*/
	EmptyList->cmp = cmp;
	/* give the comparison function to skiplist */
	EmptyList->alloc = *alloc;
	/* give the memory allocation function to skiplist */
	EmptyList->del = del;
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
	int Level_Now = s->height_limit;
	/* Level_Now is the variable used to record current level position.*/
	/* Loop index*/	
	skip_list_node *CurrentNode = s->header->forward[Level_Now];
	/* CurrentNode is the pointer to each node. */
	if (s->header->forward == NULL) return NULL;
	/* For an empty skiplist return NULL */
	for (Level_Now = s->height_limit; Level_Now >= 0; Level_Now--)
	/* from upper level to lower level, check whether there is the key*/
	{
		if (CurrentNode -> forward == NULL || CurrentNode -> forward[Level_Now] -> key > key)
			/* If forward is NULL or the key is larger */
			/* It means that we need to go to lower level */
			/* So we continue to lower level. */
		{
			continue;
			
		}
		else if (CurrentNode -> forward[Level_Now] -> key == key)
			/* the key is just the one we want*/
		{
			return CurrentNode->val;
			/* return the pointer to the value */
		}else{
			/* else, i.e. key is smaller than the target key*/
			/* we should go to forward nodes until the key is equal or larger than the target key */
			while (CurrentNode -> forward[Level_Now] -> key < key) 
				{
					CurrentNode = CurrentNode -> forward[Level_Now];
					/* Move forward to next node. */
					if (CurrentNode -> forward[Level_Now] -> key == key) return CurrentNode->val;
					/* the key is just the one we want*/
					/* return the pointer to the value */
				}
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
	int Level_Now = s->height_limit;
	/* Level_Now is the variable used to record current level position.*/
	/* Loop index*/	
	int i, node_level = 0;
	/* Loop index*/	
	/* Loop level*/
	skip_list_node *CurrentNode = s->header->forward[Level_Now];
	/* CurrentNode is the pointer to each node. */
	while (node_level < s->height_limit - 1 && rand() < RAND_HALF) ++node_level;
	/* Ensure the level of the inserted node. */
	if (s->header->forward == NULL)
	{
		skip_list_node *new_node = (skip_list_node *) malloc(sizeof(skip_list_node *));
		/* Create a newnode to store the inserted node */
		new_node -> forward = NULL;
		/* It is the only node, so the forward is NULL. */
		new_node -> key = key;
		/* Give the key to the new node */
		new_node -> val = val;
		/* Give the value to the new node */
		for (i = 0;i <= node_level;i++)
			/* We need to arrange the node to the level of node_level*/
		{
			s->header->forward[i] = new_node;
			/* From level 0 to node_level, link the new node with the header.*/
		}
		return 0;
		/* It is a new node so we return 0. */
	}else{
		skip_list_node *new_node = (skip_list_node*) malloc(sizeof(skip_list_node*));
		/*Allocate memory for the new node*/
		new_node = NULL;
		/* When it is not an empty list. */
		for (Level_Now = s->height_limit; Level_Now >= 0; Level_Now--)
		/* from upper level to lower level, check whether there is the key*/
		{
			if (CurrentNode -> forward == NULL || CurrentNode -> forward[Level_Now] -> key > key)
				/* If forward is NULL or the key is larger */
				/* It means that we need to go to lower level */
				/* So we continue to lower level. */
			{
				continue;
				
			}
			else if (CurrentNode -> forward[Level_Now] -> key == key)
				/* the key is just the one we want*/
			{
				int CurrentLevel = 0;
				skip_list_node *find_node = NULL;
				/* These two varibles are used to find the current level of the existed node. */
				/* We will check it level by level from 0 to height limit. */
				for (CurrentLevel=0; CurrentLevel<= s->height_limit;CurrentLevel++)
				{
					find_node = s->header->forward[CurrentLevel];
					while (find_node -> forward != NULL && find_node -> forward[CurrentLevel] -> key < key)
					{
						find_node = find_node -> forward[CurrentLevel];
					}
					if (find_node -> forward[CurrentLevel] -> key == key)
					{
						continue;
						/* It means that current level now has the key */
						/* So we continue to upper level. */
					}else{
						/* Else it means that there is no such a node in this level */
						/* Then of course it could not be higher than this level. */
						/* Therefore the loop could be terminated. */
						break;
					}
				}
				if (CurrentLevel < node_level)
					/* It means that the new level is larger than the current level */
					/* We need to add the node to upper levels */
				{
					for (i = CurrentLevel; i <= node_level; i++)
						/* Add the inserted node step by step */
					{
						skip_list_node *find_node = s->header->forward[i];
						/* These two varibles are used to find the current level of the existed node. */
						/* We will check it level by level from 0 to height limit. */
						if (find_node == NULL)
						{
							s->header->forward[i] = CurrentNode;
							CurrentNode -> forward[i] = NULL;
							continue;
						}
						while (find_node -> forward != NULL && find_node -> forward[CurrentLevel] -> key < key) find_node = find_node -> forward[CurrentLevel];
						CurrentNode -> forward[i] = find_node -> forward[i];
						find_node -> forward[i] = CurrentNode;
					}
				}else if (CurrentLevel > node_level){
					/* It means that the new level is smaller than the current level */
					/* We need to delete the node to upper levels */
					for (i=node_level;i<=CurrentLevel;i--)
					{
						skip_list_node *find_node = s->header->forward[i];
						/* These two varibles are used to find the current level of the existed node. */
						/* We will check it level by level from 0 to height limit. */
						while (find_node -> forward != NULL && find_node -> forward[CurrentLevel] -> key < key) find_node = find_node -> forward[CurrentLevel];
						find_node -> forward[i] = CurrentNode -> forward[i];
						CurrentNode -> forward[i] = NULL;
					}

				}
				CurrentNode -> forward[Level_Now] -> val = val;
				/* Replace old value with the new one */
				/* Since the node level has changed, we need to change the structure of the node. */


				return 1;		
			}else{
				/* else, i.e. key is smaller than the target key*/
				/* we should go to forward nodes until the key is equal or larger than the target key */
				while (CurrentNode -> key < key) CurrentNode = CurrentNode -> forward[Level_Now];
				/* Move forward to next node. */

			}
		}
		/*Till now, we are sure that there is no such key in the list*/
		/*Therefore, we create one.*/
		new_node -> val = val;
		/*Give the new node value*/
		new_node -> key = key;
		/*Give the new node key*/
		for (i=0; i<=node_level; i++)
		{
			CurrentNode = s->header->forward[i];
			/*The starting position is the node right after the header.*/
			while (CurrentNode -> forward != NULL && CurrentNode -> forward[i] -> key < key) CurrentNode = CurrentNode -> forward[i];
			/*Find the right place*/
			new_node -> forward[i] = CurrentNode -> forward[i];
			/*The pointer after the new node*/
			CurrentNode -> forward[i] = new_node;

		}
		if (new_node == NULL) free(new_node);
		/*Prevent memory leak*/
		return 0;
	}
}

/*
 * A function to remove a node with value given by "key" from a list.
 * Return value:
 *      return 0 on success
 *      return -1 on failure
 */
int deleteNode(list s, void *key)
{
	if (searchNode(s, key) == NULL)
	{
		return -1;
	}else{
		int i;
		skip_list_node *current = NULL;
		skip_list_node *temp = NULL;
		for (i = 0;i <= s -> height_limit; i++)
		{
			current = s->header->forward[i];
			while (current -> forward[i] -> key != key && current != NULL) current = current->forward[i];
			if (current -> forward[i] -> key == key)
			{
				temp = current -> forward[i];
				current -> forward[i] = current -> forward[i] -> forward[i];
			}
		}
		free(temp);
		return 0;
	}
}

/*
 * A function to free all the dynamically allocated memory used by the tree.
 */
void freeSkipList(list s)
{
	skip_list_node *current = s -> header -> forward[0];
	skip_list_node *mid = NULL;
	while (current != NULL)
	{
		mid = current -> forward[0];
		free(current);
		current = mid;
	}
}

/*
The comparison function with inputs a and b will return -1 if a < b, return 0 if a==b and
    return 1 if a > b.
*/
int compareInt(const void *a, const void *b)
{
	int diff = *(int*) a - *(int*) b;
	if (diff > 0)
	{
		return 1;
	}else if (diff == 0){
		return 0;

	}else{
		return -1;
	}
}

void deleteInt(void *a)
{
	free(a);
}

void *allocInt(const void *a)
{
	int *loc = (int*) malloc(sizeof(int));
	*loc = *(int*) a;
	return loc;
}
