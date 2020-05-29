#include <iostream>
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

int getUnimodId(microVector<int>& arr, size_t l_bound, size_t r_bound) {
    int checkid = 0;
    if(l_bound == 0 && arr[l_bound + 1] < arr[l_bound]) {
        return l_bound;
    }
    if(r_bound == arr.Size() - 1 && arr[r_bound - 1] < arr[r_bound]) {
        return r_bound;
    }
    if(arr[l_bound] > arr[l_bound - 1] && arr[r_bound] < arr[r_bound - 1]){
        while(l_bound <= r_bound) {
            checkid = (r_bound + l_bound) / 2;
            if(arr[checkid] > arr[checkid -1] && arr[checkid] > arr[checkid + 1]) {
                return checkid;
            }
            if(arr[checkid] > arr[checkid + 1] && arr[checkid] < arr[checkid - 1]) {
                r_bound = checkid - 1;
            } else {
                l_bound = checkid + 1;
            }

        }
    }
    return -1;
}

int getUnimodIdOptimization(microVector<int>& arr) {
    size_t l_bound = 1, r_bound = 2;
    int result = -1;
    while((result = getUnimodId(arr, l_bound - 1, r_bound - 1)) == -1) {
        l_bound *= 2;
        r_bound *= 2;
        if(r_bound > arr.Size() - 1) {
            r_bound = arr.Size();
        }
    }
    return result;
}

int main() {
    size_t arrays_size = 0;
    cin >> arrays_size;
    microVector<int> array(arrays_size);
    get_array(array, cin);
    cout << getUnimodIdOptimization(array);
    return 0;
}