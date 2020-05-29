#include <iostream>
#include <cstring>

template <typename T, typename Comparator = std::less<T>>
class Heap {
public:
    Heap(int size): arrSize(size){
        arr = new T[arrSize];
    }

    ~Heap(){
        delete[] arr;
    };

    int PeekMaxK(int k){
        siftUp(k - 1);
        siftDown(0, k);
    }

    void heapCopy(int* part, int k){
        std::memcpy(arr, part, sizeof(int) * k );
    }

    void print(int k){
        std::cout<<arr[0]<<" ";
    }
private:
    int* arr;
    int arrSize;
    Comparator compare;
    void siftUp(int i){
        while (i > 0){
            int parent = (i - 1) / 2;
            if (arr[i] <= arr[parent])
                return;
            std::swap(arr[i], arr[parent]);
            i = parent;
        }
    }

    void siftDown(int i, int k) {
        while (i <= k) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;
            if ((left <= k) && (arr[left] > arr[i])){
                largest = left;
            }
            else{
                largest = i;
            }
            if ((right <= k) && (arr[right] > arr[largest]))
                largest = right;
            if (largest != i) {
                std::swap(arr[i], arr[largest]);
                i = largest;
            } else{
                break;
            }
        }
    }
};

void maximum(int* inputArr, int size, int k){
    if(size == 1){
        std::cout<<inputArr[0];
    }

    else {
        int left = 0;
        int right = 0;
        int i = 0;

        while (right < size - 1) {
            left = i;
            right = i + k - 1;
            int *part = inputArr + left;
            Heap *heap = new Heap(k);

            heap->heapCopy(part, k);
            heap->PeekMaxK(k);
            heap->print(k);

            delete heap;
            i++;
        }
    }
}

int main(){
    int size = 0;
    std::cin >> size;

    int* inputArr = new int[size];
    for (int i = 0; i < size; i++){
        std::cin >> inputArr[i];
    }

    int k;
    std::cin>>k;

    maximum(inputArr, size, k);

    delete [] inputArr;

    return 0;
}

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
            size_t new_cap = (capacity == 0) ? 1 : 2 * capacity; //сделай тут через обычный иф
            T* new_data = new T[new_cap];
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