/*
 * ShanghaiTech University
 * 
 * Computer Architecture I
 * 
 * HW 1
 * 
 * (c) 2016 Sören Schwertfeger
 * 
 */
#ifndef DOUBLL_H
#define DOUBLL_H

/*
 Header file for a doubly linked list.
 An empty list has NULL pointers for first and last.
 items is a counter for the total numbers of items in the list.
 bytes is a counter for the total size in the list.

 A list item (doubll_item) has a pointer to the previous and next items (NULL if non-existent). 
 It also has a pointer to the data (owned by the list item) and the size of that data. 

*/

typedef struct doubll doubll;

typedef struct doubll_item doubll_item;

/* The doubly linked list.
*/
struct doubll
{
	doubll_item * first;
	doubll_item * last;

	size_t items;
	size_t bytes;
};

/* One item in the doubly linked list.
*/
struct doubll_item
{
	doubll_item * prev;
	doubll_item * next;

	void * data;
	size_t size;
};


/*
  Initializes doubly linked list
*/
void init(doubll *list);

/*
  Inserts a new item in the list after the item item. The data is copied. The new item is returned. If item is NULL the new item will be inserted at the beginning of the list.

  If the define CHECK_DLLIST is set your code has to check if the item item is actually member of the list. Returns NULL on error. This check is slow...
  If the define CHECK_DLLIST is not set your code shall not perform that check but rather run very fast.
*/
doubll_item * insert_list(doubll *list, doubll_item *item, void * data, size_t size);

/*
  Removes the item from the list and returns the next item

  If the define CHECK_DLLIST is set your code has to check if the item to be removed is actually member of the list. Returns NULL on error. This check is slow...
  If the define CHECK_DLLIST is not set your code shall not perform that check but rather run very fast.
*/
doubll_item * remove_item(doubll *list, doubll_item * item);

/*
  purges all items from the list
*/
void purge_list(doubll *list);

/*
  Returns a new list with the order of items reversed
*/
doubll reverse_list(doubll *list);



#endif
