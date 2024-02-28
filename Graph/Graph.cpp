// Graph.cpp : include implementations of all methods of the class Graph
//

// the sentence below must be used because this cpp file is included in the header file
#pragma once

#include "Graph.h"

namespace Graph {

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
	size_t Graph<vertex_type, edge_type>::getKeyIndex(const vertex_type& key) {
		// get the iterator within the map
		typename std::unordered_map<vertex_type, size_t>::iterator key_it = key_to_index.find(key);

		size_t index;
		// the key isn't found
		if (key_it == key_to_index.end()) {
			// assign this key with a new index
			index = key_to_index.size();

			// add key-index pair
			key_to_index[key] = key_to_index.size();
			index_to_key.push_back(key);
			tail_index.push_back(-1);
		}
		// the key is found, then return its index
		else {
			index = key_it->second;
		}

		// 'key_to_index.size() = index_to_key.size() = tail_index.size()' is always true
		assert(key_to_index.size() == index_to_key.size() && 
			key_to_index.size() == tail_index.size());

		return index;
	}

	/****************** Below are public functions ******************/

	template <typename vertex_type, typename edge_type>
	bool Graph<vertex_type, edge_type>::addEdge(const vertex_type& from, 
		const vertex_type& to, const edge_type& value, bool double_side) {
		// get the indexes of both from and to
		size_t index_from = getKeyIndex(from), index_to = getKeyIndex(to);

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
	bool Graph<vertex_type, edge_type>::deleteEdge(const vertex_type& from, const vertex_type& to) {
		// get the index of the edge
		std::pair<size_t, size_t> edge_index = getEdgeIndexByKey(from, to);

		// if not found
		if (edge_index.first == -1) {
			return false;
		}
		else if (edge_index.second == -1) {
			// the index of 'from'
			size_t from_index = edges[edge_index.first].from;

			// "delete" the edge
			tail_index[from_index] = edges[edge_index.first].prev;
			next_index.push_back(edge_index.first);

			// delete successfully
			return true;
		}
		else {
			// "delete" the edge
			edges[edge_index.second].prev = edges[edge_index.first].prev;
			next_index.push_back(edge_index.first);

			// delete successfully
			return true;
		}
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
			for (size_t cur_index = tail; cur_index != -1;
				cur_index = edges[cur_index].prev) {
				// print the details of the edge
				std::cout << "The edge from " << index_to_key[edges[cur_index].from] << " to "
					<< index_to_key[edges[cur_index].to] << " values " << edges[cur_index].value << '\n';
			}
		}
	}
#endif // DEBUG

} // namespace Graph
