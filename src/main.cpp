#include <iostream>
#include "../include/BinarySearchTree.hpp"

using std::cout;
using std::cin;

int main() {
    BinarySearchTree<int> bst;
    int keys[] = { 10, 5, 15, 2, 7, 13, 20 };
    for (int k : keys) bst.Insert(k);

    std::cout << "=== левый обход ===\n";
    bst.PrintLeftOrder();

    auto sq = bst.Map([](int x){ return x * x; });
    for (int v: sq) std::cout<< v <<' ';
    cout<<'\n';

    auto greater8 = bst.Where([](int x){ return x > 8; });
    for (int v : greater8) std::cout << v << ' ';
    cout<<'\n';

    int sum = bst.Reduce<int>( [](int s,int x){ return s + x; }, 0);
    cout<< sum <<'\n';

    cout << "Size after inserts: " << bst.Size() << '\n';
    cout << "InOrder : ";
    for (int v : bst) 
        cout << v << ' ';
    cout << '\n';

    cout << "PreOrder: ";
    for (auto it = bst.begin(BinarySearchTree<int>::TraversalOrder::PreOrder);
         it != bst.end(); ++it)
        cout << *it << ' ';
    cout << '\n';

    int q = 7;
    cout << "Contains " << q << "? "
         << std::boolalpha << bst.Contains(q) << '\n';

    bst.Remove(10);
    bst.Remove(2); 

    cout << "After removals, size = " << bst.Size() << '\n';
    cout << "InOrder  : ";
    for (int v : bst) cout << v << ' ';
    cout << '\n';

    cout << "Empty? " << bst.Empty() << '\n';

    while (true) {
        cout << "\nMenu: 1-insert  2-remove  3-print  0-exit : ";
        int cmd; if (!(cin >> cmd)) break;
        if (cmd == 0) break;
        if (cmd == 1) {
            int x; cout << "  key = "; cin >> x;
            bst.Insert(x);
        } else if (cmd == 2) {
            int x; cout << "  key = "; cin >> x;
            bst.Remove(x);
        } else if (cmd == 3) {
            cout << "  InOrder : ";
            for (int v : bst) cout << v << ' ';
            cout << '\n';
        }
    }

    return 0;
}
