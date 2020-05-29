#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <cstdlib>
#include <assert.h>

#define INIT_VALUE 8

using namespace std;

template <class T> struct HashFunc;

template<> struct HashFunc<int> {
    size_t operator() (const int& key) { return key; }
};

template<> struct HashFunc<std::string> {
    size_t operator() (const std::string& key) {
        size_t result = 0;
        for(const auto& c: key) {
            result = result * 17 + c;
        }
        return result;
    }
};

template <class T> struct EqualsFunc {
    bool operator() (const T& l, const T& r) const { return l == r; }
};


template <class Key, class Value, class Hash = HashFunc<Key>, class Equals = EqualsFunc<Key>>
class HashTable {
    static constexpr double max_load_factor = 0.75;

public:
    HashTable(Key del, Key empt, size_t capacity = INIT_VALUE, Hash hash = Hash(), Equals equals = Equals())
            : items_count(0), size_idx(capacity)
            , hash(hash), equals(equals)
            , del_value(move(del)), empty_value(move(empt)), data(capacity, {empt, ""})
            {

            }
    HashTable& operator=(HashTable<Key, Value>&) = delete;
    HashTable& operator=(HashTable<Key, Value>&& new_table){
        this->del_value = move(new_table.del_value);
        this->empty_value = move(new_table.empty_value);
        this->data = move(new_table.data);
        this->items_count = move(new_table.items_count);
        this->size_idx = move(new_table.size_idx);
        return *this;
    }
    ~HashTable() = default;

    Value* find(const Key& key) {
        size_t real_index = hash(key) % size_idx;
        for (size_t i = 0; i < size_idx; ++i) {
            if(equals(data[real_index].first, key)) {
                return &data[real_index].second;
            }
            if (equals(data[real_index].first, empty_value)) {
                return nullptr;
            }
            real_index = clustering(real_index, i);
        }
        return nullptr;
    }

    bool insert(const Key& key, const Value& value) {
        if(IsKeyInTable(key)) {
            return false;
        }
        if(items_count > max_load_factor * size_idx) {
            grow();
        }
        return insert_item({key, value});
    }

    bool erase(const Key& key) {
        size_t real_index = hash(key) % size_idx;
        for (size_t i = 0; i < size_idx; ++i) {
            if (equals(data[real_index].first, empty_value)) {
                return false;
            }

            if (equals(data[real_index].first, key)) {
                data[real_index].first = del_value;
                return true;
            }

            real_index = clustering(real_index, i);
        }
        return false;

    }

private:
    size_t clustering(size_t k, size_t i) {
        return (k + i + 1) % size_idx;
    }
    bool IsKeyInTable(Key k) {
        size_t real_index = hash(k) % size_idx;
        for (size_t i = 0; i < size_idx; ++i) {
            if (equals(data[real_index].first, k)) {
                return true;
            } else if(equals(data[real_index].first, empty_value)) {
                return false;
            } else {
                real_index = clustering(real_index, i);
            }
        }
        return false;
    }

    bool insert_item(pair<Key, Value>&& item) {
        size_t real_index = hash(item.first) % size_idx;
        for (size_t i = 0; i < size_idx; ++i) {
           if (equals(data[real_index].first, del_value) || equals(data[real_index].first, empty_value)) {
                data[real_index] = move(item);
                items_count++;
                return true;
            }
            real_index = clustering(real_index, i);
        }
        return false;
    }

    void grow() {
        HashTable<Key, Value> new_table(del_value, empty_value, size_idx * 2, hash, equals);
        for(auto& item: data) {
            if(!(equals(item.first, del_value) || equals(item.first, empty_value))) {
                new_table.insert_item(move(item));
            }
        }
        *this = move(new_table);

    }
    vector<pair<Key, Value>> data;

    size_t items_count;
    size_t size_idx;

    Value del_value, empty_value;

    Hash hash;
    Equals equals;
};

int main() {
    char operation = '\0';
    std::string key = "";

    HashTable<std::string, std::string> hash("DEL", "EMPTY");
    while (std::cin >> operation >> key) {
        bool res = false;
        switch (operation) {
            case '+': {
                res = hash.insert(key, key);
                break;
            }
            case '-': {
                res = hash.erase(abs(key));
                break;
            }
            case '?': {
                res = (hash.find(key) != nullptr);
                break;
            }
            default: {
                break;
            }
        }
        if (res) {
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }

    return 0;
}
