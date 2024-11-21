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
        node->height -= 2;
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
        node->height -= 2;
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
            // left child of the left subtree (single rotation)
            if (leftChildBalanceFactor > 0)
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
            // right child of the right subtree (single rotation)
            if (rightChildBalanceFactor < 0)
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
        // cout << "checkPerformRotations " << node->value << endl;
        bool didRotate = false;
        if (node->left != nullptr)
        {
            didRotate = didRotate || rotateIfNeeded(node->left, node);
            // cout << "node->left rotation " << didRotate << endl;
            if (didRotate)
            {
                return;
            }
        }
        if (node->right != nullptr)
        {
            didRotate = didRotate || rotateIfNeeded(node->right, node);
            // cout << "node->right rotation " << didRotate << endl;
            if (didRotate)
            {
                return;
            }
        }
        if (node == rootNode)
        {
            didRotate = didRotate || rotateIfNeeded(node, nullptr);
            // cout << "rootNode rotation " << didRotate << endl;
            if (didRotate)
            {
                return;
            }
        }
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
        node->height = std::max(node->left ? node->left->height : (leafNodeHeight - 1), node->right ? node->right->height : (leafNodeHeight - 1)) + 1;
        return insertedNode;
    }

    Node<T> *findNodeParent(Node<T> *node, T item)
    {
        if (node == nullptr)
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

    void bstRemoveWithTwoChildren(Node<T> *parent, Node<T> *node)
    {
        // find in order successor and its parent
        Node<T> *inOrderSuccessorParent = findInOrderSuccessorParent(node);
        Node<T> *inOrderSuccessor;
        Node<T> *newParent;
        if (inOrderSuccessorParent == node)
        {
            inOrderSuccessor = node->right;
            newParent = inOrderSuccessor;
        }
        else
        {
            inOrderSuccessor = inOrderSuccessorParent->left;
            newParent = inOrderSuccessorParent;
        }
        std::string printParent = "rootNode";
        if (parent != nullptr)
        {
            printParent = std::to_string(parent->value);
        }

        Node<T> *temp = new Node<int>(-1);
        // switch with in order successor

        // save inOrderSuccessor children
        temp->left = inOrderSuccessor->left;
        temp->right = inOrderSuccessor->right;
        // reassign inOrderSuccessor as rootNode or child of parent
        inOrderSuccessor->left = node->left;
        inOrderSuccessor->right = node->right;
        if (node == rootNode)
        {
            rootNode = inOrderSuccessor;
        }
        else if (parent->left == node)
        {
            parent->left = inOrderSuccessor;
        }
        else
        {
            parent->right = inOrderSuccessor;
        }
        // reassign node as a child of inOrderSuccessorParent
        node->left = temp->left;
        node->right = temp->right;
        // only add this connection if it's not the same element otherwise it will create a circular reference
        if (inOrderSuccessorParent != node)
        {
            if (inOrderSuccessorParent->left == inOrderSuccessor)
            {
                inOrderSuccessorParent->left = node;
            }
            else
            {
                inOrderSuccessorParent->right = node;
            }
        }

        delete temp;

        // call bstRemove again on node
        bstRemove(newParent, node);
    }

    void bstRemoveWithOneChild(Node<T> *parent, Node<T> *node)
    {
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
        }
        node->left = nullptr;
        node->right = nullptr;
    }

    void bstRemove(Node<T> *parent, Node<T> *node)
    {
        // case 1: no children
        if (node->left == nullptr && node->right == nullptr)
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
        }
        // case 2: 2 children
        else if (node->left != nullptr && node->right != nullptr)
        {
            bstRemoveWithTwoChildren(parent, node);
            // return without deleting since the node will be deleted in a recursive call
            return;
        }
        // case 3: 1 child
        else
        {
            bstRemoveWithOneChild(parent, node);
        }
        delete node;
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