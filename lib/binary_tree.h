//
// Created by oblepixa on 9/28/25.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <iostream>
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

    // Disable copying
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;
};

template<typename T>
class BinaryTree {
private:
    // Pointer to the root of the tree
    Node<T>* root;

    // Method to properly clear the tree if destructor was called
    void clear_recursive(Node<T>* node) {
        if (node) {
            clear_recursive(node->left);
            clear_recursive(node->right);
            delete node;
        }
    }

    // Recursive method to search for a value in the tree
    bool search_recursive(Node<T>* current, T value) {
        if (current == nullptr) return false;
        if (current->data == value) return true;
        return search_recursive(current->left, value) ||
            search_recursive(current->right, value);
    }

    // Method for Recursive inorder traversal of the tree
    void inorder_recursive(Node<T>* node) {
        if (node != nullptr) {
            inorder_recursive(node->left);
            std::cout << node->data << " ";
            inorder_recursive(node->right);
        }
    }

    void print_tree_helper(Node<T>* r, const int level) const {
        if (r == nullptr) return;

        print_tree_helper(r->right, level + 1);
        for (int i = 0; i < level; i++) {
            std::cout << "   ";
        }

        std::cout << r->data << std::endl;
        print_tree_helper(r->left, level + 1);
    }

public:
    // Constructor to initialize the tree
    BinaryTree() : root(nullptr) {}
    // Destructor
    ~BinaryTree() {
        clear_recursive(root);
        root = nullptr;
    }
    // Disable copying
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;
    BinaryTree(BinaryTree&&) = default;
    BinaryTree& operator=(BinaryTree&&) = default;

    // Access methods
    Node<T> *get_root() { return root; }
    const Node<T> *get_root() const { return root; }

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

    // Method to search for a value in the tree
    bool search(T value) {
        return search_recursive(root, value);
    }

    // Method to perform inorder traversal of the tree
    void inorder() {
        inorder_recursive(root);
        std::cout << std::endl;
    }

    // Method to print the tree
    void print_tree() const {
        print_tree_helper(root, 0);
    }


};

#endif //BINARY_TREE_H
