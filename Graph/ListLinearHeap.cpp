#include "ListLinearHeap.h"

namespace utils {

	ListLinearHeap::ListLinearHeap(size_t N) : total_number(N + 1) {
		// actually the maximum possible degree of a node is N - 1 
		//     (repeated edge and self-circled edge not considered)
		this->heads = new ListLinearHeapNode[N + 1];
		this->nodes = new ListLinearHeapNode[N + 1];
		this->size_of_each_list = new size_t[N + 1]{};
	}

	ListLinearHeap::~ListLinearHeap() {
		delete [] this->heads;
		delete [] this->nodes;
		delete [] this->size_of_each_list;
	}

	void ListLinearHeap::addNode(size_t no, size_t degree) {
		// make sure the index is correct
		assert(no < total_number);

		// add node into the list, please make sure each node is added only once
		nodes[no].no = no;
		nodes[no].key = degree;

		// connect this node into the linked list
		nodes[no].prev = &heads[no];
		nodes[no].next = heads[no].next;

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

	void ListLinearHeap::modifyNode(size_t no, size_t new_key) {
		// make sure the index is correct
		assert(no < total_number);

		// remove first and add then
		removeNode(no);
		addNode(no, new_key);
	}

	std::pair<size_t, size_t> ListLinearHeap::top() const {
		// the heap shouldn't be empty
		assert(total_size);

		// return the inquired value
		ListLinearHeapNode* first_node = heads[lowest_index].next;
		return std::make_pair(first_node->no, first_node->key);
	}

	void ListLinearHeap::pop() {
		// the heap shouldn't be empty
		assert(total_size);

		// remove the node from the linked list
		removeNode(heads[lowest_index].next->no);
	}

	size_t ListLinearHeap::size() const {
		return total_size;
	}
}
