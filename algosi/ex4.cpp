/*
#include <iostream>
#include <memory.h>

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
struct DefaultComparator {
    bool operator() (const T& lhs, const T& rhs) const {
        return lhs > rhs;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Heap {
public:
    Heap() = default;
    explicit Heap(const microVector<T>& other_tree) {
        tree = other_tree;
        BuildHeap();

    }
    T ExtractMax() {
        int result = tree[0];
        tree[0] = tree.PopBack();
        if(!tree.IsEmpty() ) {
            SiftDown(0);
        }
        return result;

    }
    const T& PeekMax() const {
        return tree[0];
    }
    void Insert(const T& element) {
        tree.PushBack(element);
        SiftUp(tree.Size() - 1);
    }

    bool IsEmpty() {
        return tree.IsEmpty();
    }
    ~Heap() = default;
private:
    microVector<T> tree;
    Comparator compare;
    void SiftDown(int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < tree.Size() && compare(tree[left], tree[index])) {
            largest = left;
        }
        if (right < tree.Size() && compare(tree[right], tree[largest])) {
            largest = right;
        }
        if (largest != index) {
            swap(tree[largest], tree[index]);
            SiftDown(largest);
        }
    }
    void BuildHeap() {
        for(int i = (tree.Size() / 2) - 1; i >= 0; --i ) {
            SiftDown(i);
        }
    }
    void SiftUp(int index) {
        while(index > 0) {
            int parent = (index - 1) / 2;
            if(!compare(tree[index], tree[parent])) { //tree[index] <= tree[parent]
                return;
            }
            swap(tree[parent], tree[index]);
            index = parent;
        }
    }
};

template <typename T>
void get_array(microVector<T>& tree, istream& in) {
    for (int i = 0; i < tree.Size(); ++i) {
        in >> tree[i];
    }
}

int greedy(Heap<int>& heap, int power) {
    int result = 0;
    while(!heap.IsEmpty()) {
        result++;
        int current_power = power;
        microVector<int> cheker;
        while(heap.PeekMax() <= current_power && !heap.IsEmpty()) {
            current_power -= heap.PeekMax();
            cheker.PushBack(heap.ExtractMax());
        }
        for (int i = 0; i < cheker.Size(); ++i) {
            int fruit_new_weight = cheker[i] / 2;
            if(fruit_new_weight) {
                heap.Insert(fruit_new_weight);
            }
        }
    }
    return result;
}

int main() {
    int heap_size;
    cin >> heap_size;
    microVector<int> vec(heap_size);
    get_array(vec, cin);
    Heap<int> heap(vec);
    int power;
    cin >> power;
    cout << greedy(heap, power);
}*/

#include <iostream>
#include <cstring>

using namespace std;

struct elementVector {
    int index;
    int value;
};

template <typename T>
class myVector {
public:
    myVector() = default;
    explicit myVector(size_t size) : data(new T[size]), size(size), capacity(size) {}
    ~myVector() {
        delete[] data;
    }
    myVector& operator=(const myVector<T>& other) {
        delete[] data;
        data = new T[other.size];
        memcpy(data, other.data, sizeof(T) * other.size);
        this->size = other.size;
        this->capacity = other.capacity;
    }
    myVector operator=(myVector<T>&& other) = delete;

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
    T PopBack() {
        T result = data[size - 1];
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
struct DefaultComparator {
    bool operator() (int& lhs, int& rhs) {
        return lhs > rhs;
    }
};

template <typename T, typename Comparator = DefaultComparator<T>>
class Heap {
public:
    Heap() = default;
    explicit Heap(const myVector<T>& other_tree) {
        tree = other_tree;
        BuildHeap();

    }
    T ExtractMax() {
        T result = tree[0];
        tree[0] = tree.PopBack();
        if(!tree.IsEmpty() ) {
            SiftDown(0);
        }
        return result;

    }
    const T& PeekMax() const {
        return tree[0];
    }
    void Insert(const T& element) {
        tree.PushBack(element);
        SiftUp(tree.Size() - 1);
    }

    bool IsEmpty() {
        return tree.IsEmpty();
    }

    ~Heap() = default;
private:
    myVector<T> tree;
    Comparator compare;
    void SiftDown(int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left < tree.Size() && compare(tree[left].value, tree[index].value)) {
            largest = left;
        }
        if (right < tree.Size() && compare(tree[right].value, tree[largest].value)) {
            largest = right;
        }
        if (largest != index) {
            swap(tree[largest], tree[index]);
            SiftDown(largest);
        }
    }
    void BuildHeap() {
        for(int i = (tree.Size() / 2) - 1; i >= 0; --i ) {
            SiftDown(i);
        }
    }
    void SiftUp(int index) {
        while(index > 0) {
            int parent = (index - 1) / 2;
            if(!compare(tree[index].value, tree[parent].value)) { //tree[index] <= tree[parent]
                return;
            }
            swap(tree[parent], tree[index]);
            index = parent;
        }
    }
};



template <typename T>
void slidingMaximum(myVector<T>& arr, int sizeOfWindow, int size) {

    if (size == 1) {
        elementVector current = arr.PopBack();
        std::cout << current.value;
    } else {
        myVector<elementVector> window(sizeOfWindow);


        for (int i = 0; i < sizeOfWindow; i++) {
            window[i] = arr[i];
        }

        Heap<elementVector> heap(window);

        elementVector current;

        int left = 0;
        int right = sizeOfWindow;

        while (right < size) {

            elementVector rightElement = arr[right];
            heap.Insert(rightElement);
            elementVector currentMax = heap.PeekMax();


            while ((currentMax.index < left)) {
                heap.ExtractMax();
                currentMax = heap.PeekMax();
            }
            std::cout << currentMax.value << endl;

            left++;
            right++;
        }
    }
}


template <typename T>
void input(myVector<T>& arr, int size) {
    for (int i = 0; i < size; i++) {
        elementVector current;
        cin >> current.value;
        current.index = i;
        arr.PushBack(current);
    }
}

int main() {
    int size;
    std::cin >> size;

    myVector <elementVector> array;

    input(array, size);

    int sizeOfWindow;
    std::cin >> sizeOfWindow;

    slidingMaximum(array, sizeOfWindow, size);

}