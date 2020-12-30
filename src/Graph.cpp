//
// Created by YKROPCHIK on 06.12.2020.
//

#include <string>
#include <iostream>
#include "Graph.h"

void NodeQueue::addNode(GraphNode *node) {
    NodeQueue* runner = this;

    while (runner->nextNode != nullptr) {
        runner = runner->nextNode;
    }

    runner->nextNode = new NodeQueue{node};
}

GraphNode::GraphNode(int nodeNumber) {
    this->nodeNumber = nodeNumber;
    this->color = -1;
    this->adjacencyList = nullptr;
    this->prevNode = nullptr;
    this->nextNode = nullptr;
    this->mark = NODE_NOT_MARKED;
}

GraphNode::GraphNode(int nodeNumber, GraphNode* prevNode, GraphNode* nextNode) {
    this->nodeNumber = nodeNumber;
    this->color = -1;
    this->adjacencyList = nullptr;
    this->prevNode = prevNode;
    this->nextNode = nextNode;
    this->mark = NODE_NOT_MARKED;
}

void GraphNode::addAdjacentNode(GraphNode *node) {
    if (this->adjacencyList == nullptr) {
        this->adjacencyList = new AdjacencyListNode(node);
        return;
    }

    AdjacencyListNode* runner = this->adjacencyList;

    if (runner->node->nodeNumber > node->nodeNumber) {
        this->adjacencyList = new AdjacencyListNode(node, nullptr, runner);
        runner->prev = this->adjacencyList;
        return;
    }

    while (runner->next != nullptr) {
        if (runner->next->node->nodeNumber > node->nodeNumber) {
            runner->next = new AdjacencyListNode(node, runner, runner->next);
            runner->next->next->prev = runner->next;
            return;
        }

        runner = runner->next;
    }

    runner->next = new AdjacencyListNode(node, runner, nullptr);
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

int GraphNode::getMinColor() {
    if (this->adjacencyList == nullptr) {
        return 1;
    }

    AdjacencyListNode* adjacencyRunner = this->adjacencyList;

    int adjacencyCount = 0;

    while (adjacencyRunner != nullptr) {
        adjacencyCount++;
        adjacencyRunner = adjacencyRunner->next;
    }

    bool* busyColors = new bool[adjacencyCount + 1];

    for(int i = 0; i < adjacencyCount + 1; i++) {
        busyColors[i] = false;
    }

    adjacencyRunner = this->adjacencyList;

    while (adjacencyRunner != nullptr) {
        if (adjacencyRunner->node->color != -1) {
           busyColors[adjacencyRunner->node->color - 1] = true;
        }

        adjacencyRunner = adjacencyRunner->next;
    }

    for (int i = 0; i < adjacencyCount + 1; ++i) {
        if (!busyColors[i]) {
            delete[] busyColors;
            return (i + 1);
        }
    }

    delete[] busyColors;
    return adjacencyCount;
}

bool GraphNode::checkColor(int checkColor) {
    if (this->adjacencyList == nullptr) {
        return true;
    }

    AdjacencyListNode* runner = this->adjacencyList;

    while (runner != nullptr) {
        if (runner->node->color == checkColor && runner->node->nodeNumber < this->nodeNumber) {
            return false;
        }

        runner = runner->next;
    }

    return true;
}

AdjacencyListNode::AdjacencyListNode(GraphNode *node) {
    this->node = node;
    this->prev = nullptr;
    this->next = nullptr;
    this->mark = EDGE_NOT_PASSED;
}

AdjacencyListNode::AdjacencyListNode(GraphNode* node, AdjacencyListNode* prev, AdjacencyListNode* next) {
    this->node = node;
    this->prev = prev;
    this->next = next;
    this->mark = EDGE_NOT_PASSED;
}

Graph::Graph() {
    this->nodeAdjacencyList = nullptr;
}

Graph::~Graph() {
    if (nodeAdjacencyList == nullptr) {
        return;
    }

    while (this->nodeAdjacencyList != nullptr) {
        this->removeNode(this->nodeAdjacencyList->nodeNumber);
    }

    /*GraphNode* runner = nodeAdjacencyList;

    while (runner->nextNode != nullptr) {
        runner = runner->nextNode;
    }

    while (runner->prevNode != nullptr) {
        runner = runner->prevNode;
        this->removeNode(runner->nextNode->nodeNumber);
    }

    nodeAdjacencyList = nullptr;
    delete runner;*/
}

int Graph::getColorCount() {
    GraphNode* runner = this->nodeAdjacencyList;
    int colorCount = 0;

    while (runner != nullptr) {
        if (runner->color > colorCount) {
            colorCount = runner->color;
        }

        runner = runner->nextNode;
    }

    return colorCount;
}

void Graph::colorizeNodes(GraphNode* node) {

    if (node == nullptr) {
        return;
    }

    GraphNode* runner = node->nextNode;

    while (runner != nullptr) {
        runner->color = runner->getMinColor();
        runner = runner->nextNode;
    }


}

void Graph::reColorizeAdjacency(GraphNode* node, unsigned int colorCount) {

    if (node->prevNode == nullptr && node->color + 1 == colorCount) {
        colorizeNodes(node->nextNode);
        return;
    }

    if (node->color + 1 >= colorCount || !node->checkColor(node->color + 1)) {
        if (node->adjacencyList != nullptr) {
            node->color = -1;

            AdjacencyListNode* runner = node->adjacencyList;

            while (runner->next != nullptr) {
                runner = runner->next;
            }

            while (runner->node->color == -1) {
                runner = runner->prev;
            }

            reColorizeAdjacency(runner->node, colorCount);
        }
    } else {
        node->color++;
        colorizeNodes(node);
    }
}

GraphNode* Graph::getMinNodeWithColor(int color) {
    GraphNode* runner = this->nodeAdjacencyList;

    while (runner->nextNode != nullptr) {
        runner = runner->nextNode;
    }

    GraphNode* maxNode = runner;

    while (runner != nullptr) {
        if (runner->color == color && runner->nodeNumber < maxNode->nodeNumber) {
            maxNode = runner;
        }

        runner = runner->prevNode;
    }

    return maxNode;
}

bool Graph::addNode(int nodeNumber) {
    if (nodeAdjacencyList == nullptr) {
        nodeAdjacencyList = new GraphNode(nodeNumber);
        return true;
    }

    GraphNode* runner = nodeAdjacencyList;

    if (nodeNumber < runner->nodeNumber) {
        nodeAdjacencyList = new GraphNode(nodeNumber, nullptr, runner);
        return true;
    }

    while (runner->nextNode != nullptr) {
        if (nodeNumber == runner->nodeNumber) {
            return false;
        }

        if (nodeNumber < runner->nextNode->nodeNumber) {
            runner->nextNode = new GraphNode(nodeNumber, runner, runner->nextNode);
            runner->nextNode->nextNode->prevNode = runner->nextNode;
            return true;
        }

        runner = runner->nextNode;
    }

    if (runner->nodeNumber != nodeNumber) {
        runner->nextNode = new GraphNode(nodeNumber, runner, nullptr);
        return true;
    }

    return false;
}

bool Graph::addEdge(int nodeNumberFirst, int nodeNumberSecond) {
    if (nodeNumberFirst == nodeNumberSecond) {
        return false;
    }

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
            if (node->prevNode != nullptr) {
                node->prevNode->nextNode = node->nextNode;
                this->nodeAdjacencyList = node->nextNode;
            }

            if (node->nextNode != nullptr) {
                node->nextNode->prevNode = node->prevNode;
            }

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

/*unsigned int Graph::getNodeCount() {
    if (nodeAdjacencyList == nullptr) {
        return 0;
    }

    GraphNode* runner = nodeAdjacencyList;
    unsigned int counter = 0;

    while(runner != nullptr) {
        counter++;
        runner = runner->nextNode;
    }

    return counter;
}*/

void Graph::colorizeGraph() {
    if (nodeAdjacencyList == nullptr) {
        return;
    }

    this->nodeAdjacencyList->color = 1;
    colorizeNodes(this->nodeAdjacencyList);
    int colorCount = this->getColorCount();

    while (this->getColorCount() >= colorCount && this->nodeAdjacencyList->color + 1 != colorCount) {
        GraphNode* minNodeWithMaxColor = getMinNodeWithColor(colorCount);
//        printGraph();
//        std::cout << std::endl;
        reColorizeAdjacency(minNodeWithMaxColor, colorCount);
    }
}

void Graph::traversing() {
    if (this->nodeAdjacencyList == nullptr || this->nodeAdjacencyList->adjacencyList == nullptr) {
        return;
    }

    auto* queue = new NodeQueue{this->nodeAdjacencyList};
    NodeQueue* queueRunner = queue;
    AdjacencyListNode* runner;
    GraphNode* edgeEnd;
    this->nodeAdjacencyList->mark = NODE_MARKED_NOT_PASSED;

    /*while (queueRunner != nullptr) {
        runner = queueRunner->node->adjacencyList;
        while (runner != nullptr) {
            runner->mark = EDGE_PASSED;
            edgeEnd = runner->node;

            if (edgeEnd->mark == NODE_NOT_MARKED) {
                edgeEnd->mark = NODE_MARKED_NOT_PASSED;
                std::cout << queueRunner->node->nodeNumber << " --- " << edgeEnd->nodeNumber << std::endl;
                queue->addNode(edgeEnd);
            }

            runner = runner->next;
        }

        queueRunner->node->mark = NODE_PASSED;
        queueRunner = queueRunner->nextNode;
    }*/

    while (queueRunner != nullptr) {
        runner = queueRunner->node->adjacencyList;
        while (runner != nullptr) {
            runner->mark = EDGE_PASSED;
            edgeEnd = runner->node;

            if (edgeEnd->mark == NODE_NOT_MARKED) {
                edgeEnd->mark = NODE_MARKED_NOT_PASSED;
                queue->addNode(edgeEnd);
            }

            if (edgeEnd->mark != NODE_PASSED) {
                std::cout << queueRunner->node->nodeNumber << " --- " << edgeEnd->nodeNumber << std::endl;
            }

            runner = runner->next;
        }

        queueRunner->node->mark = NODE_PASSED;
        queueRunner = queueRunner->nextNode;
    }

    /*while (queueRunner != nullptr) {
        runner = queueRunner->node->adjacencyList;
        while (runner != nullptr) {
            edgeEnd = runner->node;

            if (edgeEnd->mark == NODE_NOT_MARKED) {
                edgeEnd->mark = NODE_MARKED_NOT_PASSED;
                queue->addNode(edgeEnd);
            }

            if (runner->mark == EDGE_NOT_PASSED && edgeEnd->mark != NODE_PASSED) {
                std::cout << queueRunner->node->nodeNumber << " --- " << edgeEnd->nodeNumber << std::endl;
            }

            runner->mark = EDGE_PASSED;
            runner = runner->next;
        }

        queueRunner->node->mark = NODE_PASSED;
        queueRunner = queueRunner->nextNode;
    }*/
}

