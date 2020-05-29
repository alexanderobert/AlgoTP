#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
class microDec {
public:
    microDec():tail(nullptr), head(nullptr) {};

    microDec& operator = (const microDec&) = delete;
    microDec& operator = (microDec&&) = delete;

    bool is_empty() const{
        return  head == tail && tail == nullptr;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (is_empty()) {
            tail = node;
            head = node;
            return;
        }
        node->prev = tail;
        tail->next = node;
        tail = tail->next;
    }
    void push_front(const T& value) {
        Node* node = new Node(value);
        if (is_empty()) {
            tail = node;
            head = node;
            return;
        }
        head->prev = node;
        node->next = head;
        head = head->prev;
    }
    T pop_front() {
        T value = head->value;
        Node* node = head;
        head = head->next;
        delete node;

        if(head == nullptr) {
            tail = nullptr;
        }
        return value;
    }
    T pop_back() {
        T value = tail->value;
        Node* node = tail;
        tail = tail->prev;
        delete node;

        if(tail == nullptr) {
            head = nullptr;
        }
        return value;
    }

    ~microDec() {
        while (!is_empty()) {
            pop_front();
        }
    }
private:
    struct Node{
        Node* prev;
        Node* next;
        T value;
        explicit Node(const T& val) : value(val), prev(nullptr), next(nullptr) {
        }
    };
    Node* head;
    Node* tail;
};


int main() {
    int req_count = 0;
    cin >> req_count;
    microDec<int> dec;
    for (int i = 0; i < req_count; ++i) {
        int req = 0, value = -1, q_value = -1;
        cin >> req >> value;
        switch(req) {
            case 1: {  // push front
                dec.push_front(value);
                break;
            }
            case 2: {  // pop front
                if (dec.is_empty()) {
                    q_value = -1;
                } else {
                    q_value = dec.pop_front();
                }
                if (q_value != value) {
                    cout << "NO" << endl;
                    return 0;
                }
                break;
            }
            case 3: {  // push back
                dec.push_back(value);
                break;
            }
            case 4: {  // pop back
                if (dec.is_empty()) {
                    q_value = -1;
                } else {
                    q_value = dec.pop_back();
                }
                if (q_value != value) {
                    cout << "NO" << endl;
                    return 0;
                }
                break;
            }
            default: {
                cout << "NO";
                return 0;
            }
        }
    }
    cout << "YES";
    return 0;
}