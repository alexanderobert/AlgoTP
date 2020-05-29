#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <stack>
#include <assert.h>

template <typename T>
struct DefaultComparator {
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

using namespace std;
template <typename T, typename Comparator = DefaultComparator<T>>
class BST {

public:
    //Выведите элементы в порядке pre-order (сверху вниз).
    BST():head(nullptr), size(0) {};

    vector<T> PreOrder() {
        Node* curr = head;
        vector<T> result;
        stack<Node*> stack;
        while (curr || !stack.empty()) {
            if (!curr) {
                curr = stack.top();
                stack.pop();
            }
            while (curr) {
                result.push_back(curr->value);
                if (curr->right_son) {
                    stack.push(curr->right_son);
                }
                curr = curr->left_son;
            }
        }
        return result;
    }
    void AddValue(T value) {
        Node* curr = head;
        if (!curr) {
            head = new Node(value);
            return;
        }
        while(true) {
            if (compare(value, curr->value)) {
                if (curr->left_son) {
                    curr = curr->left_son;
                } else {
                    curr->left_son = new Node(value);
                    break;
                }
            } else {
                if (curr->right_son) {
                    curr = curr->right_son;
                } else {
                    curr->right_son =  new Node(value);
                    break;
                }
            }
        }
    }
    ~BST() {
        destruct(head);
    }


private:
    struct Node {
        T value;
        Node* left_son;
        Node* right_son;
        explicit Node(T v): value(v), left_son(nullptr), right_son(nullptr){}
        Node& operator= (Node& othr) {
            value = othr.value;
            left_son = othr.left_son;
            right_son = othr.right_son;
            return *this;
        }
    };

    Comparator compare;
    Node* head;
    size_t size;

    void destruct(Node* tree) {//fix it
        Node *curr = head;
        stack<Node *> stack;
        while (curr || !stack.empty()) {
            if (!curr) {
                curr = stack.top();
                stack.pop();
            }
            while (curr) {
                Node *to_del = curr;
                if (curr->right_son) {
                    stack.push(curr->right_son);
                }
                curr = curr->left_son;
                delete to_del;
            }
        }
    }

};

template <typename T>
void GetTree(size_t size, BST<T>& tree) {
    for (int i = 0; i < size; ++i) {
        int value;
        cin >> value;
        tree.AddValue(value);
    }
}
/*template <typename Key>
bool DeleteByKey(Key key, HashTable htable) {
    int h = hash(key);
    Node* prev_bucket = nullptr;
    Node* bucket = htable.table[h];
    while(bucket != nullptr) {
        if (bucket.key == key) {
            if (prev_bucket) {
                prev_bucket->next = bucket->next;
            } else {
                htable.table[h] = bucket->next;
            }
            delete bucket;
            return true;
        }
        prev_bucket = bucket;
        bucket = bucket->next;
    }
    return false;
}

int MinDepth(Node* head) { // вызываем для корня дерева(лист имеет глубину 0)
    if (!head) {
        return - 1;
    }
    int l, r;
    l = MinDepth(head->left);
    r = MinDepth(head->right);
    if (l   r) {
        return l + 1;
    } else {
        return r + 1;
    }
}*/
int main() {
    BST<int, DefaultComparator<int>> tree;
    size_t size;
    cin >> size;
    GetTree(size, tree);
    for(const auto& c: tree.PreOrder()) {
        cout << c << ' ';
    }
    return 0;
}

