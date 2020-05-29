#include <iostream>
#include <cmath>

using namespace std;
template <class T>
struct DefaultComparator {
    int operator() (const T& l, const T& r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template<class Key, class Comparator=DefaultComparator<Key>>
class AVLTree {
    struct Node {
        Node* left;
        Node* right;

        Key key;
        int height;
        int child_count;

        Node(const Key& key) : left(nullptr), right(nullptr),
                               key(key), height(1), child_count(0) {}
    };
public:
    AVLTree(Comparator comp = Comparator()) : root(nullptr), comp(comp), nodes_count(0) {}

    ~AVLTree() {
        destruct(root);
    }

    size_t size() const { return nodes_count; }


    void insert(const Key& key) {
        root = insert_aux(root, key);
    }

    void erase(const Key& key) {
        root = erase_aux(root, key);
    }

    Key get_k_stat(size_t k) {
        return get_k_stat_aux(root, k);
    }
private:
    void destruct(Node* head) {
        if (head) {
            destruct(head->left);
            destruct(head->right);
            delete(head);
        }
    }

    Key get_k_stat_aux(Node* start, size_t k) {
        int l_child_count = 0;
        if (start->left) {
            l_child_count += start->left->child_count + 1;
        }
        if (start->left && l_child_count > k) {
            return get_k_stat_aux(start->left, k);
            //go left
        } else if (start->right && l_child_count < k) {
            return get_k_stat_aux(start->right, k - (l_child_count + 1));
            //go right
        } else {
            return start->key;
        }

    }

    Node* balance(Node* node) {
        fix_height(node);
        fix_child_count(node);

        int bf = balance_factor(node);

        if (bf == 2) {
            if (balance_factor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            Node* result = rotate_left(node);
            return result;
        } else if (bf == -2) {
            if (balance_factor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            Node* result = rotate_right(node);
            return result;
        }
        return node;
    }

    int balance_factor(Node* node) {
        return height(node->right) - height(node->left);
    }
    void fix_child_count(Node* node) {
        int result = 0;
        if (node->left) {
            result += node->left->child_count + 1;
        }
        if (node->right) {
            result += node->right->child_count + 1;
        }
        node->child_count = result;

    }
    void fix_height(Node* node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    int height(Node* node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    Node* insert_aux(Node* node, const Key& key) {
        if (!node) {
            nodes_count++;
            return new Node(key);
        }
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = insert_aux(node->left, key);
        } else {
            node->right = insert_aux(node->right, key);
        }

        return balance(node);
    }

    Node* erase_aux(Node* node, const Key& key) {
        if (!node) {
            return nullptr;
        }
        int comp_res = comp(key, node->key);
        if (comp_res == -1) {
            node->left = erase_aux(node->left, key);
        } else if (comp_res == 1) {
            node->right = erase_aux(node->right, key);
        } else {
            nodes_count--;

            Node* left = node->left;
            Node* right = node->right;
            delete node;

            if (!right) {
                return left;
            }

            Node* min_node = remove_min(right);
            min_node->left = left;
            return balance(min_node);
        }
        return balance(node);
    }

    Node* remove_min(Node* node) {

        if (!node->left) {
            return node;
        }
        Node* pnode = node;
        pnode->child_count--;
        Node* cnode = node->left;
        while (cnode->left) {
            pnode = cnode;
            pnode->child_count--;
            cnode = pnode->left;
        }
        pnode->left = cnode->right;
        fix_child_count(pnode);
        cnode->right = node;
        fix_child_count(cnode);
        return cnode;
    }

    Node* rotate_right(Node* node) {
        Node* l = node->left;
        node->left = l->right;
        l->right = node;
        fix_child_count(node);
        fix_child_count(l);
        fix_height(node);
        fix_height(l);
        return l;

    }
    Node* rotate_left(Node* node) {
        Node* r = node->right;
        node->right = r->left;
        r->left = node;
        fix_child_count(node);
        fix_child_count(r);
        fix_height(node);
        fix_height(r);
        return r;

    }

    Node* root;
    Comparator comp;
    size_t nodes_count;
};

int main() {
    AVLTree<int> tree;
    int q;
    std::cin >> q;
    for (int i = 0; i < q; ++i) {
        int key, k;
        std::cin >> key >> k;
        if (key < 0) {
            tree.erase(std::abs(key));
        } else {
            tree.insert(key);
        }
        std::cout << tree.get_k_stat(k) << '\n';
    }
    return 0;
}