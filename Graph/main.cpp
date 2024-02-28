// main.cpp : include 'main' function, where the program starts executing
//

#include "Graph.h"
#include <iostream>

int main()
{
	// total test number
	size_t total_input_number = 10, total_graph_number = 1;
	std::cin >> total_input_number >> total_graph_number;

	// construct the Graph class
	graph_t* test_graph = new graph_t[total_graph_number];

	while (total_input_number--) {
		// test operator
		char op; int index, other_index;
		std::cin >> op >> index;

		// keys and value
		uint64_t from, to;
		std::string value;

		switch (op) {
		case 'A': case 'a':
			// add edge
			std::cin >> from >> to >> value;
			test_graph[index].addEdge(from, to, value);
			break;

		case 'D': case 'd':
			// delete edge
			std::cin >> from >> to;
			test_graph[index].deleteEdge(from, to);
			break;

		case 'P': case 'p':
			// print all edges
			test_graph[index].printGraph();
			break;

		case 'M': case'm':
			// merge graphs
			std::cin >> other_index;
			test_graph[index].mergeGraph(test_graph[other_index]);
			break;

		default:
			break;
		}
	}

	// release the memory
	delete [] test_graph;
}
