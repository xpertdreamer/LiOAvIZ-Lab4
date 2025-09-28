//
// Created by oblepixa on 9/28/25.
//
#include <../lib/binary_tree.h>

int main() {
    BinaryTree<int> tree;
    tree.insert_node(1);
    tree.insert_node(2);
    tree.insert_node(3);
    tree.insert_node(4);
    tree.insert_node(5);
    tree.insert_node(6);
    tree.insert_node(7);
    tree.insert_node(5);
    tree.insert_node(5);
    tree.insert_node(5);
    tree.insert_node(5);
    tree.insert_node(5);

    tree.print_tree();
    tree.inorder();
    tree.preorder();
    std::cout << std::endl;
    std::cout << tree.count_entries(3);
    std::cout << std::endl;
    tree.get_path(5);
    return 0;
}
