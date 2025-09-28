//
// Created by oblepixa on 9/28/25.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <queue>

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

    T get_root() { return root->data; }
    T get_root_left() { return root->left->data; }
    T get_root_right() { return root->right->data; }

    // Methods to insert node in the binary tree
    void insert_node(T value) {
        auto newNode = new Node<T>(value);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        std::queue<Node<T>*> q;
        q.push(root);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            if (current->left == nullptr) {
                current->left = newNode;
                return;
            }
            q.push(current->left);

            if (current->right == nullptr) {
                current->right = newNode;
                return;
            }
            q.push(current->right);
        }
    }
};

#endif //BINARY_TREE_H
