package Dijkstras_algorithm;

import java.util.*;

public class DijkstrasAlgo {

    // Small epsilon value to compare double values.
    private static final double EPS = 1e-6;

    // An edge class to represent a directed edge between two nodes with a certain cost.
    public static class Edge {
        double cost;
        int from, to;

        public Edge(int from, int to, double cost) {
            this.from = from;
            this.to = to;
            this.cost = cost;
        }
    }

    // Node class to track the nodes to visit while running Dijkstra's
    public static class Node {
        int id;
        double value;

        public Node(int id, double value) {
            this.id = id;
            this.value = value;
        }
    }

    // Custom Priority Queue using a binary heap
    public static class MyPriorityQueue {

        private List<Node> heap;
        private Comparator<Node> comparator;

        public MyPriorityQueue(Comparator<Node> comparator) {
            this.heap = new ArrayList<>();
            this.comparator = comparator;
        }

        public void offer(Node node) {
            heap.add(node);
            heapifyUp(heap.size() - 1);
        }

        public Node poll() {
            if (heap.isEmpty()) return null;
            Node root = heap.get(0);
            Node lastNode = heap.remove(heap.size() - 1);
            if (!heap.isEmpty()) {
                heap.set(0, lastNode);
                heapifyDown(0);
            }
            return root;
        }

        public boolean isEmpty() {
            return heap.isEmpty();
        }

        private void heapifyUp(int index) {
            Node node = heap.get(index);
            while (index > 0) {
                int parentIndex = (index - 1) / 2;
                Node parent = heap.get(parentIndex);
                if (comparator.compare(node, parent) >= 0) break;
                heap.set(index, parent);
                index = parentIndex;
            }
            heap.set(index, node);
        }

        private void heapifyDown(int index) {
            int size = heap.size();
            Node node = heap.get(index);
            while (index < size / 2) {
                int leftChildIndex = 2 * index + 1;
                int rightChildIndex = 2 * index + 2;
                int smallestChildIndex = leftChildIndex;

                if (rightChildIndex < size && comparator.compare(heap.get(rightChildIndex), heap.get(leftChildIndex)) < 0) {
                    smallestChildIndex = rightChildIndex;
                }

                if (comparator.compare(heap.get(smallestChildIndex), node) >= 0) break;
                heap.set(index, heap.get(smallestChildIndex));
                index = smallestChildIndex;
            }
            heap.set(index, node);
        }
    }

    private int n;
    private double[] dist;
    private Integer[] prev;
    private List<List<Edge>> graph;

    private Comparator<Node> comparator =
            new Comparator<Node>() {
                @Override
                public int compare(Node node1, Node node2) {
                    if (Math.abs(node1.value - node2.value) < EPS) return 0;
                    return (node1.value - node2.value) > 0 ? +1 : -1;
                }
            };

    public DijkstrasAlgo(int n) {
        this.n = n;
        createEmptyGraph();
    }

    public DijkstrasAlgo(int n, Comparator<Node> comparator) {
        this(n);
        if (comparator == null) throw new IllegalArgumentException("Comparator cannot be null");
        this.comparator = comparator;
    }

    public void addEdge(int from, int to, int cost) {
        graph.get(from).add(new Edge(from, to, cost));
    }

    public List<List<Edge>> getGraph() {
        return graph;
    }

    public List<Integer> reconstructPath(int start, int end) {
        if (end < 0 || end >= n) throw new IllegalArgumentException("Invalid node index");
        if (start < 0 || start >= n) throw new IllegalArgumentException("Invalid node index");
        double dist = dijkstra(start, end);
        List<Integer> path = new ArrayList<>();
        if (dist == Double.POSITIVE_INFINITY) return path;
        for (Integer at = end; at != null; at = prev[at]) path.add(at);
        Collections.reverse(path);
        return path;
    }

    public double dijkstra(int start, int end) {
        // Maintain an array of the minimum distance to each node
        dist = new double[n];
        Arrays.fill(dist, Double.POSITIVE_INFINITY);
        dist[start] = 0;

        // Keep priority queue of the next most promising node to visit.
        MyPriorityQueue pq = new MyPriorityQueue(comparator);
        pq.offer(new Node(start, 0));

        // Array used to track which nodes have already been visited.
        boolean[] visited = new boolean[n];
        prev = new Integer[n];

        while (!pq.isEmpty()) {
            Node node = pq.poll();
            visited[node.id] = true;

            // We already found a better path before we got to processing this node, so we can ignore it.
            if (dist[node.id] < node.value) continue;

            List<Edge> edges = graph.get(node.id);
            for (int i = 0; i < edges.size(); i++) {
                Edge edge = edges.get(i);

                // You cannot get a shorter path by revisiting a node you have already visited before.
                if (visited[edge.to]) continue;

                // Relax edge by updating minimum cost if applicable.
                double newDist = dist[edge.from] + edge.cost;
                if (newDist < dist[edge.to]) {
                    prev[edge.to] = edge.from;
                    dist[edge.to] = newDist;
                    pq.offer(new Node(edge.to, dist[edge.to]));
                }
            }
            // Once we've visited all the nodes spanning from the end node we know we can return the minimum distance value to
            // the end node because it cannot get any better after this point.
            if (node.id == end) return dist[end];
        }
        // End node is unreachable
        return Double.POSITIVE_INFINITY;
    }

    private void createEmptyGraph() {
        graph = new ArrayList<>(n);
        for (int i = 0; i < n; i++) graph.add(new ArrayList<>());
    }
    
    public static void main(String[] args) {
        // Example usage of Dijkstra's algorithm
        DijkstrasAlgo dijkstra = new DijkstrasAlgo(5);

        // Add edges (from, to, cost)
        dijkstra.addEdge(0, 1, 10);
        dijkstra.addEdge(0, 2, 3);
        dijkstra.addEdge(1, 2, 1);
        dijkstra.addEdge(1, 3, 2);
        dijkstra.addEdge(2, 1, 4);
        dijkstra.addEdge(2, 3, 8);
        dijkstra.addEdge(2, 4, 2);
        dijkstra.addEdge(3, 4, 7);
        dijkstra.addEdge(4, 3, 9);

        for (int i = 0; i < 5; i++) {
            // Find the shortest path from node 0 to every other node
            List<Integer> path = dijkstra.reconstructPath(0, i);
            double distance = dijkstra.dijkstra(0, i);

            System.out.print("Shortest path from node 0 to node " + i);
            System.out.println(": " + path);
            System.out.println("Distance: " + distance);
        }
    }
}
