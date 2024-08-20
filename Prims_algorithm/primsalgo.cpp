#include <iostream>
#include <vector>
#include <algorithm>

struct Edge
{
    int from, to, cost;

    Edge(int from, int to, int cost) : from(from), to(to), cost(cost) {}

    bool operator<(const Edge &other) const
    {
        return cost > other.cost;
    }
};

class PriorityQueue
{
private:
    std::vector<Edge> heap;

    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;
            if (heap[index] < heap[parent])
            {
                std::swap(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    void heapifyDown(int index)
    {
        int size = heap.size();
        while (index < size)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest])
            {
                smallest = left;
            }
            if (right < size && heap[right] < heap[smallest])
            {
                smallest = right;
            }
            if (smallest != index)
            {
                std::swap(heap[index], heap[smallest]);
                index = smallest;
            }
            else
            {
                break;
            }
        }
    }

public:
    void push(const Edge &edge)
    {
        heap.push_back(edge);
        heapifyUp(heap.size() - 1);
    }

    Edge top() const
    {
        if (heap.empty())
            throw std::runtime_error("Priority queue is empty");
        return heap[0];
    }

    void pop()
    {
        if (heap.empty())
            throw std::runtime_error("Priority queue is empty");
        std::swap(heap[0], heap.back());
        heap.pop_back();
        heapifyDown(0);
    }

    bool empty() const
    {
        return heap.empty();
    }

    size_t size() const
    {
        return heap.size();
    }
};

class PrimsAlgo
{
private:
    int n;
    std::vector<std::vector<Edge>> graph;
    bool solved = false;
    bool mstExists = false;
    std::vector<bool> visited;
    PriorityQueue pq;
    long long minCostSum = 0;
    std::vector<Edge> mstEdges;

    void addEdges(int nodeIndex)
    {
        visited[nodeIndex] = true;
        for (const Edge &e : graph[nodeIndex])
        {
            if (!visited[e.to])
            {
                pq.push(e);
            }
        }
    }

    void solve()
    {
        if (solved)
            return;
        solved = true;

        int m = n - 1, edgeCount = 0;
        visited.assign(n, false);
        mstEdges.clear();
        mstEdges.reserve(m);

        addEdges(0); // Start from node 0

        while (!pq.empty() && edgeCount != m)
        {
            Edge edge = pq.top();
            pq.pop();

            int nodeIndex = edge.to;
            if (visited[nodeIndex])
                continue;

            mstEdges.push_back(edge);
            minCostSum += edge.cost;
            edgeCount++;

            addEdges(nodeIndex);
        }

        mstExists = (edgeCount == m);
    }

public:
    PrimsAlgo(const std::vector<std::vector<Edge>> &graph)
        : graph(graph), n(graph.size()) {}

    std::vector<Edge> getMst()
    {
        solve();
        return mstExists ? mstEdges : std::vector<Edge>();
    }

    long long getMstCost()
    {
        solve();
        return mstExists ? minCostSum : -1;
    }
};

std::vector<std::vector<Edge>> createEmptyGraph(int n);
void addDirectedEdge(std::vector<std::vector<Edge>> &g, int from, int to, int cost);
void addUndirectedEdge(std::vector<std::vector<Edge>> &g, int from, int to, int cost);
void example();

int main()
{
    example();
    return 0;
}

std::vector<std::vector<Edge>> createEmptyGraph(int n)
{
    return std::vector<std::vector<Edge>>(n);
}

void addDirectedEdge(std::vector<std::vector<Edge>> &g, int from, int to, int cost)
{
    g[from].emplace_back(from, to, cost);
}

void addUndirectedEdge(std::vector<std::vector<Edge>> &g, int from, int to, int cost)
{
    addDirectedEdge(g, from, to, cost);
    addDirectedEdge(g, to, from, cost);
}

void example()
{
    int nodes = 10;
    // std::cout << "Find the Minimum Spanning Tree of a graph using Prim's Algorithm." << std::endl;
    
    // std::cout << "Enter the number of nodes in the graph: ";
    // int nodes;
    // std::cin >> nodes;
    
    auto g = createEmptyGraph(nodes);

    // std::cout << "Enter the number of edges in the graph: ";
    // int edges;
    // std::cin >> edges;

    // for (int i = 0; i < edges; i++) {
    //     std::cout << "Enter the 'from' node: ";
    //     int from;
    //     std::cin >> from;
    //     std::cout << "Enter the 'to' node: ";
    //     int to;
    //     std::cin >> to;
    //     std::cout << "Enter the edge weight: ";
    //     int cost;
    //     std::cin >> cost;
        
    //     addUndirectedEdge(g, from, to, cost);
    // }

    addUndirectedEdge(g, 0, 1, 5);
    addUndirectedEdge(g, 1, 2, 4);
    addUndirectedEdge(g, 2, 9, 2);
    addUndirectedEdge(g, 0, 4, 1);
    addUndirectedEdge(g, 0, 3, 4);
    addUndirectedEdge(g, 1, 3, 2);
    addUndirectedEdge(g, 2, 7, 4);
    addUndirectedEdge(g, 2, 8, 1);
    addUndirectedEdge(g, 9, 8, 0);
    addUndirectedEdge(g, 4, 5, 1);
    addUndirectedEdge(g, 5, 6, 7);
    addUndirectedEdge(g, 6, 8, 4);
    addUndirectedEdge(g, 4, 3, 2);
    addUndirectedEdge(g, 5, 3, 5);
    addUndirectedEdge(g, 3, 6, 11);
    addUndirectedEdge(g, 6, 7, 1);
    addUndirectedEdge(g, 3, 7, 2);
    addUndirectedEdge(g, 7, 8, 6);

    PrimsAlgo solver(g);
    long long cost = solver.getMstCost();

    if (cost == -1)
    {
        std::cout << "No MST exists" << std::endl;
    }
    else
    {
        std::cout << "MST cost: " << cost << std::endl;
        for (const Edge &e : solver.getMst())
        {
            std::cout << "from: " << e.from << ", to: " << e.to << ", cost: " << e.cost << std::endl;
        }
    }
}
