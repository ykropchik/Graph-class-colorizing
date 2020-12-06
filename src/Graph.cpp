//
// Created by YKROPCHIK on 06.12.2020.
//

#include <string>
#include <iostream>
#include "Graph.h"

GraphNode::GraphNode(int nodeNumber) {
    this->nodeNumber = nodeNumber;
    this->color = -1;
    this->adjacencyList = nullptr;
    this->prevNode = nullptr;
    this->nextNode = nullptr;
}

GraphNode::GraphNode(int nodeNumber, GraphNode* prevNode, GraphNode* nextNode) {
    this->nodeNumber = nodeNumber;
    this->color = -1;
    this->adjacencyList = nullptr;
    this->prevNode = prevNode;
    this->nextNode = nextNode;
}

void GraphNode::addAdjacentNode(GraphNode *node) {
    if (this->adjacencyList == nullptr) {
        this->adjacencyList = new AdjacencyListNode(node);
        return;
    }

    AdjacencyListNode* runner = this->adjacencyList;

    if (runner->next == nullptr && node != runner->node) {
        runner->next = new AdjacencyListNode(node, runner, nullptr);
        return;
    }

    while (runner->next != nullptr) {
        runner = runner->next;
    }

    runner->next = new AdjacencyListNode(node, runner, nullptr);
}

AdjacencyListNode* GraphNode::searchAdjacent(int searchNodeNumber) {
    if (adjacencyList == nullptr) {
        return nullptr;
    }

    AdjacencyListNode* runner = adjacencyList;

    while (runner != nullptr) {
        if (runner->node->nodeNumber == searchNodeNumber) {
            return runner;
        }

        runner = runner->next;
    }

    return nullptr;
}

void GraphNode::removeAdjacentNode(AdjacencyListNode* adjacentNode) {
    if (adjacentNode->prev == nullptr && adjacentNode->next == nullptr) {
        this->adjacencyList = nullptr;
        delete adjacentNode;
        return;
    }

    if (adjacentNode->prev == nullptr) {
        this->adjacencyList = adjacentNode->next;
        adjacentNode->next->prev = nullptr;
        delete adjacentNode;
        return;
    }

    if (adjacentNode->next == nullptr) {
        adjacentNode->prev->next = nullptr;
        delete adjacentNode;
        return;
    }

    adjacentNode->prev->next = adjacentNode->next;
    adjacentNode->next->prev = adjacentNode->prev;
    delete adjacentNode;
}

AdjacencyListNode::AdjacencyListNode(GraphNode *node) {
    this->node = node;
    this->prev = nullptr;
    this->next = nullptr;
}

AdjacencyListNode::AdjacencyListNode(GraphNode* node, AdjacencyListNode* prev, AdjacencyListNode* next) {
    this->node = node;
    this->prev = prev;
    this->next = next;
}

Graph::Graph() {
    this->nodeAdjacencyList = nullptr;
    chromaticNumber = 0;
}

Graph::~Graph() {
    if (nodeAdjacencyList == nullptr) {
        return;
    }

    GraphNode* runner = nodeAdjacencyList;

    while (runner->nextNode != nullptr) {
        runner = runner->nextNode;
    }

    while (runner->prevNode != nullptr) {
        runner = runner->prevNode;
        delete runner->nextNode;
    }

    nodeAdjacencyList = nullptr;
    delete runner;
}

void Graph::resetChromaticNumb() {
    this->chromaticNumber = -1;
}

bool Graph::addNode(int nodeNumber) {
    if (nodeAdjacencyList == nullptr) {
        nodeAdjacencyList = new GraphNode(nodeNumber);
        resetChromaticNumb();
        return true;
    }

    GraphNode* runner = nodeAdjacencyList;

    if (runner->nextNode == nullptr && nodeNumber < runner->nodeNumber) {
        nodeAdjacencyList = new GraphNode(nodeNumber, nullptr, runner);
        resetChromaticNumb();
        return true;
    }

    while (runner->nextNode != nullptr) {
        if (nodeNumber == runner->nodeNumber) {
            return false;
        }

        if (nodeNumber < runner->nextNode->nodeNumber) {
            runner->nextNode = new GraphNode(nodeNumber, runner, runner->nextNode);
            runner->nextNode->nextNode->prevNode = runner->nextNode;
            resetChromaticNumb();
            return true;
        }

        runner = runner->nextNode;
    }

    if (runner->nodeNumber != nodeNumber) {
        runner->nextNode = new GraphNode(nodeNumber, runner, nullptr);
        resetChromaticNumb();
        return true;
    }

    return false;
}

bool Graph::addEdge(int nodeNumberFirst, int nodeNumberSecond) {
    if (searchEdge(nodeNumberFirst, nodeNumberSecond)) {
        return false;
    }

    if(!searchNode(nodeNumberFirst)) {
        addNode(nodeNumberFirst);
    }

    if (!searchNode(nodeNumberSecond)) {
        addNode(nodeNumberSecond);
    }

    GraphNode* firstNode = searchNode(nodeNumberFirst);
    GraphNode* secondNode = searchNode(nodeNumberSecond);

    firstNode->addAdjacentNode(secondNode);
    secondNode->addAdjacentNode(firstNode);

    resetChromaticNumb();
    return true;
}

GraphNode* Graph::searchNode(int nodeNumber) {
    if (nodeAdjacencyList == nullptr) {
        return nullptr;
    }

    GraphNode* runner = nodeAdjacencyList;

    while (runner != nullptr) {
        if (runner->nodeNumber == nodeNumber) {
            return runner;
        }

        runner = runner->nextNode;
    }

    return nullptr;
}

bool Graph::removeNode(int nodeNumber) {
    GraphNode* node = searchNode(nodeNumber);

    if (node != nullptr) {
        if (node->adjacencyList == nullptr) {
            node->prevNode->nextNode = node->nextNode;
            node->nextNode->prevNode = node->prevNode;
            delete node;
            return true;
        }

        AdjacencyListNode* runner = node->adjacencyList;

        while (runner->next != nullptr) {
            runner = runner->next;
        }

        while (runner->prev != nullptr) {
            runner = runner->prev;
            removeEdge(node, runner->next->node);
        }

        removeEdge(node, runner->node);

        if (this->nodeAdjacencyList == node) {
            nodeAdjacencyList = node->nextNode;
        } else {
            if (node->nextNode == nullptr) {
                node->prevNode->nextNode = nullptr;
            } else {
                node->prevNode->nextNode = node->nextNode;
                node->nextNode->prevNode = node->prevNode;
            }
        }

        delete node;
        return true;
    }

    return false;
}

bool Graph::removeEdge(int nodeNumberFirst, int nodeNumberSecond) {
    GraphNode* nodeFirst = searchNode(nodeNumberFirst);
    GraphNode* nodeSecond = searchNode(nodeNumberSecond);

    if (nodeFirst == nullptr || nodeSecond == nullptr) {
        return false;
    }

    removeEdge(nodeFirst, nodeSecond);
}

bool Graph::removeEdge(GraphNode* nodeFirst, GraphNode* nodeSecond) {
    AdjacencyListNode* firstInSecond = nodeSecond->searchAdjacent(nodeFirst->nodeNumber);
    AdjacencyListNode* secondInFirst = nodeFirst->searchAdjacent(nodeSecond->nodeNumber);

    if (firstInSecond == nullptr || secondInFirst == nullptr) {
        return false;
    }

    nodeSecond->removeAdjacentNode(firstInSecond);
    nodeFirst->removeAdjacentNode(secondInFirst);
}

bool Graph::searchEdge(int nodeNumberFirst, int nodeNumberSecond) {
    GraphNode* firstNode = searchNode(nodeNumberFirst);
    GraphNode* secondNode = searchNode(nodeNumberSecond);

    if (firstNode && secondNode) {
        AdjacencyListNode* runner = firstNode->adjacencyList;

        while (runner != nullptr) {
            if(runner->node == secondNode) {
                return true;
            }

            runner = runner->next;
        }
    }

    return false;
}

void Graph::printGraph() {
    if (nodeAdjacencyList == nullptr) {
        std::cout << "null" << std::endl;
    }

    GraphNode* nodeRunner = nodeAdjacencyList;

    while (nodeRunner != nullptr) {
        std::cout << nodeRunner->nodeNumber << "|" << nodeRunner->color << " --";

        AdjacencyListNode* adjacencyNodeRunner = nodeRunner->adjacencyList;

        if (adjacencyNodeRunner != nullptr) {
            while (adjacencyNodeRunner != nullptr) {
                std::cout << "- " << adjacencyNodeRunner->node->nodeNumber << " ";
                adjacencyNodeRunner = adjacencyNodeRunner->next;
            }
        } else {
            std::cout << "- null";
        }

        std::cout << std::endl;
        nodeRunner = nodeRunner->nextNode;
    }
}

void Graph::colorizeGraph() {

}

int Graph::getChromaticNumber() {
    return this->chromaticNumber;
}
