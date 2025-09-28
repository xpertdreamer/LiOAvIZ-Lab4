//
// Created by oblepixa on 9/28/25.
//
#include <../lib/binary_tree.h>
#include <iostream>

int main() {
    BinaryTree<int> tree;
    tree.insert_node(1);
    tree.insert_node(2);
    tree.insert_node(3);

    std::cout << tree.get_root() << std::endl;
    std::cout << tree.get_root_left() << std::endl;
    std::cout << tree.get_root_right() << std::endl;
    return 0;
}
