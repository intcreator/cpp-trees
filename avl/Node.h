#pragma once

template <class T>
struct Node
{
    Node<T> *left;
    Node<T> *right;
    T value;
    int height; // leaf nodes have height 0

    Node(T v) : left(nullptr), right(nullptr), value(v), height(1) {}
};