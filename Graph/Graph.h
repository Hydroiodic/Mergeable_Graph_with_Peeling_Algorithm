// Graph.h : include the definition of class Graph
//

#pragma once

#include "ListLinearHeap.h"
#include <cassert>
#include <iostream>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace graph
{
	// 'struct Edge' defines the structure to store an edge
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
		size_t getKeyIndex(const vertex_type& key, bool create = true);

		// use hash-method to judge whether an edge exists or not
		std::unordered_set<std::string> edge_set;

		// data members stored using Adjacency List here
		// use a vector to store all edges
		std::vector<Edge<vertex_type, edge_type>> edges;
		std::vector<size_t> tail_index;

		// add or delete an edge from 'from' to 'to' within the vector 'edges'
		bool addEdgeInner(size_t from, size_t to, const edge_type& value);
		bool deleteEdgeInner(size_t from, size_t to);

		// get the next index to insert
		std::vector<size_t> next_index;
		size_t nextEdgeIndex();

		// return the index of the edge from 'from' to 'to'
		std::pair<size_t, size_t> getEdgeIndexByKey(const vertex_type& from, const vertex_type& to) const;
		std::pair<size_t, size_t> getEdgeIndexByIndex(size_t from_index, size_t to_index) const;

	public:
		// the construction and destruction functions
		Graph();
		~Graph();

		// the basic method of a graph
		bool addEdge(const vertex_type& from, const vertex_type& to, 
			const edge_type& value, bool double_side = true);
		bool deleteEdge(const vertex_type& from, const vertex_type& to, 
			bool double_side = true);

		// merge other_graph into the current graph
		void mergeGraph(const Graph& other_graph);

		// print out all edges of the graph
		void printGraph() const;

		/************** Below implement Peeling Algorithm **************/

	private:
		// use a vector to store the core number of each vertex
		std::vector<size_t> core_numbers;

		// use a vector to store the degrees of each vertex
		std::vector<size_t> degrees;

	public:
		// use Peeling Algorithm to calculate core_number of all vertexes
		void peeling();

		// use the calculated core_numbers to find the k-core sub-graph
		Graph getKCoreGraph(size_t k) const;
	};
} // namespace graph

// definition and implementation of a template class must be within one file
#include "Graph.cpp"

// these types may change to meet our needs
// ATTENTION! the first parameter-type must be hashable!
using vertex_type = int;
using edge_type = std::string;
using graph_t = graph::Graph<vertex_type, edge_type>;
