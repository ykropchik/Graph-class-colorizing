//
// Created by YKROPCHIK on 06.12.2020.
//

struct GraphNode;
struct AdjacencyListNode;

struct GraphNode {
    int color;
    int nodeNumber;
    AdjacencyListNode* adjacencyList;
    GraphNode* prevNode;
    GraphNode* nextNode;

    GraphNode(int nodeNumber);
    GraphNode(int nodeNumber, GraphNode* prevNode, GraphNode* nextNode);
    void addAdjacentNode(GraphNode* node);
    AdjacencyListNode* searchAdjacent(int nodeNumber);
    void removeAdjacentNode(AdjacencyListNode* adjacentNode);
};

struct AdjacencyListNode {
    AdjacencyListNode* prev;
    AdjacencyListNode* next;
    GraphNode* node;

    AdjacencyListNode(GraphNode* node);
    AdjacencyListNode(GraphNode* node, AdjacencyListNode* prev, AdjacencyListNode* next);
};

class Graph {
private:
    GraphNode* nodeAdjacencyList;
    int chromaticNumber;

    void resetChromaticNumb();

public:
    Graph();
    ~Graph();
    bool addNode(int nodeNumber);
    bool addEdge(int nodeNumberFirst, int nodeNumberSecond);
    bool removeNode(int nodeNumber);
    bool removeEdge(int nodeNumberFirst, int nodeNumberSecond);
    bool removeEdge(GraphNode* nodeFirst, GraphNode* nodeSecond);
    GraphNode* searchNode(int nodeNumber);
    bool searchEdge(int nodeNumberFirst, int nodeNumberSecond);
    void printGraph();
    void colorizeGraph();
    int getChromaticNumber();
};