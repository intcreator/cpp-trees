#pragma once

#include <queue>
#include <sstream>
#include "Node.h"

// a function that takes a BST and returns a level-order string representation of the BST
// returns a string representation of the nodes in level order
std::string BSTtoString(Node<int> *rootNode, bool showHeight = false)
{
    std::queue<Node<int> *> readQ;   // used to read in the levels of the tree, contains Node*
    std::stringstream nodeReader_ss; // used to store the values of the nodes and the level-order sequence
    int depth = 0;                   // the depth of a node on the tree

    if (rootNode == NULL)
    {
        return "BST is empty\n";
    }

    readQ.push(rootNode); // push the root node of the tree into the queue

    while (!readQ.empty())
    {                         // as long as the queue has a remaining node:
        int i = readQ.size(); // store the number of nodes on this level of the tree
        nodeReader_ss << depth << ":  ";
        for (; i > 0; i--)
        {                                        // for each node on this level,
            Node<int> *nextNode = readQ.front(); // store the next node in the queue
            std::stringstream height;
            if (showHeight)
            {
                height << "(" << nextNode->height << ")";
            }
            nodeReader_ss << nextNode->value << height.str() << " "; // store the data from the node into the ss
            if (nextNode->left != NULL)
            { // if there is a left child, push the left child into the queue
                readQ.push(nextNode->left);
            }
            if (nextNode->right != NULL)
            { // if there is a right child, push the left child into the queue
                readQ.push(nextNode->right);
            }
            readQ.pop(); // pop the node off of the queue, leaving its children in the queue
        }
        nodeReader_ss << "\n"; // push an endl into the ss to distinguish levels
        depth++;
    }

    return nodeReader_ss.str();
}