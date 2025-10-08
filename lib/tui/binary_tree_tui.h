#ifndef BINARY_TREE_PLAYGROUND_H
#define BINARY_TREE_PLAYGROUND_H

#include "../binarytree/binary_tree.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace Colors {
    // ANSI color codes for terminal output
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN = "\033[36m";
    const std::string BOLD = "\033[1m";
    const std::string UNDERLINE = "\033[4m";
}

namespace BinaryTreePlayground {

    // Convert any value type to string for display purposes
    template<typename T>
    std::string value_to_string(const T& value) {
        if constexpr (std::is_same_v<T, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<T, char>) {
            return std::string(1, value);
        } else {
            return std::to_string(value);
        }
    }

    // Wrapper class that adds history tracking and utility methods to BinaryTree
    template<typename T>
    class TreeWrapper {
    private:
        std::unique_ptr<BinaryTree<T>> tree_;  // The actual binary tree
        std::string name_;                     // Name identifier for this tree
        std::vector<std::string> history_;     // Operation history (last 20 operations)

    public:
        explicit TreeWrapper(std::string name) : name_(std::move(name)) {
            tree_ = std::make_unique<BinaryTree<T>>();
        }

        // Getters
        [[nodiscard]] const std::string &get_name() const { return name_; }
        BinaryTree<T>* get_tree() { return tree_.get(); }
        const BinaryTree<T>* get_tree() const { return tree_.get(); }
        [[nodiscard]] const std::vector<std::string>& get_history() const { return history_; }

        // Add operation to history with size limit
        void add_to_history(const std::string& operation) {
            history_.push_back(operation);
            if (history_.size() > 20) {
                history_.erase(history_.begin());
            }
        }

        // Insert value into the tree and record operation
        void insert(const T &value, bool& repeat) {
            tree_->insert_node(value, repeat);
            add_to_history("insert " + value_to_string(value));
        }

        // Search for value in tree and record operation with result
        bool search(const T &value) {
            const bool result = tree_->search(value);
            add_to_history("search " + value_to_string(value) + " -> " + (result ? "found" : "not found"));
            return result;
        }

        // Perform inorder traversal and capture output
        std::string inorder() {
            const std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            tree_->inorder();
            std::cout.rdbuf(old);
            add_to_history("inorder");
            return buffer.str();
        }

        // Perform preorder traversal and capture output
        std::string preorder() {
            const std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            tree_->preorder();
            std::cout.rdbuf(old);
            add_to_history("preorder");
            return buffer.str();
        }

        // Count occurrences of value in tree
        int count_entries(const T &value) {
            const int count = tree_->count_entries(value);
            add_to_history("count " + value_to_string(value) + " -> " + std::to_string(count));
            return count;
        }

        // Get path to value in tree
        std::string get_path(const T &value) {
            const std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            tree_->get_path(value);
            std::cout.rdbuf(old);
            add_to_history("path " + value_to_string(value));
            return buffer.str();
        }

        // Print tree structure
        std::string print_tree() {
            const std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            tree_->print_tree();
            std::cout.rdbuf(old);
            add_to_history("print");
            return buffer.str();
        }

        std::string find_level() {
            const std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            tree_->find_levels();
            std::cout.rdbuf(old);
            add_to_history("find level");
            return buffer.str();
        }

        // Check if tree is empty
        [[nodiscard]] bool empty() const {
            return tree_->get_root() == nullptr;
        }

        // Get number of nodes in tree
        [[nodiscard]] int size() const {
            return count_nodes(tree_->get_root());
        }

        // Clear all nodes from tree
        void clear() {
            tree_ = std::make_unique<BinaryTree<T>>();
            add_to_history("clear");
        }

        // Display tree statistics
        void print_stats() const {
            auto root = tree_->get_root();
            if (!root) {
                std::cout << Colors::YELLOW << "Tree is empty" << Colors::RESET << std::endl;
                return;
            }

            std::cout << Colors::CYAN << "=== Tree Statistics ===" << Colors::RESET << std::endl;
            std::cout << "Root value: " << Colors::BOLD << root->data << Colors::RESET << std::endl;
            std::cout << "Total nodes: " << Colors::BOLD << count_nodes(root) << Colors::RESET << std::endl;
            std::cout << Colors::BOLD;
            tree_->find_levels();
            std::cout << Colors::RESET;
            std::cout << "Min value: " << Colors::BOLD << find_min(root) << Colors::RESET << std::endl;
            std::cout << "Max value: " << Colors::BOLD << find_max(root) << Colors::RESET << std::endl;
        }

    private:
        // Recursively count nodes in subtree
        int count_nodes(const Node<T>* node) const {
            if (!node) return 0;
            return 1 + count_nodes(node->left) + count_nodes(node->right);
        }

        // Find minimum value in subtree
        T find_min(const Node<T>* node) const {
            while (node && node->left) node = node->left;
            return node ? node->data : T{};
        }

        // Find maximum value in subtree
        T find_max(const Node<T>* node) const {
            while (node && node->right) node = node->right;
            return node ? node->data : T{};
        }
    };

    // Main manager class for handling multiple trees and user interactions
    template<typename T>
    class BinaryTreePlaygroundManager {
    private:
        std::unordered_map<std::string, std::unique_ptr<TreeWrapper<T>>> trees_;  // All created trees
        std::unordered_map<std::string, std::function<void(std::istringstream &)>> commands_;  // Command handlers
        std::string current_tree_;        // Currently selected tree name
        int tree_counter_ = 0;            // Counter for auto-generating tree names
        std::vector<std::string> command_history_;  // Command history (last 20 commands)
        bool show_colors_ = true;         // Flag for colored output

        // Initialize all supported commands with their handlers
        void initialize_commands() {
            commands_ = {
                // Create a new tree with optional name
                {
                    "create", [this](std::istringstream &iss) {
                        std::string name;
                        if (!(iss >> name)) name = generate_tree_name();
                        handle_create(name);
                    }
                },
                // Switch to using specified tree
                {
                    "use", [this](std::istringstream &iss) {
                        std::string name;
                        if (!(iss >> name)) throw std::runtime_error("Invalid tree name");
                        handle_use(name);
                    }
                },
                // Remove specified tree
                {
                    "remove", [this](std::istringstream &iss) {
                        std::string name;
                        if (!(iss >> name)) throw std::runtime_error("Invalid tree name");
                        handle_remove(name);
                    }
                },
                // Insert value into current tree
                {
                    "insert", [this](std::istringstream &iss) {
                        T value;
                        bool repeat;
                        if (!(iss >> value) || !(iss >> repeat)) throw std::runtime_error("Invalid value");
                        handle_insert(value, repeat);
                    }
                },
                // Insert value into current tree (alias)
                {
                    "+", [this](std::istringstream &iss) {
                        T value;
                        bool repeat;
                        if (!(iss >> value) || !(iss >> repeat)) throw std::runtime_error("Invalid value");
                        handle_insert(value, repeat);
                    }
                },
                // Search for value in current tree
                {
                    "search", [this](std::istringstream &iss) {
                        T value;
                        if (!(iss >> value)) throw std::runtime_error("Invalid value");
                        handle_search(value);
                    }
                },
                // Perform inorder traversal
                {"inorder", [this](std::istringstream&) { handle_inorder(); }},
                // Perform preorder traversal
                {"preorder", [this](std::istringstream &) { handle_preorder(); }},
                // Count occurrences of value
                {
                    "count", [this](std::istringstream &iss) {
                        T value;
                        if (!(iss >> value)) throw std::runtime_error("Invalid value");
                        handle_count(value);
                    }
                },
                // Show path to value
                {
                    "path", [this](std::istringstream &iss) {
                        T value;
                        if (!(iss >> value)) throw std::runtime_error("Invalid value");
                        handle_path(value);
                    }
                },
                // Print tree structure
                {"print", [this](std::istringstream &) { handle_print(); }},
                // Print levels of subtree
                {"levels", [this](std::istringstream &) { handle_get_level(); }},
                // Clear current tree
                {"clear", [this](std::istringstream &) { handle_clear(); }},
                // List all available trees
                {"list", [this](std::istringstream &) { handle_list(); }},
                // Check if current tree is empty
                {"empty", [this](std::istringstream &) { handle_empty(); }},
                // Get size of current tree
                {"size", [this](std::istringstream &) { handle_size(); }},
                // Show statistics of current tree
                {"stats", [this](std::istringstream &) { handle_stats(); }},
                // Show command history
                {"history", [this](std::istringstream &) { handle_history(); }},
                // Show operation history for current tree
                {"treehistory", [this](std::istringstream &) { handle_tree_history(); }},
                // Toggle colored output
                {"colors", [this](std::istringstream &) { handle_colors(); }},
                // Show help information
                {"help", [this](std::istringstream &) { handle_help(); }},
                // Show help information (alias)
                {"?", [this](std::istringstream &) { handle_help(); }}
            };
        }

        // Get pointer to currently selected tree
        TreeWrapper<T>* get_current_tree() const {
            auto it = trees_.find(current_tree_);
            if (it == trees_.end()) throw std::runtime_error("No tree selected! Use 'use <name>' first.");
            return it->second.get();
        }

        // Generate unique tree name when not provided by user
        std::string generate_tree_name() {
            return "tree_" + std::to_string(++tree_counter_);
        }

        // Print colored message to console
        void print_colored(const std::string& message, const std::string& color = Colors::RESET) {
            if (show_colors_) {
                std::cout << color << message << Colors::RESET;
            } else {
                std::cout << message;
            }
        }

        // Print colored message with newline
        void println_colored(const std::string& message, const std::string& color = Colors::RESET) {
            print_colored(message + "\n", color);
        }

        // Handle tree creation
        void handle_create(const std::string &name) {
            std::string actual_name = name.empty() ? generate_tree_name() : name;

            if (trees_.count(actual_name)) {
                println_colored("Error: Tree '" + actual_name + "' already exists!", Colors::RED);
                return;
            }

            trees_[actual_name] = std::make_unique<TreeWrapper<T>>(actual_name);
            current_tree_ = actual_name;
            println_colored("✓ Created tree: '" + actual_name + "'", Colors::GREEN);
            println_colored("Now using: " + actual_name, Colors::CYAN);
        }

        // Handle switching to different tree
        void handle_use(const std::string &name) {
            if (trees_.count(name)) {
                current_tree_ = name;
                println_colored("✓ Now using: " + name, Colors::GREEN);
            } else {
                println_colored("Error: Tree '" + name + "' not found!", Colors::RED);
            }
        }

        // Handle value insertion
        void handle_insert(const T &value, bool& repeat) {
            auto tree = get_current_tree();
            tree->insert(value, repeat);
            println_colored("✓ Inserted: " + value_to_string(value), Colors::GREEN);
        }

        // Handle value search
        void handle_search(const T &value) {
            auto tree = get_current_tree();
            bool found = tree->search(value);
            std::string result = "Value '" + value_to_string(value) + "' was " +
                           (found ? "FOUND" : "NOT FOUND") + " in the tree";
            println_colored(result, found ? Colors::GREEN : Colors::YELLOW);
        }

        // Handle inorder traversal
        void handle_inorder() {
            auto tree = get_current_tree();
            println_colored("Inorder traversal:", Colors::CYAN);
            std::string result = tree->inorder();
            if (result.empty()) {
                println_colored("(empty)", Colors::YELLOW);
            } else {
                std::cout << result;
            }
        }

        // Handle preorder traversal
        void handle_preorder() {
            auto tree = get_current_tree();
            println_colored("Preorder traversal:", Colors::CYAN);
            std::string result = tree->preorder();
            if (result.empty()) {
                println_colored("(empty)", Colors::YELLOW);
            } else {
                std::cout << result;
            }
        }

        // Handle value counting
        void handle_count(const T &value) {
            auto tree = get_current_tree();
            const int count = tree->count_entries(value);
            const std::string message = "Value '" + value_to_string(value) + "' appears " +
                           std::to_string(count) + " time(s) in the tree";
            println_colored(message, Colors::CYAN);
        }

        // Handle path display
        void handle_path(const T &value) {
            auto tree = get_current_tree();
            println_colored("Path to '" + value_to_string(value) + "': ", Colors::CYAN);
            const std::string result = tree->get_path(value);
            std::cout << result;
        }

        // Handle tree printing
        void handle_print() {
            auto tree = get_current_tree();
            println_colored("Tree structure:", Colors::CYAN);
            if (const std::string result = tree->print_tree(); result.empty()) {
                println_colored("(empty)", Colors::YELLOW);
            } else {
                std::cout << result;
            }
        }

        // Handle level display
        void handle_get_level() {
            auto tree = get_current_tree();
            tree->find_level();
            if (const std::string result = tree->find_level(); result.empty()) {
                println_colored("(empty)", Colors::YELLOW);
            } else {
                std::cout << result;
            }
        }

        // Handle tree clearing
        void handle_clear() {
            auto tree = get_current_tree();
            tree->clear();
            println_colored("✓ Tree cleared", Colors::GREEN);
        }

        // Handle tree size query
        void handle_size() {
            auto tree = get_current_tree();
            std::cout << "Size: " << tree->size() << std::endl;
        }

        // Handle tree listing
        void handle_list() {
            if (trees_.empty()) {
                println_colored("No trees created!", Colors::YELLOW);
                return;
            }

            println_colored("Available trees:", Colors::CYAN);
            for (const auto &[name, tree]: trees_) {
                std::string marker = (name == current_tree_) ? " → " : "   ";
                std::string status = tree->empty() ? "empty" : "non-empty";
                std::string color = (name == current_tree_) ? Colors::GREEN : Colors::RESET;

                print_colored(marker + name, color);
                std::cout << " (" << status << ")" << std::endl;
            }
        }

        // Handle empty check
        void handle_empty() {
            auto tree = get_current_tree();
            const std::string result = tree->empty() ? "empty" : "not empty";
            const std::string color = tree->empty() ? Colors::YELLOW : Colors::GREEN;
            println_colored(result, color);
        }

        // Handle statistics display
        void handle_stats() {
            auto tree = get_current_tree();
            tree->print_stats();
        }

        // Handle command history display
        void handle_history() {
            if (command_history_.empty()) {
                println_colored("No command history!", Colors::YELLOW);
                return;
            }

            println_colored("Command history (last 20):", Colors::CYAN);
            for (size_t i = 0; i < command_history_.size(); ++i) {
                std::cout << "  " << std::setw(2) << (i + 1) << ". " << command_history_[i] << std::endl;
            }
        }

        // Handle tree operation history display
        void handle_tree_history() {
            auto tree = get_current_tree();
            const auto& history = tree->get_history();

            if (history.empty()) {
                println_colored("No operations performed on this tree!", Colors::YELLOW);
                return;
            }

            println_colored("Operation history for '" + tree->get_name() + "':", Colors::CYAN);
            for (size_t i = 0; i < history.size(); ++i) {
                std::cout << "  " << std::setw(2) << (i + 1) << ". " << history[i] << std::endl;
            }
        }

        // Handle color output toggle
        void handle_colors() {
            show_colors_ = !show_colors_;
            const std::string status = show_colors_ ? "enabled" : "disabled";
            println_colored("Colors " + status, Colors::GREEN);
        }

        // Handle tree removal
        void handle_remove(const std::string &name) {
            if (trees_.count(name)) {
                if (current_tree_ == name) {
                    current_tree_.clear();
                }
                trees_.erase(name);
                println_colored("✓ Removed: " + name, Colors::GREEN);
            } else {
                println_colored("Error: Tree '" + name + "' not found!", Colors::RED);
            }
        }

        // Handle help display
        void handle_help() {
            println_colored("\n" + Colors::BOLD + "=== Binary Tree Playground Commands ===" + Colors::RESET, Colors::CYAN);
            std::cout << Colors::BOLD << "Tree Management:" << Colors::RESET << std::endl;
            std::cout << "  create [name]           - Create new tree (auto-name if omitted)" << std::endl;
            std::cout << "  use <name>              - Switch to tree" << std::endl;
            std::cout << "  remove <name>           - Remove tree" << std::endl;
            std::cout << "  list                    - List all trees" << std::endl;

            std::cout << Colors::BOLD << "\nTree Operations:" << Colors::RESET << std::endl;
            std::cout << "  insert <value>          - Insert value into current tree" << std::endl;
            std::cout << "  search <value>          - Search for value" << std::endl;
            std::cout << "  count <value>           - Count occurrences of value" << std::endl;
            std::cout << "  path <value>            - Show path to value" << std::endl;
            std::cout << "  clear                   - Clear current tree" << std::endl;

            std::cout << Colors::BOLD << "\nTree Analysis:" << Colors::RESET << std::endl;
            std::cout << "  levels                  - Print min and max levels of subtree" << std::endl;
            std::cout << "  inorder                 - Inorder traversal" << std::endl;
            std::cout << "  preorder                - Preorder traversal" << std::endl;
            std::cout << "  print                   - Print tree structure" << std::endl;
            std::cout << "  size                    - Get tree size" << std::endl;
            std::cout << "  stats                   - Show tree statistics" << std::endl;
            std::cout << "  empty                   - Check if current tree is empty" << std::endl;

            std::cout << Colors::BOLD << "\nHistory & Settings:" << Colors::RESET << std::endl;
            std::cout << "  history                 - Show command history" << std::endl;
            std::cout << "  treehistory             - Show tree operation history" << std::endl;
            std::cout << "  colors                  - Toggle color output" << std::endl;
            std::cout << "  help, ?                 - Show this help" << std::endl;
            std::cout << "  exit, quit              - Exit playground" << std::endl;

            std::cout << Colors::BOLD << "\nExamples:" << Colors::RESET << std::endl;
            std::cout << "  create mytree           " << Colors::YELLOW << "# Create tree named 'mytree'" << Colors::RESET << std::endl;
            std::cout << "  insert 50 1             " << Colors::YELLOW << "# Insert value 50 with repeat" << Colors::RESET << std::endl;
            std::cout << "  stats                   " << Colors::YELLOW << "# Show tree statistics" << Colors::RESET << std::endl;
            std::cout << Colors::BOLD << "========================================" << Colors::RESET << std::endl;
        }

        // Add command to history with size limit
        void add_to_history(const std::string& command) {
            command_history_.push_back(command);
            if (command_history_.size() > 20) {
                command_history_.erase(command_history_.begin());
            }
        }

    public:
        BinaryTreePlaygroundManager() {
            initialize_commands();
        }

        // Main interactive loop
        void run() {
            println_colored("\n" + Colors::BOLD + "Binary Tree Playground" + Colors::RESET, Colors::GREEN);
            println_colored("Type 'help' for commands, 'exit' to quit", Colors::CYAN);

            std::string command;
            while (true) {
                // Display prompt with current tree context
                if (current_tree_.empty()) {
                    print_colored("bt-playground> ", Colors::YELLOW);
                } else {
                    print_colored("bt[" + current_tree_ + "]> ", Colors::GREEN);
                }

                if (!std::getline(std::cin, command)) break;
                if (command.empty()) continue;

                add_to_history(command);

                std::istringstream iss(command);
                std::string action;
                iss >> action;

                // Exit condition
                if (action == "exit" || action == "quit") {
                    println_colored("👋 Exiting Binary Tree Playground...", Colors::GREEN);
                    break;
                }

                try {
                    // Execute command if found
                    if (auto it = commands_.find(action); it != commands_.end()) {
                        it->second(iss);
                    } else {
                        println_colored("Unknown command: '" + action + "'. Type 'help' for available commands.", Colors::RED);
                    }
                } catch (const std::exception &e) {
                    println_colored("Error: " + std::string(e.what()), Colors::RED);
                } catch (...) {
                    println_colored("Unknown error occurred", Colors::RED);
                }
            }
        }
    };
}

#endif // BINARY_TREE_PLAYGROUND_H