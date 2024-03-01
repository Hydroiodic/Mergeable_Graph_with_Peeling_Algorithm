#pragma once

#include <vector>
#include <cassert>

namespace utils {

	struct ListLinearHeapNode
	{
		// "key" is the degree of the vertex, while "no" is the index of the vertex
		size_t key = size_t(-1), no = size_t(-1);

		// save the previous and the next nodes
		ListLinearHeapNode* prev = nullptr, * next = nullptr;
	};

	/********************* class ListLinearHeap ******************** /
	 * 
	 * A linked list-based data structure ListLinearHeap is created here
	 * 
	 * reference: Chang, L., Qin, L.: Cohesive Subgraph Computation over 
	 *            Large Sparse Graphs: Algorithms, Data Structures, and 
	 *            Programming Techniques. Springer Series in the Data Sciences, 
	 *            Springer, Heidelberg (2018). https://doi.org/10.1007/978-3-030-03599-0
	 * 
	 * The 2.1 chapter of the reference above mentioned the data structure "ListLinearHeap"
	 * The 3.1 chapter of the reference above mentioned Peeling Algorithm, which will be used to calculate k-core number
	 * 
	 * The basic implementation of ListLinearHeap is to use a List to store some "keys", each of which
	 *     corresponds to a linked list. The advantage of this data structure is that we could insert or
	 *     delete an element with time-complexity of O(1) if we don't care about the order of those elements 
	 *     with the same key. However the disadvantage is that space used by this data structure is relatively
	 *     large.
	 * 
	/********************* class ListLinearHeap *********************/

	class ListLinearHeap
	{
	private:
		// here the structure to store data can be replaced by vector. but in this project, 
		//     the "key" has been mapped from 0 to N - 1. Moreover, allocating a group of 
		//     memory at once on the memory is quicker than that with much more times.
		//     Thus, here I use a raw pointer to store our data.
		ListLinearHeapNode* heads = nullptr;
		size_t total_number;

		// transform from the node number into the pointer
		// ATTENTION! no check here, please make sure all nodes have been recorded
		ListLinearHeapNode* nodes = nullptr;
		size_t* size_of_each_list = nullptr;

		// record the lowest key_value among nodes according to the attributes of heap
		// actually this is the max number for size_t
		size_t lowest_index = -1;

		// the total size in the linked list
		size_t total_size = 0;

	public:
		// the construction and destruction functions
		ListLinearHeap(size_t N);
		~ListLinearHeap();

		// inquire the key value of a node
		size_t inquire(size_t no) const;

		// this function actually initialize a node
		void addNode(size_t no, size_t degree);

		// remove a node from the linked list
		void removeNode(size_t no);

		// modify the key of the node
		void modifyNode(size_t no, size_t new_key);

		// get the top element of the heap
		std::pair<size_t, size_t> top() const;

		// pop the top element of the heap
		void pop();

		// return the size of all element in the linked list
		size_t size() const;

		// return true if the linked list is empty
		bool empty() const;
	};
}
