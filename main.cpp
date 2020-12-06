#include <iostream>
#include "src/Graph.h"

int main() {
    Graph graph = Graph();
    graph.addNode(3);
    graph.addNode(1);
    graph.addEdge(1, 3);
    graph.addEdge(1, 2);
    graph.addEdge(1, 4);
    graph.addEdge(1, 5);
    graph.addEdge(4, 5);
    graph.addEdge(3, 5);
    graph.addEdge(3, 4);
    graph.removeNode(1);

    graph.printGraph();
    return 0;
}
