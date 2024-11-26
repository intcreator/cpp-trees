#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include "Node.h"
#include "BSTtoString.h"

using std::cout;
using std::endl;

template <class T>
class AVL
{

private:
    Node<T> *rootNode = nullptr;

    int getHeight(Node<T> *node)
    {
        return std::max(node->left ? node->left->height : (leafNodeHeight - 1), node->right ? node->right->height : (leafNodeHeight - 1)) + 1;
    }

    int getBalanceFactor(Node<T> *node)
    {
        int leftHeight = node->left ? node->left->height : (leafNodeHeight - 1);
        int rightHeight = node->right ? node->right->height : (leafNodeHeight - 1);
        // cout << "getBalanceFactor for " << node->value << " leftHeight " << leftHeight << " rightHeight " << rightHeight << endl;
        return leftHeight - rightHeight;
    }

    void rotateLeft(Node<T> *node, Node<T> *parent)
    {
        // cout << "rotateLeft node " << node->value << endl;
        Node<T> *nodeRightLeft = node->right->left;
        if (parent == nullptr)
        {
            // cout << "rotateLeft parent is rootNode " << endl;
            rootNode = node->right;
        }
        else
        {
            // cout << "rotateLeft parent " << parent->value << endl;
            if (parent->left == node)
            {
                parent->left = node->right;
            }
            else
            {
                parent->right = node->right;
            }
        }
        node->right->left = node;
        node->right = nodeRightLeft;
        // cout << BSTtoString(rootNode, true);
    }

    void rotateRight(Node<T> *node, Node<T> *parent)
    {
        // cout << "rotateRight node " << node->value << endl;
        Node<T> *nodeLeftRight = node->left->right;
        if (parent == nullptr)
        {
            // cout << "rotateRight parent is rootNode " << endl;
            rootNode = node->left;
        }
        else
        {
            // cout << "rotateRight parent " << parent->value << endl;
            if (parent->left == node)
            {
                parent->left = node->left;
            }
            else
            {
                parent->right = node->left;
            }
        }
        node->left->right = node;
        node->left = nodeLeftRight;
        // cout << BSTtoString(rootNode, true);
    }

    // returns true if a rotation took place
    bool rotateIfNeeded(Node<T> *node, Node<T> *parent)
    {
        int nodeBalanceFactor = getBalanceFactor(node);
        // cout << "rotateIfNeeded " << node->value << "(" << node->height << ") with balanceFactor " << nodeBalanceFactor << endl;
        // cout << BSTtoString(rootNode, true) << endl;
        // balance difference greater than 1
        if (nodeBalanceFactor > 1)
        {
            int leftChildBalanceFactor = getBalanceFactor(node->left);
            // cout << "leftChildBalanceFactor " << leftChildBalanceFactor << endl;
            // left child of the left subtree (single rotation)
            if (leftChildBalanceFactor >= 0)
            {
                // cout << "left child of the left subtree (single rotation)" << endl;
                rotateRight(node, parent);
            }
            // left child of the right subtree (double rotation)
            else
            {
                // cout << "left child of the right subtree (double rotation)" << endl;
                rotateLeft(node->right, node);
                rotateRight(node, parent);
            }
            return true;
        }
        // balance difference less than -1
        else if (nodeBalanceFactor < -1)
        {
            int rightChildBalanceFactor = getBalanceFactor(node->right);
            // cout << "rightChildBalanceFactor " << rightChildBalanceFactor << endl;
            // right child of the right subtree (single rotation)
            if (rightChildBalanceFactor <= 0)
            {
                // cout << "right child of the right subtree (single rotation)" << endl;
                rotateLeft(node, parent);
            }
            // right child of the left subtree (double rotation)
            else
            {
                // cout << "right child of the left subtree (double rotation)" << endl;
                rotateRight(node->left, node);
                rotateLeft(node, parent);
            }
            return true;
        }
        return false;
    }

    void checkPerformRotations(Node<T> *node)
    {
        if (node == nullptr)
        {
            return;
        }

        // cout << BSTtoString(rootNode, true);
        // cout << "checkPerformRotations " << node->value << "(" << node->height << ")" << endl;
        bool didRotate = false;
        if (node->left != nullptr)
        {
            node->left->height = getHeight(node->left);
            didRotate = didRotate || rotateIfNeeded(node->left, node);
            // cout << "node->left rotation " << didRotate << endl;
        }
        if (node->right != nullptr)
        {
            node->right->height = getHeight(node->right);
            didRotate = didRotate || rotateIfNeeded(node->right, node);
            // cout << "node->right rotation " << didRotate << endl;
        }
        if (node == rootNode)
        {
            node->height = getHeight(node);
            didRotate = didRotate || rotateIfNeeded(node, nullptr);
            // cout << "rootNode rotation " << didRotate << endl;
        }
        // cout << "no rotations performed\n";
        checkPerformRotations(findNodeParent(rootNode, node->value));
    }

    // returns nullptr if value is in tree
    Node<T> *bstInsert(Node<T> *node, T item)
    {
        Node<T> *insertedNode;
        // bigger/left
        if (item < node->value)
        {
            if (node->left == nullptr)
            {
                node->left = new Node<T>(item);
                node->left->height = leafNodeHeight;
                insertedNode = node->left;
            }
            else
            {
                insertedNode = bstInsert(node->left, item);
            }
        }
        // smaller/right
        else if (item > node->value)
        {
            if (node->right == nullptr)
            {
                node->right = new Node<T>(item);
                node->right->height = leafNodeHeight;
                insertedNode = node->right;
            }
            else
            {
                insertedNode = bstInsert(node->right, item);
            }
        }
        else
        {
            // assume found otherwise
            return nullptr;
        }
        // store new height - one more than the maximum of the children
        // cout << "child heights for " << node->value << " are " << (node->left ? node->left->height : (leafNodeHeight - 1)) << ", " << (node->right ? node->right->height : (leafNodeHeight - 1)) << endl;
        if (insertedNode != nullptr)
        {
            checkPerformRotations(node);
        }
        node->height = getHeight(node);
        return insertedNode;
    }

    Node<T> *findNodeParent(Node<T> *node, T item)
    {
        // return null if the item is located at this level
        // we should only reach this if we're searching for the parent of the root
        // cout << "findNodeParent of " << item << endl;
        if (node == nullptr || node->value == item)
        {
            return nullptr;
        }
        Node<T> *leftChild = node->left;
        Node<T> *rightChild = node->right;
        if ((leftChild != nullptr && leftChild->value == item) || (rightChild != nullptr && rightChild->value == item))
        {
            return node;
        }
        else if (item > node->value)
        {
            return findNodeParent(node->right, item);
        }
        else
        {
            return findNodeParent(node->left, item);
        }
    }

    Node<T> *findInOrderSuccessorParent(Node<T> *node)
    {
        // if right child is empty, right child is in order successor and node is parent
        if (node->right->left == nullptr)
        {
            return node;
        }
        // otherwise search
        Node<T> *candidateSuccessorParent = node->right;
        while (candidateSuccessorParent->left->left != nullptr)
        {
            candidateSuccessorParent = candidateSuccessorParent->left;
        }
        return candidateSuccessorParent;
    }

    Node<T> *findInOrderPredecessorParent(Node<T> *node)
    {
        // if left child is empty, left child is in order predecessor and node is parent
        if (node->left->right == nullptr)
        {
            return node;
        }
        // otherwise search
        Node<T> *candidateSuccessorParent = node->left;
        while (candidateSuccessorParent->right->right != nullptr)
        {
            candidateSuccessorParent = candidateSuccessorParent->right;
        }
        return candidateSuccessorParent;
    }

    // returns the new parent of the removed node
    Node<T> *bstRemoveWithTwoChildren(Node<T> *parent, Node<T> *node)
    {
        // find in order predecessor and its parent
        Node<T> *switchingNodeParent = findInOrderPredecessorParent(node);
        Node<T> *switchingNode;

        // the new parent of the node to be removed (passed recursively)
        // Node<T> *newParent;
        if (switchingNodeParent == node)
        {
            switchingNode = switchingNodeParent->left;
        }
        else
        {
            switchingNode = switchingNodeParent->right;
        }
        // cout << "switchingNode " << switchingNode->value << " switchingNodeParent " << switchingNodeParent->value << endl;
        // std::string printParent = "rootNode";
        // if (parent != nullptr)
        // {
        //     printParent = std::to_string(parent->value);
        // }
        // cout << "parent is " << printParent << endl;

        Node<T> *temp = new Node<int>(-1);
        // switch with in order successor

        // save switchingNode children
        temp->left = switchingNode->left;
        temp->right = switchingNode->right;
        // reassign switchingNode as rootNode or child of parent
        // don't point to itself
        if (switchingNode != node->left)
        {
            switchingNode->left = node->left;
        }
        if (switchingNode != node->right)
        {
            switchingNode->right = node->right;
        }
        if (node == rootNode)
        {
            rootNode = switchingNode;
        }
        else if (parent->left == node)
        {
            parent->left = switchingNode;
        }
        else
        {
            parent->right = switchingNode;
        }
        // reassign node as a child of switchingNodeParent
        node->left = temp->left;
        node->right = temp->right;
        // only add this connection if it's not the same element otherwise it will create a circular reference
        if (switchingNodeParent != node)
        {
            if (switchingNodeParent->left == switchingNode)
            {
                switchingNodeParent->left = node;
            }
            else
            {
                switchingNodeParent->right = node;
            }
        }

        delete temp;

        // cout << BSTtoString(rootNode, true);

        // call bstRemove again on node which will recur this function or
        // call a different function once it has fewer than two children
        return bstRemove(switchingNodeParent, node);
    }

    // returns the new parent of the removed node
    Node<T> *bstRemoveWithOneChild(Node<T> *parent, Node<T> *node)
    {
        Node<T> *newParent;
        if (node == rootNode)
        {
            if (node->left != nullptr)
            {
                rootNode = node->left;
            }
            else
            {
                rootNode = node->right;
            }
            newParent = nullptr;
        }
        else if (parent->left == node)
        {
            if (node->left != nullptr)
            {
                parent->left = node->left;
            }
            else
            {
                parent->left = node->right;
            }
            newParent = parent->left;
        }
        else
        {
            if (node->left != nullptr)
            {
                parent->right = node->left;
            }
            else
            {
                parent->right = node->right;
            }
            newParent = parent->right;
        }
        node->left = nullptr;
        node->right = nullptr;
        return newParent;
    }

    // returns the new parent of the removed node
    // since no switching is done, return parent
    Node<T> *bstRemoveWithNoChildren(Node<T> *parent, Node<T> *node)
    {
        if (node == rootNode)
        {
            rootNode = nullptr;
        }
        else if (parent->left == node)
        {
            parent->left = nullptr;
        }
        else
        {
            parent->right = nullptr;
        }
        return parent;
    }

    Node<T> *bstRemove(Node<T> *parent, Node<T> *node)
    {
        // cout << "bstRemove " << node->value << endl;
        Node<T> *removedNodeNewParent;
        // case 1: no children
        if (node->left == nullptr && node->right == nullptr)
        {
            removedNodeNewParent = bstRemoveWithNoChildren(parent, node);
            checkPerformRotations(removedNodeNewParent);
        }
        // case 2: 2 children
        else if (node->left != nullptr && node->right != nullptr)
        {
            removedNodeNewParent = bstRemoveWithTwoChildren(parent, node);
            // return without deleting since the node will be deleted in a recursive call
            return removedNodeNewParent;
        }
        // case 3: 1 child
        else
        {
            removedNodeNewParent = bstRemoveWithOneChild(parent, node);
            checkPerformRotations(removedNodeNewParent);
        }
        delete node;
        // cout << BSTtoString(rootNode, true);
        return removedNodeNewParent;
    }

public:
    AVL()
    {
        // implement the contructor here
    }

    ~AVL()
    {
        // implement the destructor here
        clear();
    }

    Node<T> *getRootNode() const
    {
        // implement getRootNode here
        // return a pointer to the tree's root node
        return rootNode;
    }

    bool insert(T item)
    {
        // cout << BSTtoString(rootNode, true) << endl;
        // cout << "insert " << item << endl;
        // implement insert here
        // return true if item was inserted, false if item was already in the tree

        // insert at root if root is null
        if (rootNode == nullptr)
        {
            rootNode = new Node<T>(item);
            return true;
        }
        else
        {
            // BST insert
            Node<T> *newNode = bstInsert(rootNode, item);

            // return false if exists
            if (newNode == nullptr)
            {
                return false;
            }

            // perform rotations if needed
            return true;
        }
    }

    bool remove(T item)
    {
        // implement remove here
        // return true if item was removed, false if item wasn't in the tree
        if (rootNode == nullptr)
        {
            return false;
        }
        if (rootNode->value == item)
        {
            bstRemove(nullptr, rootNode);
            return true;
        }
        else
        {
            Node<T> *parent = findNodeParent(rootNode, item);
            if (parent == nullptr)
            {
                return false;
            }
            if (parent->left != nullptr && parent->left->value == item)
            {
                bstRemove(parent, parent->left);
                return true;
            }
            else if (parent->right != nullptr && parent->right->value == item)
            {
                bstRemove(parent, parent->right);
                return true;
            }
        }
        return false;
    }

    bool contains(T item) const
    {
        // implement contains here
        // return true if item is in the tree, false otherwise
        return false;
    }

    void clear()
    {
        // implement clear here
        // remove all nodes from the tree

        // just remove the root node over and over
        while (rootNode != nullptr)
        {
            bstRemove(nullptr, rootNode);
        }
    }

    int size() const
    {
        // implement size here
        // return the number of nodes in the tree
        return 0;
    }
};