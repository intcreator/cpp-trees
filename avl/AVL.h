#pragma once

#include "Node.h"

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

        // just remove the root node over and over?
    }

    int size() const
    {
        // implement size here
        // return the number of nodes in the tree
        return 0;
    }
};