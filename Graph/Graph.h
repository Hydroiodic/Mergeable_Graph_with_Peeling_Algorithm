// Graph.h : include the definition of class Graph
//

#pragma once

#include <cassert>
#include <iostream>
#include <stddef.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define DEBUG

namespace Graph
{
	template <typename vertex_type, typename edge_type>
	struct Edge
	{
		// the index of start node and end node
		size_t from, to;
		// if needed
		edge_type value;

		// the previous edge
		size_t prev = -1;
	};

	// methods and data members of the class Graph defined here
	template <typename vertex_type, typename edge_type>
	class Graph
	{
	private:
		// use hash-method to map vertex_type to their indexes
		std::unordered_map<vertex_type, size_t> key_to_index;
		std::vector<vertex_type> index_to_key;
		size_t getKeyIndex(const vertex_type& key);

		// use hash-method to judge whether an edge exists or not
		std::unordered_set<std::string> edge_set;

		// data members stored using Adjacency List here
		// use a vector to store all edges
		std::vector<Edge<vertex_type, edge_type>> edges;
		std::vector<size_t> tail_index;

		// methods used inside the data structure
		bool addEdgeInner(size_t from, size_t to, const edge_type& value);

		// get the next index to insert
		std::vector<size_t> next_index;
		size_t nextEdgeIndex();

	public:
		// the construction and destruction functions
		Graph();
		~Graph();

		// the basic method of a graph
		bool addEdge(const vertex_type& from, const vertex_type& to, const edge_type& value);
		bool deleteEdge(const vertex_type& from, const vertex_type& to);

		// merge other_graph into the current graph
		void mergeGraph(const Graph& other_graph);

#ifdef DEBUG
		void printGraph() const;
#endif // DEBUG

	};
} // namespace Graph

// definition and implementation of a template class must be within one file
#include "Graph.cpp"

// these types may change to meet our needs
using graph_t = Graph::Graph<uint64_t, std::string>;
