#include <iostream>
#include <random>

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
        memcpy(data, other.data, sizeof(T) * other.size);
        this->size = other.size;
        this->capacity = other.capacity;
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
template <typename T>
struct DefaultComparator {
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs < rhs;
    }
};

int GetRandom(int min, int max) {

    std::random_device random_device; // Источник энтропии.
    std::mt19937 generator(random_device()); // Генератор случайных чисел.
    // (Здесь берется одно инициализирующее значение, можно брать больше)
    std::uniform_int_distribution<> distribution(min, max); // Равномерное распределение [10, 20]
    return distribution(generator);
}


template <typename T, typename Comparator = DefaultComparator<T>>
size_t Partition(microVector<T>& arr, size_t lb, size_t rb, Comparator compare = DefaultComparator<T>()) {
    size_t pivot = GetRandom(lb, rb);
    swap(arr[lb], arr[pivot]);
    size_t i = rb , j = rb;
    while(j != lb) {
        if(!compare(arr[j], arr[lb])) {
            swap(arr[i], arr[j]);
            i--;
            j--;
        } else {
            j--;
        };
    }
    swap(arr[lb], arr[i]);
    return i;
}

template <typename T>
size_t GetIndexValue(microVector<T>& arr, size_t index) {
    size_t lb = 0, rb = arr.Size() - 1;
    size_t check_id = 0;
    while(check_id = Partition(arr, lb, rb), check_id != index ) {
        if(check_id > index) {
            rb = check_id - 1;
        } else {
            lb = check_id + 1;
        }
    }
    return arr[check_id];
}

int main() {
    int arr_size = 0, index = 0;
    cin >> arr_size >> index;
    microVector<int> array(arr_size);
    get_array(array, cin);
    cout << GetIndexValue(array, index);
}