#include <iostream>
#include <vector>
#include <stack>

using namespace std;

template <typename T>
struct DefaultComparator {
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};


template <typename T, typename Comparator = DefaultComparator<T>>
class BST {
public:
    BST(): head(nullptr){};

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
    int WidestLayer() {
        if(!head) {
            return 0;
        }
        vector<Node*> curr;
        vector<Node*> next;
        int result = 1;
        next.push_back(head);
        do {
            curr = next;
            next.clear();
            int local_result = 0;
            for(const auto& node: curr) {
                if (node->left_son) {
                    next.push_back(node->left_son);
                    local_result++;
                }
                if (node->right_son) {
                    next.push_back(node->right_son);
                    local_result++;
                }
            }

            if (local_result > result) {
                result = local_result;
            }
        } while (!next.empty());
        return result;
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
    void destruct(Node* tree) {
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

    Comparator compare;
    Node* head;

};

template <typename T, typename Comparator = DefaultComparator<T>>
class DST {
public:
    DST(): head(nullptr){};
    void Addvalue(T value, int priority) {
        if (!head) {
            head = new Node(value, priority);
            return;
        }
        if(head->priority < priority) {
            InsertInstead(&head, value, priority);
            return;
        }
        Node* curr = head;
        while (curr) {
            if (compare(value, curr->value)) {
                if (curr->left_son) {
                    if (curr->left_son->priority < priority) {
                       InsertInstead(&curr->left_son, value, priority);
                       return;
                    } else {
                        curr = curr->left_son;
                    }
                } else {
                    curr->left_son = new Node(value, priority);
                    return;
                }
            } else {
                if (curr->right_son) {
                    if (curr->right_son->priority < priority) {
                        InsertInstead(&curr->right_son, value, priority);
                        return;
                    } else {
                        curr = curr->right_son;
                    }
                } else {
                    curr->right_son = new Node(value, priority);
                    return;
                }
            }
        }

    }

    int WidestLayer() {
        if(!head) {
            return 0;
        }
        vector<Node*> curr;
        vector<Node*> next;
        int result = 1;
        next.push_back(head);
        do {
            curr = next;
            next.clear();
            int local_result = 0;
            for(const auto& node: curr) {
                if (node->left_son) {
                    next.push_back(node->left_son);
                    local_result++;
                }
                if (node->right_son) {
                    next.push_back(node->right_son);
                    local_result++;
                }
            }

            if (local_result > result) {
                result = local_result;
            }
        } while (!next.empty());
        return result;
    }
    ~DST() {
        destruct(head);
    }

private:

    struct Node {
        T value;
        int priority;
        Node* left_son;
        Node* right_son;
        explicit Node(T v, int pr): value(v), left_son(nullptr), right_son(nullptr), priority(pr){}
        Node& operator= (Node& othr) {
            value = othr.value;
            left_son = othr.left_son;
            right_son = othr.right_son;
            return *this;
        }
    };
    void destruct(Node* tree) {
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

    void Split(Node* currentNode, int value, Node** left, Node** right ) {
        if (!currentNode) {
            *left = nullptr;
            *right = nullptr;
        } else if (!compare(value, currentNode->value)) {
            Split(currentNode->right_son, value, &currentNode->right_son, right);
            *left = currentNode;
        } else {
            Split(currentNode->left_son, value, left, &currentNode->left_son);
            *right = currentNode;
        }
    }
    Node* Merge(Node* left, Node* right ) {
        if (!left || !right) {
            if (left) {
                return left;
            } else {
                return right;
            }
        }
        if (left->priority > right->priority) {
            left->right_son = Merge( left->right_son, right );
            return left;
        } else {
            right->left_son = Merge( left, right->left_son );
            return right;
        }
    }

    void InsertInstead(Node** place, T value, int priority) {
        Node* new_node = new Node(value, priority);
        Split(*place, value, &new_node->left_son, &new_node->right_son);
        *place = new_node;
    }
    Comparator compare;
    Node* head;

};


int main() {
    BST<int> bst;
    DST<int> dst;
    int q;
    cin >> q;
    for (int j = 0; j < q; ++j) {
        int value, prior;
        cin >> value >> prior;
        bst.AddValue(value);
        dst.Addvalue(value, prior);
    }
    cout << dst.WidestLayer() - bst.WidestLayer();
    return 0;
}