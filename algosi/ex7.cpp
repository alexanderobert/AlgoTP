#include <iostream>
#include <random>
#include <fstream>

using namespace std;

template <typename T>
class microVector {
public:
    microVector() = default;
    explicit microVector(size_t size) : data(new T[size]), size(size), capacity(size) {}
    ~microVector() {
        delete[] data;
    }
    microVector& operator=(const microVector<T>& other) {
        delete[] data;
        data = new T[other.size];
        this->size = other.size;
        this->capacity = other.capacity;
        for (int i = 0; i < this->size; ++i) {
            this->data[i] = other.data[i];
        }
    }
    microVector operator=(microVector<T>&& other) = delete;

    T& operator[](size_t index) {
        return data[index];
    }
    T& operator[](size_t index) const  {
        return data[index];
    }

    size_t Size() const {
        return size;
    }
    size_t Capacity() const {
        return capacity;
    }
    void Reserve(size_t cap) {
        auto new_cap = cap;
        auto new_data = new T[new_cap];
        copy(data, data + size, new_data);
        delete[] data;
        data = new_data;
        capacity = new_cap;

    }
    void PushBack(const T& value) {
        if (size >= capacity) {
            this->Reserve((capacity == 0) ? 1 : 2 * capacity);
        }
        data[size++] = value;
    }
    T PopBack() {
        T result = data[size -1];
        size--;
        return result;
    }
    bool IsEmpty() {
        return size == 0;
    }

private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;
};


template <typename T>
void get_array(microVector<T>& tree, istream& in) {
    for (int i = 0; i < tree.Size(); ++i) {
        in >> tree[i];
    }
}

size_t BinaryPartition(const microVector<uint64_t>& array, int part_bit, size_t lb, size_t rb) {
    size_t i = lb , j = lb;
    while(j < rb) {
        uint64_t check = array[j];
        if(array[j] & (1 << part_bit)) {
            j++;
        } else {
            swap(array[i], array[j]);
            i++;
            j++;

        };
    }
    return i;
}

void BinaryMSD(const microVector<uint64_t>& array, int sort_bit, size_t lb, size_t rb) {
    if ((sort_bit < 0) || rb == lb) {
        return;
    }
    size_t new_bound  = BinaryPartition(array, sort_bit, lb, rb);
    BinaryMSD(array, sort_bit - 1, lb, new_bound);
    BinaryMSD(array, sort_bit - 1, new_bound, rb);
}

bool IsSorted(microVector<uint64_t >& arr) {
    for (int i = 1; i < arr.Size(); ++i) {
        if(arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    size_t size;
    cin >> size;
    microVector<uint64_t> array(size);
    get_array(array, cin);
    microVector<uint64_t > test;
    test = array;
    BinaryMSD(array, 63, 0, array.Size());
    for (int i = 0; i < array.Size(); ++i) {
        cout << array[i] << ' ';
    }
    return 0;
}