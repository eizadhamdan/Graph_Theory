#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "ford_fulkerson_algo.h"

int main()
{
    bool runProgram = true;

    while (runProgram)
    {
        int choice;
        std::cout << std::endl
                  << "--- Maximum Flow Calculation using Ford-Fulkerson Algorithm ---" << std::endl;
        std::cout << "1. Enter graph data and calculate maximum flow" << std::endl;
        std::cout << "2. Exit program" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
        {
            std::cout << "Enter the number of vertices in the graph: ";
            int vertices;
            std::cin >> vertices;
            std::cout << "Enter the number of edges in the graph: ";
            int edges;
            std::cin >> edges;

            std::unordered_map<int, std::unordered_map<int, int>> graph;

            std::cout << "Enter the edges and capacities (u v capacity):" << std::endl;
            for (int i = 0; i < edges; i++)
            {
                int u, v, capacity;
                std::cin >> u >> v >> capacity;
                graph[u][v] = capacity;
            }

            int source, sink;
            std::cout << "Enter the source node: ";
            std::cin >> source;
            std::cout << "Enter the sink node: ";
            std::cin >> sink;

            int maxFlow = fordFulkerson(graph, source, sink, vertices);
            std::cout << "The maximum possible flow is: " << maxFlow << std::endl;
            break;
        }
        case 2:
            runProgram = false;
            std::cout << "Exiting the program..." << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}