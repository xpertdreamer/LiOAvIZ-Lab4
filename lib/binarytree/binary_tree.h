//
// Created by oblepixa on 9/28/25.
//

#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#include <iostream>
#include <vector>
#include <stdexcept>
#include <climits>

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

    // Helper method to count entries and find min/max levels
    void count_entries_helper(Node<T>* r, int& counter, const T& value, int currentLevel, int& minLevel, int& maxLevel) const {
        if (r == nullptr) return;

        // Update current level
        if (value == r->data) {
            ++counter;
            if (currentLevel < minLevel) minLevel = currentLevel;
            if (currentLevel > maxLevel) maxLevel = currentLevel;
        }

        // Recursively search in left and right subtrees
        count_entries_helper(r->left, counter, value, currentLevel + 1, minLevel, maxLevel);
        count_entries_helper(r->right, counter, value, currentLevel + 1, minLevel, maxLevel);
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

    // Modified to track levels of found elements
    bool find_path(Node<T>* r, T target, std::vector<T>& current_path, int& minLevel, int& maxLevel) const {
        if (r == nullptr) return false;

        bool found_any = false;
        current_path.push_back(r->data);

        if (r->data == target) {
            const int currentLevel = current_path.size() - 1;
            if (currentLevel < minLevel) minLevel = currentLevel;
            if (currentLevel > maxLevel) maxLevel = currentLevel;

            for (const auto& val : current_path) std::cout << val << " ";
            std::cout << std::endl;
            found_any = true;
        }

        const bool left = find_path(r->left, target, current_path, minLevel, maxLevel);
        const bool right = find_path(r->right, target, current_path, minLevel, maxLevel);

        current_path.pop_back();
        return found_any || left || right;
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

    int height_recursive(Node<T>* node, T value, const bool util) const {
        if (node == nullptr) return -1;
        if (node->data == value && util == true ) return -1;
        const int left_height = height_recursive(node->left, value, util);
        const int right_height = height_recursive(node->right, value, util);

        if (left_height > right_height) return left_height + 1;
        return right_height + 1;
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
        int counter = 0;
        int minLevel = INT_MAX;
        int maxLevel = -1;
        count_entries_helper(root, counter, value, 0, minLevel, maxLevel);
        std::cout << "Min level: " << minLevel << std::endl;
        std::cout << "Max level: " << maxLevel << std::endl;
        return counter;
    }

    void find_levels() const{
        std::cout << "Min level: 0" << std::endl;
        std::cout << "Max level: " << height_recursive(root, 0, 0) << std::endl;
    }

    // Method to search a path to a value in the tree
    void get_path(T value) const {
        std::vector<T> current_path;
        int minLevel = INT_MAX;
        int maxLevel = -1;

        if (const bool found = find_path(root, value, current_path, minLevel, maxLevel); !found) {
            throw std::runtime_error("Not found");
        }
        std::cout << "Min level: " << minLevel << std::endl;
        std::cout << "Max level: " << maxLevel << std::endl;
    }
};

#endif //BINARY_TREE_H
