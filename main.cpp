#include <iostream>
#include "src/Graph.h"

int main() {

    Graph graph = Graph();

//    graph.addEdge(1, 4);
//    graph.addEdge(1, 6);
//    graph.addEdge(4, 2);
//    graph.addEdge(4, 6);
//    graph.addEdge(5, 6);
//    graph.addEdge(2, 5);
//    graph.addEdge(2, 3);
//    graph.addEdge(3, 5);
//    graph.addEdge(3, 6);
//    graph.traversing();
//    std::cout << std::endl;

//    graph.printGraph();
//
//    for (int i = 1; i < 10; i++) {
//        for (int j = 1; j < 10; j+=2) {
//            graph.addEdge(i, j);
//        }
//    }
//
//   //graph.printGraph();
//
//    for (int i = 1; i < 10; i+=2) {
//        graph.removeNode(i);
//    }
//
//    graph.colorizeGraph();
//    graph.printGraph();
//    graph.traversing();

    /** ----- Первый граф ----- **/
    Graph graph1 = Graph();
    graph1.addEdge(1, 4);
    graph1.addEdge(1, 6);
    graph1.addEdge(4, 2);
    graph1.addEdge(4, 6);
    graph1.addEdge(2, 3);
    graph1.addEdge(2, 5);
    graph1.addEdge(3, 5);
    graph1.addEdge(3, 6);
    graph1.addEdge(5, 6);
    graph1.colorizeGraph();
    graph1.printGraph();
    std::cout << std::endl;

    /*Graph graph1 = Graph();
    graph1.addEdge(1, 3);
    graph1.addEdge(3, 4);
    graph1.addEdge(4, 2);
    graph1.addEdge(2, 7);
    graph1.addEdge(7, 6);
    graph1.addEdge(6, 5);
    graph1.addEdge(5, 8);
    graph1.addEdge(8, 1);
    graph1.colorizeGraph();
    graph1.printGraph();
    std::cout << std::endl;*/

    /** ----- Второй граф ----- **/
    Graph graph2 = Graph();
    graph2.addEdge(1, 6);
    graph2.addEdge(1, 4);
    graph2.addEdge(6, 4);
    graph2.addEdge(6, 5);
    graph2.addEdge(6, 3);
    graph2.addEdge(4, 2);
    graph2.addEdge(2, 3);
    graph2.addEdge(2, 5);
    graph2.addEdge(5, 3);
    graph2.colorizeGraph();
    graph2.printGraph();
    std::cout << std::endl;

    /** ----- Третий граф ----- **/
    Graph graph3 = Graph();
    graph3.addEdge(1, 4);
    graph3.addEdge(1, 5);
    graph3.addEdge(4, 6);
    graph3.addEdge(4, 5);
    graph3.addEdge(4, 3);
    graph3.addEdge(5, 2);
    graph3.addEdge(5, 3);
    graph3.addEdge(6, 3);
    graph3.addEdge(2, 3);
    graph3.colorizeGraph();
    graph3.printGraph();
    std::cout << std::endl;

    return 0;
}
