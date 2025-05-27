#include <cassert>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include "../include/BinarySearchTree.hpp"

template<typename T>
void assert_equal(const std::vector<T>& a, const std::vector<T>& b)
{
    assert(a.size() == b.size());
    for (size_t i = 0; i < a.size(); ++i)  assert(a[i] == b[i]);
}

template<typename T>
std::vector<T> extract_keys(const std::string& s)
{
    std::vector<T> res;
    std::string tok;
    for (char ch : s)
    {
        if (std::isalnum(static_cast<unsigned char>(ch)))
            tok += ch;
        else {
            if (!tok.empty()) {
                std::stringstream ss(tok);  T val; ss >> val;
                res.push_back(val);
                tok.clear();
            }
        }
    }
    if (!tok.empty()) { std::stringstream ss(tok); T v; ss >> v; res.push_back(v); }
    return res;
}

const std::vector<int> seq_KLP = {10,5,2,7,15,12,18};
const std::vector<int> seq_KPL = {10,15,18,12,5,7,2};
const std::vector<int> seq_LKP = {2,5,7,10,12,15,18};
const std::vector<int> seq_LPK = {2,7,5,12,18,15,10};
const std::vector<int> seq_PKL = {18,15,12,10,7,5,2};
const std::vector<int> seq_PLK = {18,12,15,7,2,5,10};

void test_BST_int_observations()
{
    BinarySearchTree<int> bst;
    for (int x : {10, 5, 15, 2, 7, 12, 18}) bst.Insert(x);
    assert(bst.Size() == 7);

    using Trav = BinarySearchTree<int>::Traversal;

    assert_equal( extract_keys<int>(bst.ToString(Trav::KLP)), seq_KLP );
    assert_equal( extract_keys<int>(bst.ToString(Trav::KPL)), seq_KPL );
    assert_equal( extract_keys<int>(bst.ToString(Trav::LKP)), seq_LKP );
    assert_equal( extract_keys<int>(bst.ToString(Trav::LPK)), seq_LPK );
    assert_equal( extract_keys<int>(bst.ToString(Trav::PKL)), seq_PKL );
    assert_equal( extract_keys<int>(bst.ToString(Trav::PLK)), seq_PLK );
}

void test_BST_map_where_reduce()
{
    BinarySearchTree<int> bst;
    for (int x : {10, 5, 15, 2, 7, 12, 18}) bst.Insert(x);

    auto squares = bst.Map([](int x){ return x * x; });
    assert_equal<int>(squares, {4,25,49,100,144,225,324});

    auto gt10 = bst.Where([](int x){ return x > 10; });
    assert_equal<int>(gt10, {12,15,18});

    int sum = bst.Reduce<int>([](int s,int x){ return s + x; }, 0);
    assert(sum == 69);
}

void test_BST_extract()
{
    BinarySearchTree<int> bst;
    for (int x : {10,5,15,2,7,12,18}) bst.Insert(x);

    auto sub = bst.ExtractSubtree(5);
    assert(sub.Size() == 3);

    using Trav = BinarySearchTree<int>::Traversal;
    assert(sub.ToString(Trav::KLP) == "(5 (2 _ _) (7 _ _))");
    assert(bst.Size() == 7);
}

void test_BST_string()
{
    BinarySearchTree<std::string> tree;
    for (std::string s : {"dog","cat","elephant","bear","ant"})
        tree.Insert(s);

    std::vector<std::string> expected = {"ant","bear","cat","dog","elephant"};
    auto inorder = extract_keys<std::string>(
        tree.ToString(BinarySearchTree<std::string>::Traversal::LKP));
    assert_equal<std::string>(inorder, expected);

    auto lens = tree.Map([](const std::string& s){ return (int)s.size(); });
    assert_equal<int>(lens, {3,4,3,3,8});

    std::string cat = tree.Reduce<std::string>(
        [](std::string acc,const std::string& s){ return acc + s + "-"; }, "");
    assert(cat == "ant-bear-cat-dog-elephant-");
}

int main()
{
    test_BST_int_observations();
    test_BST_map_where_reduce();
    test_BST_extract();
    test_BST_string();

    std::cout << "All BinarySearchTree tests passed!\n";
    return 0;
}
