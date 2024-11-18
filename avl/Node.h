#pragma once

const int leafNodeHeight = 1;

template <class T>
struct Node
{
    Node<T> *left;
    Node<T> *right;
    T value;
    int height; // leaf nodes have height 1

    Node(T v) : left(nullptr), right(nullptr), value(v), height(leafNodeHeight) {}
};