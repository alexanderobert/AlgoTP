#include <iostream>
#include <memory.h>

using namespace std;

struct Point {
    int position_;
    bool is_open_;
    Point() = default;

    Point(int value, bool is_open) {
        position_ = value;
        is_open_ = is_open;
    }
    bool operator<(const Point& other) const{
        return this->position_ < other.position_;
    }
};

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

template <typename T, typename Comparator = DefaultComparator<T>>
void Merge(microVector<T>& arr, size_t lb, size_t mb, size_t rb, Comparator compare) {
    size_t lpos = lb, rpos = mb + 1;

    microVector<T> buff;
    buff.Reserve(rb - lb + 1);
    while(lpos <= mb && rpos <= rb) {
        if(compare(arr[lpos], arr[rpos])) {
            buff.PushBack(arr[lpos]);
            lpos++;
        } else {
            buff.PushBack(arr[rpos]);
            rpos++;
        }
    }
    while(lpos <= mb) {
        buff.PushBack(arr[lpos]);
        lpos++;
    }
    while (rpos <= rb){
        buff.PushBack(arr[rpos]);
        rpos++;
    }
    for (int i = 0; i < buff.Size(); ++i) {
        arr[lb + i] = buff[i];
    }
}

template <typename T, typename Comparator = DefaultComparator<T>>
void MergeSort(microVector<T>& arr, size_t lb, size_t rb, Comparator compare = DefaultComparator<T>()) {
    if(rb > lb) {
        size_t mb = (rb + lb) / 2;
        MergeSort(arr, lb, mb);
        MergeSort(arr, mb + 1, rb);
        Merge(arr, lb, mb, rb, compare);
    }
}

int PointsPaintOnce(const microVector<Point>& lines) {
    int result = 0;
    int layers = 0;
    int left_bound = -1;
    for (int i = 0; i < lines.Size(); ++i) {
        if(layers == 1) {
            result += (lines[i].position_ - left_bound);
        }
        left_bound = lines[i].position_;
        if(lines[i].is_open_) {
            layers++;
        } else {
            layers--;
        }
    }
    return result;
}
int main() {
    size_t line_number = 0;
    cin >> line_number;
    microVector<Point> lines;
    for (size_t i = 0; i < line_number; ++i) {
        int position = 0;
        cin >> position;
        lines.PushBack({position, true});
        cin >> position;
        lines.PushBack({position, false});
    }
    MergeSort(lines, 0, lines.Size() - 1);
    cout << PointsPaintOnce(lines);

return 0;
}
