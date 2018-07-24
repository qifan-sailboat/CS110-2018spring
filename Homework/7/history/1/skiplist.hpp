/* You may only include these two header files. */
#include <utility>      /* For std::pair. */
#include <functional>   /* For std::less. */


/* You may finally allowed to use // in C++. */


#define RAND_HALF ((RAND_MAX) / 2)


//
//
// The implementation for skiplist class follows here!
//
//
//                S K I P L I S T
//
//

/* ==================== Begin students' code ==================== */

/* One possible function definition has been filled for you,
 * you may change it as you wish. */
template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::skiplist(int height_limit, Compare cmp = Compare()){
	this->height_limit = height_limit;
	this->cmp = cmp;
	this->head = nullptr;
	this->height = 1;
	this->node_num = 0;
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::~skiplist(){
	if (head != nullptr) delete head;
}

template<class Key, class Val, class Compare>
std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>
skiplist<Key, Val, Compare>::insert(const Key &key, const Val &val)
{
	// Your code here ...
	int node_level = 0;

	while (node_level < s->height_limit - 1 && rand() < RAND_HALF) ++node_level;

	if (node_num == 0){
		this->head = new _listnode(std::pair<const Key, Val>(key, val), node_level);
		node_num++;
	}else{
		_listnode *current_node, *next_node, *new_node = new _listnode;
		int current_level = this->height-1;
		_listnode adj[node_level+1];

		current_node = this->head;
		next_node = nullptr;
		for (int i = this->height-1; i >= 0; i--){
			while ((next_node = current_node -> next[i]) && cmp(next_node->data.first, key)){
				current_node = next_node;
			}
			adj[i] = current_node;
		}

		if (next_node && !cmp(next_node->key, key) && !cmp(key, next_node->key)){
			next_node->data.second = val;
			return std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>(skiplist<Key, Val, Compare>::iterator(next_node), false);
		}

		if (node_level > height){
			for (int i = height; i < node_level; i++) adj[i]
		}

	}

}
/* This may seem obscure for beginners, so here's a simple explanation:
	- "template<class Key, class Val, class Compare>"
		- We need to use template for generic skiplist class.
	- "std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>"
		- This is the return type of this function, see insert function in skiplist.h.
		- "typename skiplist<Key, Val, Compare>::iterator"
			- `typename` specifies that the type "skiplist<Key, Val, Compare>" should
				have a field called "iterator", and then use it as the type of `first`
				for std::pair.
		- It is analogy to "std::pair<iterator, bool>" in skiplist.h.
	- "skiplist<Key, Val, Compare>::insert(const Key &key, const Val &val)"
		- The real function name, refers to the inline declaration in skiplist.h.
		- `insert` belongs to type "skiplist<Key, Val, Compare>".

	This function may have the most complicated definiton in this file. The
	definition for other functions would be more friendly. You should know how
	other functions are defined after reading this explanation.
 */


/* ==================== End students' code ==================== */



//
//
// The implementation for _listnode subclass follows here!
//
//
//                L I S T   N O D E
//
//

/* ==================== Begin students' code ==================== */
/* One possible function definition has been filled for you,
 * you may change it as you wish. */
template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::_listnode::_listnode(std::pair<Key, Val> data, int node_level){
	level = node_level;
	_data = data;
	next = new _listnode*[height_limit];
	for (int i = 0; i < height_limit; i++) next[i] = nullptr;
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::_listnode::~_listnode() {
	delete next;
}
/* Simple explanation:
	- The first line is the same template argument as described in the first section.
	- "skiplist<Key, Val, Compare>::_listnode::~_listnode()"
		- This is the destructor function of _listnode subclass, so it does not
			has a return type.
		- `~_listnode` is the destructor function name inside class
			"skiplist<Key, Val, Compare>::_listnode".
 */


/* ==================== End students' code ==================== */



//
//
// The implementation for iterator subclass follows here!
//
//
//                I T E R A T O R
//
//

/* ==================== Begin students' code ==================== */

/* One possible function definition has been filled for you,
 * you may change it as you wish. */
template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::iterator::iterator(_listnode *ptr = nullptr){
	node = ptr;
}

template<class Key, class Val, class Compare>
inline std::pair<const Key, Val> &
skiplist<Key, Val, Compare>::iterator::operator*() const { return /* Your code ... */; }

/* Simple explanation:
	- The first line is the same template argument as described in the first section.
	- "std::pair<const Key, Val> &"
		- The return type of the dereference operator.
		- This is not the data pair itself, but a reference to the pair.
	- "skiplist<Key, Val, Compare>::iterator::operator*()"
		- The dereference funtion, refers to the inline declaration in skiplist.h.
 */

/* ==================== End students' code ==================== */


//
//
// The implementation for iterator subclass follows here!
//
//
//                C O N S T      I T E R A T O R
//
//
/* ==================== Begin students' code ==================== */
/* Fill in the const_iterator part, this should be similiar to iterator */




/* ==================== End students' code ==================== */