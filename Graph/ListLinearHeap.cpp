// ListLinearHeap.cpp : include implementations of all methods of the class ListLinearHeap
//

#include "ListLinearHeap.h"

namespace utils {

	// ListLinearHeap::ListLinearHeap, allocate some spaces when creating
	ListLinearHeap::ListLinearHeap(size_t N) : total_number(N + 1) {
		// actually the maximum possible degree of a node is N - 1 
		//     (repeated edge and self-circled edge not considered)
		this->heads = new ListLinearHeapNode[N + 1]{};
		this->nodes = new ListLinearHeapNode[N + 1]{};
		this->size_of_each_list = new size_t[N + 1]{};
	}

	// ListLinearHeap::~ListLinearHeap, release the space allocated previously
	ListLinearHeap::~ListLinearHeap() {
		delete [] this->heads;
		delete [] this->nodes;
		delete [] this->size_of_each_list;
	}

	// ListLinearHeap::inquire, return left-degree of the 'no'-th node
	size_t ListLinearHeap::inquire(size_t no) const {
		// make sure the memory access is safe
		assert(no < total_number);

		return nodes[no].key;
	}

	// ListLinearHeap::addNode, add a node with the index of 'no' and 
	//     degree of 'degree' into this data structure
	void ListLinearHeap::addNode(size_t no, size_t degree) {
		// make sure the index is correct
		assert(no < total_number);

		// add node into the list, please make sure each node is added only once
		nodes[no].no = no;
		nodes[no].key = degree;

		// connect this node into the linked list
		nodes[no].prev = &heads[degree];
		nodes[no].next = heads[degree].next;

		if (heads[degree].next) {
			heads[degree].next = &nodes[no];
		}
		heads[degree].next = &nodes[no];

		// update the state of the data structure
		++total_size;
		++size_of_each_list[degree];
		if (degree < lowest_index) {
			lowest_index = degree;
		}
	}

	// ListLinearHeap::removeNode, remove the node with the index of 'no' from the linked-list
	void ListLinearHeap::removeNode(size_t no) {
		// make sure the index is correct
		assert(no < total_number);

		ListLinearHeapNode* cur_node = &nodes[no];

		// "remove" the node from linked list
		cur_node->prev->next = cur_node->next;
		if (cur_node->next) {
			cur_node->next->prev = cur_node->prev;
		}
		
		cur_node->prev = cur_node->next = nullptr;

		// update the state of the data structure
		--total_size;
		--size_of_each_list[cur_node->key];
		if (total_size == 0) {
			// actually this is the max number for size_t
			lowest_index = -1;
		}
		else if (cur_node->key == lowest_index &&
			size_of_each_list[cur_node->key] == 0) {
			// find the next lowest index
			while (!size_of_each_list[++lowest_index]);
		}
	}

	// ListLinearHeap::modifyNode, modify the node with the index of 'no' with a new degree of 'new_key'
	void ListLinearHeap::modifyNode(size_t no, size_t new_key) {
		// make sure the memory access is safe
		assert(no < total_number);

		// if the node has already been removed from the linked list
		if (nodes[no].prev == nullptr && nodes[no].next == nullptr) {
			return;
		}

		// remove first and add then
		removeNode(no);
		addNode(no, new_key);
	}

	// ListLinearHeap::top, get a pair of (<no>, <degree>) of the top element like most heaps
	std::pair<size_t, size_t> ListLinearHeap::top() const {
		// the heap shouldn't be empty
		assert(total_size);

		// return the inquired value
		ListLinearHeapNode* first_node = heads[lowest_index].next;
		return std::make_pair(first_node->no, first_node->key);
	}

	// ListLinearHeap::pop, pop out the top element like most heaps
	void ListLinearHeap::pop() {
		// the heap shouldn't be empty
		assert(total_size);

		// remove the node from the linked list
		removeNode(heads[lowest_index].next->no);
	}

	// ListLinearHeap::size, return the number of elements in the linked list
	size_t ListLinearHeap::size() const {
		return total_size;
	}

	// ListLinearHeap::empty, return true iff there's no element in the linked list
	bool ListLinearHeap::empty() const {
		return total_size == 0;
	}
}
