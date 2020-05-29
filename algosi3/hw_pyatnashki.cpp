#include <iostream>
#include <array>
#include <map>
#include <queue>
#include <assert.h>
#include <vector>
#include <map>

static const int LINE_SIZE = 4;
static const int FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

static const FieldArray EMPTY_FIELD = {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
};

static const FieldArray GOAL_FIELD = {
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13,14,15,0

};


class FieldState {
public:
    FieldState() : InitMove('F') {
        init(EMPTY_FIELD);
    }

    FieldState(const FieldArray& arr) : InitMove('F') {
        init(arr);
    }

    FieldState& operator= (const FieldState& r) = default;

    bool IsChetniy() {
        int buff = (zero_pos / LINE_SIZE) + 1;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            int counter = 0;
            for (int j = 0; j < i; ++j) {
                if (field[i] != 0 && field[j] != 0) {
                    if (field[j] > field[i]) {
                        counter++;
                    }
                }
            }
            buff += counter;
        }
        return ((buff % 2) == 0);
    }
    int Manhetten() const{
        int result = 0;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            /*int x_need = (field[i] - 1) % LINE_SIZE;
            int y_need = (field[i] - 1) / LINE_SIZE;
            int x_curr = i % LINE_SIZE;
            int y_curr = i / LINE_SIZE;
            */
            //result += (abs(x_curr - x_need) + abs(y_curr - y_need));  // Xтекущее  - х нужное
            if (field[i] == 0) {
                result += (abs((i % LINE_SIZE) - 3) +
                           abs((i / LINE_SIZE) - 3));
            } else {
            result += (abs((i % LINE_SIZE) - ((field[i] - 1) % LINE_SIZE)) +
                       abs((i / LINE_SIZE)- ((field[i] - 1) / LINE_SIZE)));
            }
        }
        return result;
    }

    bool operator== (const FieldState& r) const {
        bool res = field == r.field;
        if (res) {
            assert( zero_pos == r.zero_pos);
        }
        return res;
    }

    bool operator< (const FieldState& r) const {
        return field < r.field;
    }

    FieldState(const FieldState&) = default;

    ~FieldState() = default;

    std::vector<FieldState> GetNextVertices() const {
        std::vector<FieldState> res;
        if (zero_pos >= LINE_SIZE) {
            FieldState st(*this);
            st.moveUp();
            res.push_back(st);
        }

        if (zero_pos < FIELD_SIZE - LINE_SIZE) {
            FieldState st(*this);
            st.moveDown();
            res.push_back(st);
        }

        if (zero_pos % LINE_SIZE != 0) {
            FieldState st(*this);
            st.moveLeft();
            res.push_back(st);
        }

        if ((zero_pos + 1) % LINE_SIZE != 0) {
            FieldState st(*this);
            st.moveRight();
            res.push_back(st);
        }

        return res;
    }

    void print() const {
        for (int i = 0; i < field.size(); ++i) {
            std::cout << (int)field[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }
        }
    }
    char InitMove;

private:
    void moveUp() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos -= LINE_SIZE;
        std::swap(field[zero_pos], field[old_zero_pos]);
        InitMove = 'D';
    }

    void moveDown() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos += LINE_SIZE;
        std::swap(field[zero_pos], field[old_zero_pos]);
        InitMove = 'U';
    }

    void moveLeft() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos -= 1;
        std::swap(field[zero_pos], field[old_zero_pos]);
        InitMove = 'R';
    }

    void moveRight() {
        uint8_t old_zero_pos = zero_pos;
        zero_pos += 1;
        std::swap(field[zero_pos], field[old_zero_pos]);
        InitMove = 'L';
    }

    void init(const FieldArray& arr) {
        for (int i = 0; i < arr.size(); ++i) {
            field[i] = arr[i];
            if (arr[i] == 0) {
                zero_pos = i;
            }
        }
    }
    FieldArray field;
    uint8_t zero_pos;
};

void Astar(const FieldState& start) {
    const FieldState empty_state(EMPTY_FIELD);
    const FieldState goal_state(GOAL_FIELD);

    // key - Манхетенское расстояние
    // value - first - состояние, second - расстояние
    std::multimap<int, std::pair<FieldState, int>> queue;
    // key - посещенная вершина
    // value - first - parent, second - расстояние
    std::map<FieldState, std::pair<FieldState, int>> used;
    bool found = false;
    FieldState cur_state = empty_state;
    used.insert({start, {empty_state, 0}});
    queue.insert({start.Manhetten(),{start, 0}});
    while (!queue.empty()) {
        int cur_manhetten = queue.begin()->first;
        cur_state = queue.begin()->second.first;
        int cur_dist = queue.begin()->second.second;
        queue.erase(queue.begin());
        if (cur_state == goal_state) {
            found = true;
            break;
        }
        for (FieldState child : cur_state.GetNextVertices()) {
            int pre_Manhetten = child.Manhetten();
            int child_final_Manhetten = 3 * pre_Manhetten + cur_dist + 1;
            auto used_child = used.find(child);
            int old_child_Manhetten = 3 * pre_Manhetten + used_child->second.second + 1;
            if (used_child == used.end()) {
                used.insert({child, {cur_state, cur_dist + 1}});
                queue.insert({child_final_Manhetten,{child, cur_dist + 1}}); // mb ne insert
            } else if (child_final_Manhetten < old_child_Manhetten) {
                auto range = queue.equal_range(old_child_Manhetten);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second.first == child) {
                        queue.erase(it);
                        queue.insert({child_final_Manhetten,{child, cur_dist + 1}});
                        used[child] = {cur_state, cur_dist};
                        break;
                    }
                }
            }
        }
    }

    int steps_count = -1;
    std::vector<char> answer;
    if (found) {
        while (!(cur_state == empty_state)) {
            steps_count++;
            //cur_state.print();
            if (cur_state.InitMove != 'F') {
                answer.push_back(cur_state.InitMove);
             }
            //std::cout << "--------" << std::endl;
            cur_state = used.find(cur_state)->second.first;
        }
        std::cout << steps_count << std::endl;
        for (auto it = answer.rbegin(); it != answer.rend(); ++it) {
            std::cout << *it << ' ';

        }
    } else {
        std::cout << "Not found, steps count:" << used.size() << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    FieldArray arr;
    int buff;
    for (int i = 0; i < arr.size(); ++i) {
        std::cin >> buff;
        arr[i] = buff;
    }

   /* FieldState start({
                             1,  2,  3,  4,
                             5,  6,  7,  8,
                             9,  10, 11, 0,
                             13, 14, 15, 12,
                     });*/
    FieldState start(arr);
    if (start.IsChetniy()) {
        Astar(start);
    } else {
        std::cout << -1;
    }
    return 0;
}