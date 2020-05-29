#include <iostream>
#include <fstream>
using namespace std;

template <typename T>
class microDec {
public:
    explicit microDec(size_t init_capacity) {
        size = 0;
        start = 0;
        capacity = init_capacity;
        data = new T[init_capacity];
    };

    microDec& operator = (const microDec&) = delete;
    microDec& operator = (microDec&&) = delete;

    bool is_empty() const{
        return size == 0;
    }

    void push_back(const T& value) {
        if (size == capacity) {
            BufferExpansion();
        }
        int back_idx = (start + size) % capacity;
        data[back_idx] = value;
        size++;

    }
    void push_front(const T& value) {
        if (size == capacity) {
            BufferExpansion();
        }
        start--;
        if(start < 0) {
            start += capacity;
        }
        data[start] = value;
        size++;
    }
    T pop_front() {
        T result = data[start];
        start++;
        if (start >= capacity) {
           start = 0;
        }
        size--;
        return result;
    }
    T pop_back() {
        int back_idx = (start + size - 1) % capacity;
        T result = data[back_idx];
        size--;
        return result;
    }

    ~microDec() {
        delete[] data;
    }
private:
    void BufferExpansion() {
        T* new_data = new T[capacity * 2];
        int back_idx = (start + size - 1) % capacity;
        if(back_idx >= start) {
            for (int i = start; i <= back_idx; ++i) {
                new_data[i] = data[i];
            }
        } else {
            int j = 0;
            for (j = start; j < capacity; ++j) {
                new_data[j] = data[j];
            }
            for (int i = 0; i <= back_idx; ++i) {
                new_data[j + i] = data[i];
            }
        }
        capacity *= 2;
        delete[] data;
        data = new_data;
    }
    T* data;
    size_t capacity;
    size_t size;
    int start;


};


int main() {
    int req_count = 0;
    cin >> req_count;
    microDec<int> dec(1);
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