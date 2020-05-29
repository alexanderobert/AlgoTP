#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

template <typename T>
class microVector {
public:
    microVector() = default;
    explicit microVector(size_t size) : data(new T[size]), size(size), capacity(size) {}
    ~microVector() {
        delete[] data;
    }

    T& operator[](size_t index) {
        return data[index];
    }
    const T& operator[](size_t index) const  {
        return data[index];
    }

    size_t Size() const {
        return size;
    }
    size_t Capacity() const {
        return capacity;
    }
    void PushBack(const T& value) {
        if (size >= capacity) {
            auto new_cap = (capacity == 0) ? 1 : 2 * capacity;
            auto new_data = new T[new_cap];
            copy(data, data + size, new_data);
            delete[] data;
            data = new_data;
            capacity = new_cap;
        }
        data[size++] = value;
    }

private:
    T* data = nullptr;
    size_t size = 0;
    size_t capacity = 0;
};

template <typename T>
void get_array(microVector<T>& arr, istream& in) {
    for (int i = 0; i < arr.Size(); ++i) {
        in >> arr[i];
    }
}
pair<size_t, size_t > maxSummId(const microVector<int>& first_arr, const microVector<int>& second_arr) {

    size_t max_i = 0, potencial_max_i = 0, max_j = 0;
    int max_sum = 0;
    for (size_t k = 0; k < first_arr.Size(); ++k) {
        if (first_arr[k] > first_arr[potencial_max_i]) {
            potencial_max_i = k;
        }
        if((first_arr[potencial_max_i] + second_arr[k]) > max_sum) {
            max_i = potencial_max_i;
            max_j = k;
            max_sum = first_arr[potencial_max_i] + second_arr[k];
        }
    }
    return  {max_i, max_j};
}

int main() {
    size_t arrays_size = 0;
    pair<size_t, size_t > result;
    cin >> arrays_size;
    microVector<int> first_arr(arrays_size);
    microVector<int> second_arr(arrays_size);
    get_array(first_arr, cin);
    get_array(second_arr, cin);
    result =  maxSummId(first_arr, second_arr);
    cout << result.first << ' ' << result.second << '\n';

    vector<int> a = {1, 2, 3, 4};
    vector<int> b(a);
    return 0;
}
