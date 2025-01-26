#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

bool dfs(const std::unordered_map<int, std::unordered_map<int, int>> &residualGraph, int source, int sink, std::vector<int> &parent)
{
    int V = parent.size();
    std::vector<bool> visited(V, false);
    std::stack<int> s;

    s.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!s.empty())
    {
        int u = s.top();
        s.pop();

        for (auto it = residualGraph.at(u).begin(); it != residualGraph.at(u).end(); ++it)
        {
            int v = it->first;
            int capacity = it->second;

            if (!visited[v] && capacity > 0)
            {
                s.push(v);
                parent[v] = u;
                visited[v] = true;

                if (v == sink)
                    return true;
            }
        }
    }
    return false;
}
