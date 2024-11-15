#pragma once

#include <iostream>
#include <string>
#include "Node.h"

using std::cout;
using std::endl;

template <class T>
class AVL
{

private:
    Node<T> *rootNode = nullptr;

    // returns nullptr if value is in tree
    Node<T> *bstInsert(Node<T> *node, T item, int height = 1)
    {
        int newHeight = height + 1;
        // bigger/left
        if (item < node->value)
        {
            if (node->left == nullptr)
            {
                node->left = new Node<T>(item);
                node->left->height = newHeight;
                return node->left;
            }
            else
            {
                return bstInsert(node->left, item, newHeight);
            }
        }
        // smaller/right
        else if (item > node->value)
        {
            if (node->right == nullptr)
            {
                node->right = new Node<T>(item);
                node->right->height = newHeight;
                return node->right;
            }
            else
            {
                return bstInsert(node->right, item, newHeight);
            }
        }
        // assume found otherwise
        return nullptr;
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

    void rotateLeft(Node<T> *left, Node<T> *parent, Node<T> *right)
    {
        Node<T> *newLeft;
    }

    void rotateRight()
    {
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
        if (rootNode->value == item)
        {
            bstRemove(nullptr, rootNode);
            return true;
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