cmake_minimum_required(VERSION 3.31)
project(LiOAvIZ_Lab4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

add_subdirectory(lib/binarytree)
add_subdirectory(lib/tui)

add_executable(LiOAvIZ_Lab4
        src/main.cpp
)

target_include_directories(LiOAvIZ_Lab4 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(LiOAvIZ_Lab4
        BinaryTree
        Playground
)

target_compile_options(LiOAvIZ_Lab4 PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wshadow>
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>

        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wnull-dereference -Wdouble-promotion>
)

target_compile_options(LiOAvIZ_Lab4 PRIVATE
        $<$<CONFIG:Debug>:
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O0 -g3 -fno-omit-frame-pointer>
        $<$<CXX_COMPILER_ID:MSVC>:/Od /Zi /Ob0>
        >
)

target_compile_options(LiOAvIZ_Lab4 PRIVATE
        $<$<CONFIG:Release>:
        $<$<CXX_COMPILER_ID:GNU,Clang>:-O3 -flto -DNDEBUG -march=native>
        $<$<CXX_COMPILER_ID:MSVC>:/O2 /Ob2 /DNDEBUG>
        >
)

target_link_options(LiOAvIZ_Lab4 PRIVATE
        $<$<CONFIG:Release>:
        $<$<CXX_COMPILER_ID:GNU,Clang>:-flto>
        >
)

set_target_properties(LiOAvIZ_Lab4 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)


