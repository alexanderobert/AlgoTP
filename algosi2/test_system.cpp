#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <chrono>
#include <zconf.h>

using namespace std;

int main() {
    double game_duration = 1;
    double tick_duration = 1 / 25.0;
    auto round_start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> current_game_duration(0);
    std::chrono::duration<double> current_tick_duration(0);
    auto last_tick = std::chrono::high_resolution_clock::now();
    while (current_game_duration.count() < game_duration) {
        auto curr_time = std::chrono::high_resolution_clock::now();
        current_tick_duration = curr_time - last_tick;

        if (current_tick_duration.count() > tick_duration) {
            cout << current_tick_duration.count() << ' ' << tick_duration << endl;
            cout << "----------------------------------------------------------\n";
            last_tick = curr_time;

        }
        curr_time = std::chrono::high_resolution_clock::now();
        current_game_duration = curr_time - round_start;
    }

}