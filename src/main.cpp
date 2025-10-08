#include <iostream>
#include <limits>
#include <cstdlib>

#ifndef _WIN32
#include <windows.h
#endif

#include "../lib/tui/binary_tree_tui.h"

int main() {
#ifndef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;

    if (!GetConsoleMode(h, &dwMode)) {
        throw std::runtime_error("GetConsoleMode failed");
        return 1;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    if (!SetConsoleMode(h, dwMode)) {
        throw std::runtime_error("SetConsoleMode failed");
        return 1;
    }
#endif

    std::cout << "Binary Tree Playground" << std::endl;
    std::cout << "============================" << std::endl;

    std::cout << "Choose data type:" << std::endl;
    std::cout << "1 - int" << std::endl;
    std::cout << "2 - double" << std::endl;
    std::cout << "3 - string" << std::endl;
    std::cout << "4 - char" << std::endl;
    std::cout << "Enter choice (1-4): ";

    char type_choice;
    std::cin >> type_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (type_choice) {
        case '1': {
            BinaryTreePlayground::BinaryTreePlaygroundManager<int> manager;
            manager.run();
            break;
        }
        case '2': {
            BinaryTreePlayground::BinaryTreePlaygroundManager<double> manager;
            manager.run();
            break;
        }
        case '3': {
            BinaryTreePlayground::BinaryTreePlaygroundManager<std::string> manager;
            manager.run();
            break;
        }
        case '4': {
            BinaryTreePlayground::BinaryTreePlaygroundManager<char> manager;
            manager.run();
            break;
        }
        default: {
            std::cout << "Invalid choice! Using int by default." << std::endl;
            BinaryTreePlayground::BinaryTreePlaygroundManager<int> manager;
            manager.run();
            break;
        }
    }

    return 0;
}