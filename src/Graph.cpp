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

int GraphNode::getMinColor(int minColor) {
    int startColor = 0;

    if (this->adjacencyList == nullptr) {
        return 1;
    }

    AdjacencyListNode* adjacencyRunner = this->adjacencyList;

    int maxColor = this->color;

    while (adjacencyRunner != nullptr) {
        if (adjacencyRunner->node->color > maxColor) {
            maxColor = adjacencyRunner->node->color;
        }

        adjacencyRunner = adjacencyRunner->next;
    }

    if (maxColor == -1) {
        return 1;
    }

    bool busyColors[maxColor + 1];

    for(int i = 0; i < maxColor + 1; i++) {
        busyColors[i] = false;
    }

    if (minColor != -1) {
        busyColors[minColor - 1] = true;
        startColor = minColor;
    }

    adjacencyRunner = this->adjacencyList;

    while (adjacencyRunner != nullptr) {
        if (adjacencyRunner->node->color != -1) {
           busyColors[adjacencyRunner->node->color - 1] = true;
        }

        adjacencyRunner = adjacencyRunner->next;
    }

    for (int i = startColor; i < maxColor + 1; ++i) {
        if (!busyColors[i]) {
            return (i + 1);
        }
    }

    return maxColor + 1;
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
    this->colorCount = 0;
}

Graph::~Graph() {
    if (nodeAdjacencyList == nullptr) {
        return;
    }

    while (this->nodeAdjacencyList != nullptr) {
        this->removeNode(this->nodeAdjacencyList->nodeNumber);
    }

}

int Graph::getColorCount() {
    GraphNode* runner = this->nodeAdjacencyList;
    int temp = 0;

    while (runner != nullptr) {
        if (runner->color > temp) {
            temp = runner->color;
        }

        runner = runner->nextNode;
    }

    return temp;
}

void Graph::recolorizeNode(GraphNode* node) {
    int minColor = node->getMinColor(node->color);

    if (node == this->nodeAdjacencyList && minColor >= colorCount) {
        colorizeNode(node);
        this->colorized = true;
        return;
    }

    if (minColor < colorCount) {
        node->color = minColor;
        //colorizeNodes(node->nextNode);
    } else {
        node->color = -1;
        recolorizeAdjacency(node);
        colorizeNode(node);
    }
}

void Graph::colorizeNode(GraphNode* node) {
//    std::cout << std::endl;
//    this->printGraph();

    if (colorCount == 0) {
        node->color = node->getMinColor();
    } else {
        if (node->getMinColor() < colorCount || !colorized) {
            node->color = node->getMinColor();
        } else {
            node->color = -1;
            recolorizeAdjacency(node);
            colorizeNode(node);
        }
    }
}

void Graph::colorizeNodes(GraphNode* node) {

    if (node == nullptr) {
        return;
    }

    GraphNode* runner = node;

    while (runner != nullptr) {
        colorizeNode(runner);
        runner = runner->nextNode;
    }
}

void Graph::recolorizeAdjacency(GraphNode* node) {
//    std::cout << std::endl;
//    this->printGraph();

    AdjacencyListNode* runner = node->adjacencyList;

    if (runner == nullptr) {
        return;
    }

    while (runner->next != nullptr) {
        runner = runner->next;
    }

    while (runner != nullptr && runner->node->color == -1) {
        runner = runner->prev;
    }

    if (runner == nullptr) {
        node->color = -1;
        recolorizeAdjacency(node->adjacencyList->node);
        colorizeNode(node);
        return;
    }

    recolorizeNode(runner->node);
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
        if (node->adjacencyList != nullptr) {
            AdjacencyListNode* runner = node->adjacencyList;

            while (runner->next != nullptr) {
                runner = runner->next;
            }

            while (runner->prev != nullptr) {
                runner = runner->prev;
                removeEdge(node, runner->next->node);
            }

            removeEdge(node, runner->node);
        }

//        if (this->nodeAdjacencyList == node) {
//            nodeAdjacencyList = node->nextNode;
//        } else {
//            if (node->nextNode == nullptr) {
//                node->prevNode->nextNode = nullptr;
//            } else {
//                node->prevNode->nextNode = node->nextNode;
//                node->nextNode->prevNode = node->prevNode;
//            }
//        }
//
//        delete node;
//        return true;

        if (node->prevNode != nullptr) {
            node->prevNode->nextNode = node->nextNode;
        } else {
            this->nodeAdjacencyList = node->nextNode;
        }

        if (node->nextNode != nullptr) {
            node->nextNode->prevNode = node->prevNode;
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

    this->colorized = false;
    colorizeNodes(this->nodeAdjacencyList);
    this->colorCount = this->getColorCount();
    GraphNode* node = getMinNodeWithColor(colorCount);
    if (node->adjacencyList != nullptr) {
        recolorizeNode(node);

        if (this->colorCount < this->getColorCount()) {
            colorizeNodes(this->nodeAdjacencyList);
        }
    }

//    while (nodeAdjacencyList->getMinColor() != colorCount - 1) {
//    //while (this->getColorCount() >= this->colorCount) {
//
//        GraphNode* node = getMinNodeWithColor(colorCount);
//        recolorizeNode(node);
//
////        this->printGraph();
////
////        int tempColorCount = this->getColorCount();
////        if (tempColorCount < this->colorCount) {
////            this->colorCount = tempColorCount;
////        }
//    }

    this->colorCount = this->getColorCount();
}

void Graph::traversing() {
    if (this->nodeAdjacencyList == nullptr || this->nodeAdjacencyList->adjacencyList == nullptr) {
        std::cout << "null";
        return;
    }

    auto* queue = new NodeQueue{this->nodeAdjacencyList};
    NodeQueue* queueRunner = queue;
    AdjacencyListNode* runner;
    GraphNode* edgeEnd;
    this->nodeAdjacencyList->mark = NODE_MARKED_NOT_PASSED;

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

}

