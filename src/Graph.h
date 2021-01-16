//
// Created by YKROPCHIK on 06.12.2020.
//

enum NodeMark {
    NODE_NOT_MARKED,
    NODE_MARKED_NOT_PASSED,
    NODE_PASSED
};

enum EdgeMark {
    EDGE_PASSED,
    EDGE_NOT_PASSED
};

struct GraphNode;
struct AdjacencyListNode;

struct NodeQueue {
    GraphNode* node;
    NodeQueue* nextNode;

    void addNode(GraphNode* node);
};

struct GraphNode {
    int color;
    int nodeNumber;
    AdjacencyListNode* adjacencyList;
    GraphNode* prevNode;
    GraphNode* nextNode;
    NodeMark mark;


    GraphNode(int nodeNumber);
    GraphNode(int nodeNumber, GraphNode* prevNode, GraphNode* nextNode);
    void addAdjacentNode(GraphNode* node);
    void removeAdjacentNode(AdjacencyListNode* adjacentNode);
    AdjacencyListNode* searchAdjacent(int nodeNumber);
    int getMinColor(int color = -1);
};

struct AdjacencyListNode {
    GraphNode* node;
    AdjacencyListNode* prev;
    AdjacencyListNode* next;
    EdgeMark mark;

    AdjacencyListNode(GraphNode* node);
    AdjacencyListNode(GraphNode* node, AdjacencyListNode* prev, AdjacencyListNode* next);
};

class Graph {
private:
    GraphNode* nodeAdjacencyList;
    int colorCount;

    int getColorCount();
    void recolorizeNode(GraphNode* node);
    void colorizeNode(GraphNode* node);
    void colorizeNodes(GraphNode* node);
    void recolorizeAdjacency(GraphNode* node);
    GraphNode* getMinNodeWithColor(int color);

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
    void traversing();
};