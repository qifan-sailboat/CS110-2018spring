/* You may only include these two header files. */
#include <utility>      /* For std::pair. */
#include <functional>   /* For std::less. */
//#include <cstdio>
//#include <iostream>
//#include <typeinfo>


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
skiplist<Key, Val, Compare>::skiplist(int height_limit, Compare cmp){
	this->height_limit = height_limit;
	this->cmp = cmp;
	this->current_height = 1;
	this->node_num = 0;

	_listnode *empty = new _listnode(Key(), Val(), height_limit);
	this->header = empty;
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::~skiplist(){
	_listnode *current = this->header;

	while (current != nullptr){
		_listnode *mid = current->forward[0];
		delete current;
		current = mid;
	}
	delete this->header;
}

template<class Key, class Val, class Compare>
std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>
skiplist<Key, Val, Compare>::insert(const Key &key, const Val &val)
{
	// Your code here ...
	_listnode* current;

	_listnode* adjust[1000];
	int node_level = 0, count;

	current = this->header;
	count = 0;
	node_level = 1;
	//std::cout << 1 << std::endl;
	//std::cout << "current_height: " << this->current_height << std::endl;
	if (this->node_num == 0){
		while (node_level < this->height_limit - 1 && rand() < RAND_HALF) ++node_level;
		_listnode *new_node = new _listnode(key, val, node_level);

		for (int i = 0; i < node_level; i++){
			this->header->forward[i] = new_node;
		}
		//std::cout << "node_level = " << node_level << std::endl;
		this->current_height = node_level;
		this->node_num = 1;
		return std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>(iterator(new_node), true);

	}else{
		for (int i = this->current_height-1; i >= 0; i--){
			//std::cout << i << std::endl;
			while (current->forward[i] != nullptr){
				if (cmp(current->forward[i]->_data.first, key)){
					current = current->forward[i];
					continue;
				}else if (cmp(key, current->forward[i]->_data.first)){
					break;
				}else{
					current->forward[i]->_data.second = val;
					//delete adjust;
					return std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>(iterator(current->forward[i]), false);
				}
			}
			adjust[count] = current;
			count++;
		}

		while (node_level < this->height_limit - 1 && rand() < RAND_HALF) ++node_level;
		//std::cout << 1 << std::endl;
		_listnode *new_node = new _listnode(key, val, node_level);
		//std::cout << 3 << std::endl;
		if (node_level > this->current_height){
			//std::cout << "node_level = " << node_level << std::endl;
			this->current_height = node_level;
		}
		//std::cout << 2 << std::endl;
		for (int i = node_level-1; i >= 0; i--){
			if (i < count){
				new_node->forward[i] = adjust[count-1-i]->forward[i];
				adjust[count-i-1]->forward[i] = new_node;
			}else{
				this->header->forward[i] = new_node;
			}
		}
		this->node_num++;
		return std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>(iterator(new_node), true);
	}
	/*
	for (int i = 0; i < this->current_height; i++){
		new_node->forward[i] = adjust[i]->forward[i];
		adjust[i]->forward[i] = new_node;
	}
	//std::cout << 1 << std::endl;
	if (node_level > this->current_height){
		for (int i = this->current_height; i < node_level; i++){
			this->header->forward[i] = new_node;
		}
		this->current_height = node_level;
	}
	//std::cout << 1 << std::endl;
	this->node_num++;
	delete adjust;
	return std::pair<typename skiplist<Key, Val, Compare>::iterator, bool>(iterator(new_node), true);
	*/
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

template<class Key, class Val, class Compare>
bool skiplist<Key, Val, Compare>::erase(const Key &key){
	_listnode *current = this->header;
	_listnode* adjust[1000];
	//std::cout << "current_height: " << this->current_height << std::endl;
	for (int i = this->current_height-1; i >= 0; i--){
		while (current->forward[i] != nullptr){
			if (cmp(current->forward[i]->_data.first, key)){
				current = current -> forward[i];
			}else{
				break;
			}
		}
		adjust[i] = current;
	}

	if (this->node_num != 0){
		if (!cmp(current->forward[0]->_data.first, key) && !cmp(key, current->forward[0]->_data.first)){
			_listnode *temp = current->forward[0];
			current = current->forward[0];
			int node_level = current->level;

			for (int i = 0; i < node_level; i++){
				adjust[i]->forward[i] = current->forward[i];
			}

			delete temp;
			this->node_num--;
			int sum = 0;
			for (int i = 0; i < this->current_height;i++){
				if (this->header->forward[i] != nullptr) sum++;
			}
			this->current_height = sum;
			//std::cout << "current_height: " << this->current_height << std::endl;
			return true;
		}
	}
	return false;
	/*
	_listnode *current, *next;
	_listnode **adjust = new _listnode*[this->current_height+1];

	current = this->header;
	next = nullptr;
	for (int i = this->current_height-1; i >= 0; i--){
		while ((next = current->forward[i]) && this->cmp(next->_data.first, key)){
			current = next;
		}
		adjust[i] = current;
	}

	if (!next || (next && (this->cmp(next->_data.first, key) || this->cmp(key, next->_data.first)))){
		delete adjust;
		return false;
	}

	for (int i = this->current_height-1; i >= 0; i--){
		if (adjust[i]->forward[i] == next){
			adjust[i]->forward[i] = next->forward[i];
			if (this->header->forward[i] == nullptr) this->current_height -= 1;
		}
	}
	delete next;
	delete adjust;
	return true;
	*/
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::iterator 
skiplist<Key, Val, Compare>::find(const Key &key){
	_listnode *current = this->header;

	for (int i = this->current_height-1; i >= 0; i--){
		while (current->forward[i] != nullptr){
			if (!cmp(key, current->forward[i]->_data.first)){
				current = current->forward[i];
			}else{
				break;
			}
		}
	}
	if (!cmp(current->_data.first, key) && !cmp(key, current->_data.first)){
		return iterator(current);
	}
	return iterator(nullptr);
	/*
	for (int i = this->current_height-1; i >= 0; i--){
		for (_listnode* ptr = this->header; ptr->forward[i] != nullptr; ptr = ptr->forward[i]){
			if (!this->cmp(ptr->forward[i]->_data.first, key) && !this->cmp(key, ptr->forward[i]->_data.first)){
				iterator ans(ptr->forward[i]);
				return ans;
			}
		}
	}
	iterator ans(nullptr);
	return ans;
	*/
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::const_iterator 
skiplist<Key, Val, Compare>::find(const Key &key) const{
	_listnode *current = this->header;

	for (int i = this->current_height-1; i >= 0; i--){
		while (current->forward[i] != nullptr){
			if (!cmp(key, current->forward[i]->_data.first)){
				current = current->forward[i];
			}else{
				break;
			}
		}
	}
	if (!cmp(current->_data.first, key) && !cmp(key, current->_data.first)){
		return const_iterator(current);
	}
	return const_iterator(nullptr);
}

template<class Key, class Val, class Compare>
bool skiplist<Key, Val, Compare>::empty() const{
	return (this->header->forward[0] == nullptr);
}

template<class Key, class Val, class Compare>
int skiplist<Key, Val, Compare>::level() const{
	return this->current_height;
}

template<class Key, class Val, class Compare>
int skiplist<Key, Val, Compare>::size() const{
	return this->node_num;
}
/*
template<class Key,class Val, class Compare>
void skiplist<Key, Val, Compare>::Print()
{
	int i = this->current_height-1;
	std::cout << "current_height = " << this->current_height << std::endl;
	while(i >= 0){
		_listnode* cur = this->header->forward[i];
		printf("this is %d level:", i+1);
		while(cur)
		{
			std::cout<<cur->_data.first<<":"<<"["<<cur->_data.second<<"]"<<" ";
			cur=cur->forward[i];
		}
		i--;
		printf("\n\n");
	}
}
*/
/* Erase node with key. Return true on success, or false if key is not found. */


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
skiplist<Key, Val, Compare>::_listnode::_listnode(Key key, Val val, int node_level): _data(key, val), level(node_level)
{
	forward = new _listnode*[node_level];
	for (int i = 0; i < node_level; i++){
		forward[i] = nullptr;
	}
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::_listnode::~_listnode(){
	delete forward;
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
skiplist<Key, Val, Compare>::iterator::iterator(_listnode *ptr){
	node = ptr;
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::iterator::~iterator(){
	
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator 
skiplist<Key, Val, Compare>::begin(){
	return iterator(this->header->forward[0]);
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::iterator 
skiplist<Key, Val, Compare>::end(){
	return iterator(nullptr);
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::iterator&
skiplist<Key, Val, Compare>::iterator::operator++(){
	node = node->forward[0];
	return *this;
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::iterator 
skiplist<Key, Val, Compare>::iterator::operator++(int){
	iterator temp = *this;
	node = node->forward[0];
	return temp;
}

template<class Key, class Val, class Compare>
bool 
skiplist<Key, Val, Compare>::iterator::operator==(const iterator &other) const{
	return (this->node == other.node);
}

template<class Key, class Val, class Compare>
bool 
skiplist<Key, Val, Compare>::iterator::operator!=(const iterator &other) const{
	return (this->node != other.node);
}

template<class Key, class Val, class Compare>
std::pair<const Key, Val>&
skiplist<Key, Val, Compare>::iterator::operator*() const{
	return (this->node->_data);
}

template<class Key, class Val, class Compare>
std::pair<const Key, Val>*
skiplist<Key, Val, Compare>::iterator::operator->() const{
	return &(this->node->_data);
}

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
template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::const_iterator::const_iterator(_listnode *ptr){
	node = ptr;
}

template<class Key, class Val, class Compare>
skiplist<Key, Val, Compare>::const_iterator::~const_iterator(){
	
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator 
skiplist<Key, Val, Compare>::begin() const{
	return const_iterator(this->header->forward[0]);
}

template<class Key, class Val, class Compare>
inline typename skiplist<Key, Val, Compare>::const_iterator 
skiplist<Key, Val, Compare>::end() const{
	return const_iterator(nullptr);
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::const_iterator &
skiplist<Key, Val, Compare>::const_iterator::operator++(){
	this->node = this->node->forward[0];
	return *this;
}

template<class Key, class Val, class Compare>
typename skiplist<Key, Val, Compare>::const_iterator 
skiplist<Key, Val, Compare>::const_iterator::operator++(int){
	const_iterator temp = *this;
	this->node = this->node->forward[0];
	return temp;
}

template<class Key, class Val, class Compare>
bool 
skiplist<Key, Val, Compare>::const_iterator::operator==(const const_iterator &other) const{
	return (this->node == other.node);
}

template<class Key, class Val, class Compare>
bool 
skiplist<Key, Val, Compare>::const_iterator::operator!=(const const_iterator &other) const{
	return (this->node != other.node);
}

template<class Key, class Val, class Compare>
std::pair<const Key, Val>&
skiplist<Key, Val, Compare>::const_iterator::operator*() const{
	return (this->node->_data);
}

template<class Key, class Val, class Compare>
std::pair<const Key, Val>*
skiplist<Key, Val, Compare>::const_iterator::operator->() const{
	return &(this->node->_data);
}

/* ==================== End students' code ==================== */