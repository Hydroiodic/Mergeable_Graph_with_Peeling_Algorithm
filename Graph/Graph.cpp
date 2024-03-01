// Graph.cpp : include implementations of all methods of the class Graph
//

// the sentence below must be used because this cpp file is included in the header file
#pragma once

#include "Graph.h"

namespace graph {

	/***************** Below are private functions ******************/

	// Graph::Graph, the construction function of Graph
	template <typename vertex_type, typename edge_type>
	Graph<vertex_type, edge_type>::Graph() {
		/* there's nothing to do here up to now */
	}

	// Graph::~Graph, the destruction function of Graph
	template <typename vertex_type, typename edge_type>
	Graph<vertex_type, edge_type>::~Graph() {
		/* there's nothing to do here up to now */
	}

	template <typename vertex_type, typename edge_type>
	bool Graph<vertex_type, edge_type>::addEdgeInner(size_t from, size_t to, const edge_type& value) {
		// judge whether the edge exists or not
		std::unordered_set<std::string>::iterator it = edge_set.find(
			std::to_string(from) + '-' + std::to_string(to));

		// if existing already, return false
		if (it != edge_set.end()) return false;

		/*********************** Begin insertion ***********************/

		// add the edge into the hash-set
		edge_set.insert(std::to_string(from) + '-' + std::to_string(to));

		// create new edge
		Edge<vertex_type, edge_type> new_edge{ from, to, value, tail_index[from] };
		tail_index[from] = nextEdgeIndex();
		assert(tail_index[from] <= edges.size());

		// insert the edge to the correct position
		if (tail_index[from] == edges.size()) {
			edges.push_back(new_edge);
		}
		else {
			edges[tail_index[from]] = new_edge;
		}
		
		// increase the degree of the vertex by 1
		++degrees[from];

		return true;
	}
	
	template <typename vertex_type, typename edge_type>
	bool Graph<vertex_type, edge_type>::deleteEdgeInner(size_t from, size_t to) {
		// get the index of the edge
		std::pair<size_t, size_t> edge_index = getEdgeIndexByIndex(from, to);

		// if not found
		if (edge_index.first == -1) {
			return false;
		}
		// if the edge is on the back of each linked list
		else if (edge_index.second == -1) {
			// the index of 'from'
			size_t from_index = edges[edge_index.first].from;

			// "delete" the edge
			tail_index[from_index] = edges[edge_index.first].prev;
			next_index.push_back(edge_index.first);
		}
		// if the edge is in the middle of each linked list
		else {
			// "delete" the edge
			edges[edge_index.second].prev = edges[edge_index.first].prev;
			next_index.push_back(edge_index.first);
		}

		// decrease the degree of the vertex by 1
		--degrees[from];

		// delete successfully
		return true;
	}

	template <typename vertex_type, typename edge_type>
	size_t Graph<vertex_type, edge_type>::nextEdgeIndex() {
		if (next_index.empty()) {
			// if all spaces are used, insert the new edge at the back of the vector
			return edges.size();
		}
		else {
			// reuse the deleted edge
			size_t index = next_index.back();
			next_index.pop_back();
			return index;
		}
	}

	template <typename vertex_type, typename edge_type>
	std::pair<size_t, size_t> Graph<vertex_type, edge_type>::getEdgeIndexByKey(
		const vertex_type& from, const vertex_type& to) const {
		// get the iterator within the map
		typename std::unordered_map<vertex_type, size_t>::const_iterator 
			from_it = key_to_index.find(from), to_it = key_to_index.find(to);

		// the key isn't found
		if (from_it == key_to_index.cend() || to_it == key_to_index.cend()) {
			return std::make_pair<size_t, size_t>(-1, -1);
		}
		// the key is found, then continue to search
		else {
			return getEdgeIndexByIndex(from_it->second, to_it->second);
		}
	}

	template <typename vertex_type, typename edge_type>
	std::pair<size_t, size_t> Graph<vertex_type, edge_type>::getEdgeIndexByIndex(
		size_t from_index, size_t to_index) const {
		// make sure the memory access is safe
		assert(from_index < tail_index.size() && to_index < tail_index.size());

		// search every edge from 'from_index'
		for (int cur_index = tail_index[from_index], prev_index = -1; cur_index != -1;
			prev_index = cur_index, cur_index = edges[cur_index].prev) {
			// if found
			if (edges[cur_index].to == to_index) {
				return std::make_pair(cur_index, prev_index);
			}
		}

		// if not found
		return std::make_pair<size_t, size_t>(-1, -1);
	}

	template <typename vertex_type, typename edge_type>
	size_t Graph<vertex_type, edge_type>::getKeyIndex(const vertex_type& key, bool create) {
		// get the iterator within the map
		typename std::unordered_map<vertex_type, size_t>::iterator key_it = key_to_index.find(key);

		size_t index;
		// the key isn't found
		if (key_it == key_to_index.end()) {
			// if create == false, return -1 directly
			if (!create) return -1;

			// assign this key with a new index
			index = key_to_index.size();

			// add key-index pair
			key_to_index[key] = key_to_index.size();
			index_to_key.push_back(key);
			tail_index.push_back(-1);
			degrees.push_back(0);
		}
		// the key is found, then return its index
		else {
			index = key_it->second;
		}

		// 'key_to_index.size() = index_to_key.size() = tail_index.size() = degrees.size()' is always true
		assert(key_to_index.size() == index_to_key.size() && 
			key_to_index.size() == tail_index.size() && 
			key_to_index.size() == degrees.size());

		return index;
	}

	/****************** Below are public functions ******************/

	template <typename vertex_type, typename edge_type>
	bool Graph<vertex_type, edge_type>::addEdge(const vertex_type& from, 
		const vertex_type& to, const edge_type& value, bool double_side) {
		// get the indexes of both from and to
		size_t index_from = getKeyIndex(from), index_to = getKeyIndex(to);

		// no self-circle allowed
		if (index_from == index_to) return false;

		// if return_value is true, the edge has been added correctly
		bool return_value = false;

		// add edges
		return_value |= addEdgeInner(index_from, index_to, value);
		// if double_side is specified, add the opposite edge
		if (double_side) {
			return_value |= addEdgeInner(index_to, index_from, value);
		}

		return return_value;
	}
	
	template <typename vertex_type, typename edge_type>
	bool Graph<vertex_type, edge_type>::deleteEdge(const vertex_type& from, 
		const vertex_type& to, bool double_side) {
		// get the index of the two vertexes
		size_t from_index = getKeyIndex(from, false), to_index = getKeyIndex(to, false);

		// if any of them is not found
		if (from_index == -1 || to_index == -1) {
			return false;
		}

		// if return_value is true, the edge has been deleted correctly
		bool return_value = false;

		// delete the edge from 'from' to 'to'
		return_value |= deleteEdgeInner(from_index, to_index);
		// delete double-side edges
		if (double_side) {
			return_value |= deleteEdgeInner(to_index, from_index);
		}

		return return_value;
	}

	template <typename vertex_type, typename edge_type>
	void Graph<vertex_type, edge_type>::mergeGraph(const Graph& other_graph) {
		// add edges in the other graph one by one
		for (size_t tail : other_graph.tail_index) {
			// if tail is invalid, continue for the next
			if (tail == -1) {
				continue;
			}

			// all edges from tail
			for (size_t cur_index = tail; cur_index != -1; 
				cur_index = other_graph.edges[cur_index].prev) {
				// add edge into this graph
				this->addEdge(other_graph.index_to_key[other_graph.edges[cur_index].from],
					other_graph.index_to_key[other_graph.edges[cur_index].to], 
					other_graph.edges[cur_index].value, false);
			}
		}
	}

#ifdef DEBUG
	template <typename vertex_type, typename edge_type>
	void Graph<vertex_type, edge_type>::printGraph() const {
		// iterate through all tails
		for (size_t tail : tail_index) {
			// if tail is invalid, continue for the next
			if (tail == -1) {
				continue;
			}

			// all edges from tail
			for (size_t cur_index = tail; cur_index != -1; cur_index = edges[cur_index].prev) {
				// print the details of the edge
				std::cout << "The edge from " << index_to_key[edges[cur_index].from] << " to "
					<< index_to_key[edges[cur_index].to] << " values " << edges[cur_index].value << '\n';
			}
		}

		for (int i = 0; i < degrees.size(); ++i) {
			std::cout << "The vertex " << index_to_key[i] << " has a core number of "
				<< core_numbers[i] << '\n';
		}
	}
#endif // DEBUG

} // namespace graph

namespace graph {

	/************** Below implement Peeling Algorithm ***************/

	template <typename vertex_type, typename edge_type>
	void Graph<vertex_type, edge_type>::peeling() {
		// resize the vector to fit the requirement of capacity
		core_numbers.resize(tail_index.size());

		// create a ListLinearHeap to fetch the node one by one
		utils::ListLinearHeap heap(tail_index.size());

		// 'key_to_index.size() = index_to_key.size() = tail_index.size() = degrees.size()' is always true
		assert(key_to_index.size() == index_to_key.size() &&
			key_to_index.size() == tail_index.size() &&
			key_to_index.size() == degrees.size());

		// add every node into the heap
		for (int i = 0; i < degrees.size(); ++i) {
			heap.addNode(i, degrees[i]);
		}
		
		// apply Peeling Algorithm
		size_t core = 0;
		std::unordered_set<size_t> vertex_set;

		// fetch the top vertex and then modify those adjacent vertexes
		while (!heap.empty()) {
			// the first element of the pair is 'no', while the second is 'degree'
			std::pair<size_t, size_t> top_element = heap.top();
			// pop out the top element
			heap.pop();

			// modify 'core' and update the core_number of the vertex
			if (top_element.second > core) {
				core = top_element.second;
			}
			core_numbers[top_element.first] = core;

			// iterate all adjacent vertex
			for (int cur_index = tail_index[top_element.first]; cur_index != -1;
				cur_index = edges[cur_index].prev) {
				// decrease the degree of the adjacent node by 1
				heap.modifyNode(edges[cur_index].to, heap.inquire(edges[cur_index].to) - 1);
			}
		}
	}

	template <typename vertex_type, typename edge_type>
	Graph<vertex_type, edge_type> Graph<vertex_type, edge_type>::getKCoreGraph(size_t k) const {
		// find all nodes that has a larger core_number than k
		std::unordered_set<size_t> needed_nodes;
		for (int i = 0; i < core_numbers.size(); ++i) {
			if (core_numbers[i] >= k) {
				needed_nodes.insert(i);
			}
		}

		// add all edges between the vertexes above into a graph
		Graph<vertex_type, edge_type> new_graph;
		for (size_t index : needed_nodes) {
			for (size_t cur_index = tail_index[index]; cur_index != -1; 
				cur_index = edges[cur_index].prev) {
				// if the other end of the edge is in the set above, then add it
				if (needed_nodes.find(edges[cur_index].to) != needed_nodes.end()) {
					new_graph.addEdge(index_to_key[index], index_to_key[edges[cur_index].to], 
						edges[cur_index].value, false);
				}
			}
		}

		// here we could return the newly created graph using std::move to decrease copy-cost
		return new_graph;
	}

} // namespace graph
