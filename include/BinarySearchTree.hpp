#pragma once
#include <functional>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <type_traits>
#include <sstream>
#include <stack>


template<typename T, typename Compare = std::less<T>>
class BinarySearchTree{
    private:
        struct Node{
            T key;
            Node* left = nullptr;
            Node* right = nullptr;
            Node* parent = nullptr;
            int height = 1;

            explicit Node(const T& k) : key(k) {}
        };

        Node* root = nullptr;
        size_t count = 0;
        Compare cmp;

        int h(Node* n) const{ return n ? n->height : 0; }

        void update(Node* n){
            if (!n) return;
            n->height = 1 + std::max(h(n->right), h(n->left));
        }

        int balanceFactor(Node* n) const {
            return h(n->right) - h(n->left);
        }
        Node* rotateLeft(Node* x){
            Node* y = x->right;
            x->right = y->left;
            if (y->left) y->left->parent = x;
            y->left = x;
            y->parent = x->parent;
            x->parent = y;
            update(x);
            update(y);
            return y;
        }
        Node* rotateRight(Node* y){
        Node* x = y->left;
        y->left = x->right;
        if (x->right) x->right->parent = y;
        x->right = y;
        x->parent = y->parent;
        y->parent = x;
        update(y);
        update(x);
        return x;
        }
        Node* rebalance(Node* n){
            update(n);
            int bf = balanceFactor(n);
            if (bf>1 && balanceFactor(n->right)>=0){
                return rotateLeft(n);
            }
            if (bf>1 && balanceFactor(n->right)<0){
                n->right = rotateRight(n->right);
                return rotateLeft(n);
            }
            if (bf<-1 && balanceFactor(n->left)<=0){
                return rotateRight(n);
            }
            if(bf<-1 && balanceFactor(n->left)>0){
                n->left = rotateLeft(n->left);
                return rotateRight(n);
            }
            return n;
        }
        Node* insert(Node* cur, const T& x, Node* parent){
            if(!cur){
                ++count;
                cur = new Node(x);
                cur->parent = parent;
                return cur;
            }
            if(cmp(x, cur->key)) cur->left = insert(cur->left, x, cur);
            else if (cmp(cur->key, x)) cur->right = insert(cur->right,x,cur);

            return rebalance(cur);
        }
        Node* find(Node* cur, const T& x) const {
            if(!cur) return nullptr;
            if(cmp(x,cur->key)) return find(cur->left,x);
            else if (cmp(cur->key,x)) return find(cur->right,x);
            else{
                return cur;
            }
        }
        Node* minNode(Node* n) const {
            while(n && n->left) n = n->left;
            return n;
        }
        Node* erase(Node* cur, const T& x){
            if (!cur) return nullptr;
            if(cmp(x,cur->key)) cur->left = erase(cur->left,x);
            else if(cmp(cur->key, x)) cur->right = erase(cur->right,x);
            else{
                --count;
                if(!cur->left || !cur->right){
                    Node* tmp = cur->left ? cur->left : cur->right;
                    if (tmp) tmp->parent = cur->parent;
                    delete cur;
                    return tmp;
                }
            }
            Node* succ = minNode(cur->right);
            cur->key = succ->key;
            cur->right = erase(cur->right,succ->key);
            return rebalance(cur);
        }
        void printNode(Node* cur) const{
            if (!cur) return;
            print(cur->left);
            std::cout << cur->key << ' ';
            print(cur->right);
        }
        void destroy(Node* cur){
            if (!cur) return;
            destroy(cur->left);
            destroy(cur->right);
            delete(cur);
        }
    public:
        BinarySearchTree() = default;
        ~BinarySearchTree() {destroy(root);}
        size_t Size() const { return count;}
        bool Empty() const {return count == 0;}
        void Insert(const T& x) { root = insert(root,x,nullptr);}
        bool Contains(const T& x) const { return find(root,x) != nullptr;}
        void Remove(const T& x) {root = erase(root,x);}
        void PrintInOrder() const { print(root); std::cout << '\n';}

        enum class TraversalOrder { InOrder, PreOrder };
        
        class Iterator {
            Node* current = nullptr;
            std::vector<Node*> st;
            TraversalOrder ord;
            
            void pushLeft(Node* n){
                while (n) { st.push_back(n); n = n->left; }
            }
        
            public:
                Iterator(Node* root, TraversalOrder o) : ord(o){
                    if (!root) return;
                    if (ord == TraversalOrder::PreOrder)
                        st.push_back(root);
                    else
                        pushLeft(root);
                    ++(*this); 
                }
                T& operator*() const { return current->key; }
        
                bool operator!=(const Iterator& other) const{ 
                    return current != other.current; 
                }
                Iterator& operator++(){
                    if (st.empty()) { 
                        current = nullptr; 
                        return *this;
                    }
                    Node* n = st.back(); st.pop_back();
                    if (ord == TraversalOrder::PreOrder) {
                        if (n->right) st.push_back(n->right);
                        if (n->left)  st.push_back(n->left);
                        current = n;
                    } else {
                        pushLeft(n->right);
                        current = n;
                    }
                    return *this;
                }
        };
        
        
        Iterator begin(TraversalOrder ord = TraversalOrder::InOrder) const{
            return Iterator(root, ord); }
        Iterator end() const{ 
            return Iterator(nullptr, TraversalOrder::InOrder); }
        
        template<typename Func>
        auto Map(Func f) const{
            using ResT = decltype(f(std::declval<T>()));
            std::vector<ResT> out;
            std::function<void(Node*)> dfs = [&](Node* n){
                if (!n) return;
                dfs(n->left);
                out.push_back(f(n->key));
                dfs(n->right);
            };
            dfs(root);
            return out;
        }
        template<typename Pred>
        std::vector<T> Where(Pred p){
            std::vector<T> out;
            std::function<void(Node*)> dfs = [&](Node* n) {
                if (!n) return;
                dfs(n->left);
                if (p(n->key))
                    out.push_back(n->key);
                dfs(n->right);
            };
            dfs(root);
            return out;    
        }
        template<typename AccT, typename Func>
        AccT Reduce(Func f, AccT acc) const {
            std::function<void(Node*)> dfs = [&](Node* n) {
                if (!n) return;
                dfs(n->left);
                acc = f(acc, n->key);
                dfs(n->right);
            };
            dfs(root);
            return acc;
        }
        enum class Order {Pre, In };
        std::string ToString(Order ord = Order::Pre) const {

            std::ostringstream out;
            std::function<void(Node*)> dfs = [&](Node* n){
                if (!n) { out << "_"; return; }

                out <<"(";

                if(ord == Order::Pre){
                    out << n->key << ' '; // K
                    dfs(n->left);  out << ' ';
                    dfs(n->right);
                }
                else{
                    dfs(n->left); out<< ' ';
                    out<< n->key<< ' ';
                    dfs(n->right);
                }

                out << ')';
            };
            dfs(root);
            return out.str();
        }
        BinarySearchTree ExtractSubtree(const T& key) {
            BinarySearchTree out;
            Node* start = find(root, key);
            if (!start) return out;

            std::stack<Node*> st;
            st.push(start);

            while (!st.empty()) {
                Node* cur = st.top(); 
                st.pop();
                out.Insert(cur->key);
                if (cur->right) st.push(cur->right);
                if (cur->left)  st.push(cur->left);
            }
            return out;
        }

        void PrintLeftOrder()  const { 
            std::function<void(Node*)> dfs = [&](Node* n){
                if (!n) return;
                dfs(n->left);
                std::cout << n->key << '\n';
                dfs(n->right);
            };
            dfs(root);
        }

        void PrintRightOrder() const {
            std::function<void(Node*)> dfs = [&](Node* n){
                if (!n) return;
                dfs(n->right);
                std::cout << n->key << '\n';
                dfs(n->left);
            };
            dfs(root);
        }

};