#pragma once

template <class T>
struct Node
{
    Node<T> *left;
    Node<T> *right;
    T value;
    int height;

    Node(T v) : left(nullptr), right(nullptr), value(v), height(1) {}
};