// main.cpp : include 'main' function, where the program starts executing
//

// this definition will ignore the warning of the function "freopen"
#define _CRT_SECURE_NO_WARNINGS

#include "Graph.h"
#include <iostream>
#include <cstdio>

// redirectStdin: to redirect stdin to the input file, return false if open failed
bool redirectStdin(const char* file_name) {
	// whether the file exits or not
	if (FILE* file = fopen(file_name, "r")) {
		// close the opened file
		fclose(file);
		
		// redirect stdin to the file
		freopen(file_name, "r", stdin);

		// success
		return true;
	}
	else {
		// failure
		return false;
	}
}

// addEdge: input "A <graph number> <from> <to> <value>"
void addEdge(graph_t* test_graph, size_t index, 
	size_t from, size_t to, std::string value) {
	if (test_graph[index].addEdge(from, to, value)) {
		std::cout << "Add edges from " << from << " to " << to <<
			" or from " << to << " to " << to << " successfully!\n";
	}
	else {
		std::cout << "Failed to add edges both from " << from << " to " << to <<
			" and from " << to << " to " << to << ".\n";
	}
}

// deleteEdge: input "D <graph number> <from> <to>"
void deleteEdge(graph_t* test_graph, size_t index, size_t from, size_t to) {
	if (test_graph[index].deleteEdge(from, to)) {
		std::cout << "Delete edges from " << from << " to " << to <<
			" or from " << to << " to " << to << " successfully!\n";
	}
	else {
		std::cout << "Failed to delete edges both from " << from << " to " << to <<
			" and from " << to << " to " << to << ".\n";
	}
}

// peeling: input "G <graph number>"
void peeling(graph_t* test_graph, size_t index) {
	test_graph[index].peeling();
	std::cout << "Apply Peeling Algorithm to calculate core_number in graph " << index << ".\n";
}

// mergeGraph: input "M <the merging graph> <the merged graph>"
void mergeGraph(graph_t* test_graph, size_t index, size_t other_index) {
	test_graph[index].mergeGraph(test_graph[other_index]);
	std::cout << "Merge graph " << other_index << " into graph " << index << ".\n";
}

// printGraph: input "P <graph number>"
void printGraph(graph_t* test_graph, size_t index) {
	std::cout << "Below is the information about graph " << index << ":\n";
	test_graph[index].printGraph();
}

// getKCoreGraph: input "S <graph number> <sub-graph number> <k-value>"
void getKCoreGraph(graph_t* test_graph, size_t index, size_t other_index, size_t k) {
	std::cout << "Assign the " << k << "-core sub-graph of graph " 
		<< index << " into graph " << other_index << ".\n";
	test_graph[other_index] = test_graph[index].getKCoreGraph(k);
}

int main(int argc, char* argv[])
{
	// command-line parameters
	if (argc > 1) {
		// whether the file argv[1] exits or not
		if (redirectStdin(argv[1])) {
			std::cout << "Open file \"" << argv[1] << "\" successfully!\n";
		}
		else {
			std::cout << "Failed to open file \"" << argv[1] << "\", use terminal to input instead.\n";
			std::cout << "\tUsage: <executable file> <filename>\n";
		}
	}

	// total test number
	size_t total_input_number = 10, total_graph_number = 1;
	std::cin >> total_input_number >> total_graph_number;

	// construct the Graph class
	graph_t* test_graph = new graph_t[total_graph_number];

	while (total_input_number--) {
		// test operator
		char op; int index, other_index;
		std::cin >> op >> index;

		// if the input content is incorrect
		if (std::cin.bad() || index >= total_input_number) {
			std::cin.ignore(-1);
			std::cin.clear();
			std::cout << "The graph no is incorrect input!\n";
			continue;
		}

		// keys and value
		uint64_t from, to, k;
		std::string value;

		switch (op) {
		case 'A': case 'a':
			// add edge
			std::cin >> from >> to >> value;
			addEdge(test_graph, index, from, to, value);
			break;

		case 'D': case 'd':
			// delete edge
			std::cin >> from >> to;
			deleteEdge(test_graph, index, from, to);
			break;

		case 'G': case 'g':
			// apply Peeling Algorithm to calculate core_numbers
			peeling(test_graph, index);
			break;

		case 'M': case'm':
			// merge graphs
			std::cin >> other_index;
			mergeGraph(test_graph, index, other_index);
			break;

		case 'P': case 'p':
			// print all edges and the core_number of all vertexes
			printGraph(test_graph, index);
			break;

		case 'Q': case 'q':
			// the inputting process ends
			goto End;

		case 'S': case 's':
			// assign the k-core sub-graph of graph 'index' to 'other_index'
			// ATTENTION! please use "G" to calculate first and then use this operation
			std::cin >> other_index >> k;
			getKCoreGraph(test_graph, index, other_index, k);
			break;

		default:
			break;
		}
	}

End:
	// release the memory
	delete [] test_graph;
}
