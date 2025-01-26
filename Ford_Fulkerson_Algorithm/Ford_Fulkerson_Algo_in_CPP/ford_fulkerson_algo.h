#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include <algorithm>
#include "dfs.h"

int fordFulkerson(std::unordered_map<int, std::unordered_map<int, int>> &graph, int source, int sink, int V)
{
    std::unordered_map<int, std::unordered_map<int, int>> residualGraph = graph;
    std::vector<int> parent(V);
    int maxFlow = 0;
    int iteration = 1;

    // Augment the flow while there is a path from source to sink
    while (dfs(residualGraph, source, sink, parent))
    {
        std::cout << "Iteration " << iteration++ << ":" << std::endl;

        // Find the maximum flow through the path found
        int pathFlow = INT_MAX;
        std::vector<int> path; // To store the augmenting path for debugging
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residualGraph[u][v]);
            path.push_back(v);
        }
        path.push_back(source);

        // Reverse the path vector for proper order (source to sink)
        std::reverse(path.begin(), path.end());

        // Print the augmenting path and its flow
        std::cout << "  Augmenting Path: ";
        for (int i = 0; i < path.size(); i++)
        {
            std::cout << path[i];
            if (i != path.size() - 1)
                std::cout << " -> ";
        }
        std::cout << std::endl
                  << "  Path Flow (Bottleneck Capacity): " << pathFlow << std::endl;

        // Update residual capacities of the edges and reverse edges along the path
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Print the updated residual graph
        std::cout << "  Updated Residual Graph:" << std::endl;
        for (auto uIt = residualGraph.begin(); uIt != residualGraph.end(); ++uIt)
        {
            int u = uIt->first;
            for (auto vIt = uIt->second.begin(); vIt != uIt->second.end(); ++vIt)
            {
                int v = vIt->first;
                int capacity = vIt->second;
                std::cout << "    Edge " << u << " -> " << v << " : " << capacity << std::endl;
            }
        }

        // Add path flow to the overall flow
        maxFlow += pathFlow;
        std::cout << "  Cumulative Maximum Flow: " << maxFlow << std::endl
                  << std::endl;
    }

    return maxFlow;
}
