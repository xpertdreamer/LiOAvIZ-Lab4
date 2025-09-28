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

    tree.print_tree();
    tree.inorder();
    return 0;
}
