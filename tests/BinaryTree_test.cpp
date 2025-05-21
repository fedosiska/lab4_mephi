#include <cassert>
#include <iostream>
#include <vector>
#include "../include/BinarySearchTree.hpp"

template<typename T>
void assert_equal(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i) assert(a[i] == b[i]);
}

void test_BST_basic()
{
    BinarySearchTree<int> bst;
    for (int x : {10, 5, 15, 2, 7, 12, 18}) bst.Insert(x);

    assert(bst.Size() == 7);

    std::string pre = bst.ToString();
    std::string in  = bst.ToString(BinarySearchTree<int>::Order::In);

    assert(pre == "(10 (5 (2 _ _) (7 _ _)) (15 (12 _ _) (18 _ _)))");
    assert(in  == "(((_ 2 _) 5 (_ 7 _)) 10 ((_ 12 _) 15 (_ 18 _)))");

    auto squares = bst.Map([](int x){ return x * x; });
    assert_equal<int>(squares, {4, 25, 49, 100, 144, 225, 324});
}

void test_BST_where_reduce()
{
    BinarySearchTree<int> bst;
    for (int x : {10, 5, 15, 2, 7, 12, 18}) bst.Insert(x);

    auto gt10 = bst.Where([](int x){ return x > 10; });
    assert_equal<int>(gt10, {12, 15, 18});

    int sum = bst.Reduce<int>([](int s,int x){ return s + x; }, 0);
    assert(sum == 69);
}

void test_BST_extract(){
    BinarySearchTree<int> bst;
    for (int x : {10, 5, 15, 2, 7, 12, 18}) bst.Insert(x);

    auto sub = bst.ExtractSubtree(5);
    assert(sub.Size() == 3);
    assert(sub.ToString() == "(5 (2 _ _) (7 _ _))");

    assert(bst.Size() == 7);
}

int main(){
    test_BST_basic();
    test_BST_where_reduce();
    test_BST_extract();

    std::cout << "All BinarySearchTree tests passed!\n";
    return 0;
}
