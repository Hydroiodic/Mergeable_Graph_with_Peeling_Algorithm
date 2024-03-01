// main.cpp : include 'main' function, where the program starts executing
//

// this definition will ignore the warning of the function "freopen"
#define _CRT_SECURE_NO_WARNINGS

#include "Graph.h"
#include <iostream>
#include <cstdio>

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
			if (!test_graph[index].deleteEdge(from, to)) {
				std::cout << "delete failed\n";
			}
			break;

#ifdef DEBUG
		case 'P': case 'p':
			// print all edges and the core_number of all vertexes
			test_graph[index].peeling();
			test_graph[index].printGraph();
			break;
#endif // DEBUG

		case 'M': case'm':
			// merge graphs
			std::cin >> other_index;
			test_graph[index].mergeGraph(test_graph[other_index]);
			break;

		case 'Q': case 'q':
			goto End;

		default:
			break;
		}
	}

End:
	// release the memory
	delete [] test_graph;
}
