//
// Created by oblepixa on 9/28/25.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H

// Template class for node of a Binary tree
template<typename T>
class Node {
public:
    // Data held by the node
    T data;
    // Pointer to the left child
    Node* left;
    // Pointer to the right child
    Node* right;

    // Constructor to initialize node with a value
    explicit Node (T value) : data(value), left(nullptr), right(nullptr) {}
    // Destructor
    ~Node() = default;
};

template<typename T>
class BinaryTree {
private:
    // Pointer to the root of the tree
    Node<T>* root;

public:
    // Constructor to initialize the tree
    BinaryTree() : root(nullptr) {}
    // Destructor
    ~BinaryTree() = default;
};

#endif //BINARY_TREE_H
