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

        if (value < current->data) return search_recursive(current->left, value);
        return search_recursive(current->right, value);
    }

    // Method for Recursive inorder traversal of the tree
    void inorder_recursive(Node<T>* node) {
        if (node != nullptr) {
            inorder_recursive(node->left);
            std::cout << node->data << " ";
            inorder_recursive(node->right);
        }
    }

    // Method for Recursive preorder traversal of the tree
    void preorderRecursive(Node<T>* node) {
        if (node != nullptr) {
            std::cout << node->data << " ";
            preorderRecursive(node->left);
            preorderRecursive(node->right);
        }
    }

    void count_entries_helper(Node<T>* r, int& counter, const T& value) const {
        if (r == nullptr) return;

        if (value == r->data) ++counter;

        if (value < r->data) count_entries_helper(r->left, counter, value);
        else count_entries_helper(r->right, counter, value);
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

    bool find_path(Node<T>* r, T target) const {
        if (r == nullptr) return false;

        std::cout << r->data << " ";

        if (r->data == target) return true;

        if (target < r->data) return find_path(r->left, target);
        return find_path(r->right, target);
    }

    Node<T>* insert_recursive_repeat(Node<T>* node, T value) {
        if (node == nullptr) return new Node<T>(value);
        if (value <= node->data) node->left = insert_recursive_repeat(node->left, value);
        else node->right = insert_recursive_repeat(node->right, value);
        return node;
    }

    Node<T>* insert_recursive(Node<T>* node, T value) {
        if (node == nullptr) return new Node<T>(value);
        if (value < node->data) node->left = insert_recursive(node->left, value);
        else if (value > node->data) node->right = insert_recursive(node->right, value);
        return node;
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

    // Methods to insert node in the binary tree (excluding the same elements)
    void insert_node(T value, const bool repeat) {
        if (!repeat) {
            if (search(value)) return;
            root = insert_recursive(root, value);
            return;
        }
        root = insert_recursive_repeat(root, value);
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

    // Method to perform preorder traversal of the tree
    void preorder() {
        preorderRecursive(root);
        std::cout << std::endl;
    }

    // Method to print the tree
    void print_tree() const {
        print_tree_helper(root,0);
    }

    // Method of calculating the number of entries of a given element into a tree.
    int count_entries(const T& value) const {
        int n = 0;
        count_entries_helper(root, n, value);
        return n;
    }


    // Method to search a path to a value in the tree
    void get_path(T value) const {
        if (!find_path(root, value)) std::cout << "Не найден" << std::endl;
        else std::cout << std::endl;
    }
};

#endif //BINARY_TREE_H
