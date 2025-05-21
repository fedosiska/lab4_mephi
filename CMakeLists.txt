cmake_minimum_required(VERSION 3.10)
project(Laba_4)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(main
    src/main.cpp
)

add_executable(test_tree
    tests/BinaryTree_test.cpp
)