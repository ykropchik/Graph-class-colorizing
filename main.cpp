#include <iostream>
#include "src/Graph.h"

int main() {

    Graph graph = Graph();

    graph.addEdge(1, 4);
    graph.addEdge(1, 6);
    graph.addEdge(4, 2);
    graph.addEdge(4, 6);
    graph.addEdge(5, 6);
    graph.addEdge(2, 5);
    graph.addEdge(2, 3);
    graph.addEdge(3, 5);
    graph.addEdge(3, 6);
    graph.traversing();
    std::cout << std::endl;
    graph.colorizeGraph();

    /*graph.addEdge(1, 2);
    graph.addEdge(1, 5);
    graph.addEdge(1, 6);
    graph.addEdge(2, 3);
    graph.addEdge(2, 7);
    graph.addEdge(3, 4);
    graph.addEdge(3, 8);
    graph.addEdge(4, 5);
    graph.addEdge(4, 9);
    graph.addEdge(5, 10);
    graph.addEdge(6, 8);
    graph.addEdge(6, 9);
    graph.addEdge(7, 9);
    graph.addEdge(7, 10);
    graph.addEdge(8, 10);
    graph.colorizeGraph();*/

    /*for (int i = 1; i < 10; i++) {
        graph.addEdge(i, i*2 + 1);
    }

    graph.printGraph();
    std::cout << std::endl;

    for (int i = 1; i < 8; i++) {
        graph.removeNode(i);
    }

    graph.printGraph();*/

    return 0;
}
