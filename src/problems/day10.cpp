#include "day10.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <set>
#include <string>
#include <cstdlib>
#include <ranges>
#include <print>
#include <sys/types.h>
#include <ranges>
#include <map>
#include <vector>

using namespace std;
namespace adventofcode
{
Day10::Day10() : DaySolver(10) {
    // Data for tests
    test_data = R"([.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5})";
    test_results = { 7, 33 };
}

ResultType Day10::solvePart1() {
    vector<uint64_t> indicators = {};
    vector<vector<uint64_t>> buttons = {};
    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(' ');
            auto indicator = string(str.substr(0, pos));
            auto indicator_num = (size_t)0;
            for (auto i = (size_t)(1); i < indicator.size() - 1; i++) {
                auto symb = indicator[i];
                if (symb == '#') {
                    indicator_num |= 1 << (i - 1);
                }
            }
            indicators.push_back(indicator_num);
            auto rest_str = string(str.substr(pos + 1));
            auto rest = rest_str | views::split(' ');
            vector<uint64_t> res_v = rest
                    | views::transform([](auto&& rng) {
                        // auto cur_str = string(rng.begin(), rng.end());
                        auto len = rng.size();
                        auto bracketless = rng
                        | views::drop(1)
                        | views::take(len - 2);

                        auto button_num = (size_t)0;
                        ranges::for_each(bracketless | views::split(','),
                           [&](auto but_id) {
                               auto but_id_str = string(but_id.begin(), but_id.end());
                               button_num |= 1 << (stoll(but_id_str));
                        });
                        return button_num;
                    }) | ranges::to<vector>();
            res_v.pop_back(); // dont need joules
            buttons.push_back(res_v);
        });
    uint64_t res = (uint64_t)0;
    for (auto [i, indicator]: indicators | views::enumerate) {
        uint64_t minimal_presses = (uint64_t)0;
        auto cur_buttons = buttons[i];
        pmr::deque<array<uint64_t, 3>> q = {}; // counter, number, prev_modifier
        array<uint64_t, 3> first = {0, 0, 0};
        q.push_back(first);
        while (minimal_presses == 0) {
            auto [cur_counter, cur_number, cur_pmodifier] = q.front();
            if (i == 0) {
                // println("indicator = {}, cur_counter = {}, cur_number = {}, prev_mod = {}", indicator, cur_counter, cur_number, cur_pmodifier);
            }
            q.pop_front();
            for (auto modifier: (cur_buttons
                | views::filter(
                    [&](auto but) { return but != cur_pmodifier; }))) {
                auto new_number = cur_number ^ modifier;
                auto new_counter = cur_counter + 1;
                if (new_number == indicator) {
                    minimal_presses = new_counter;
                    println("minimal presses = {}", minimal_presses);
                    break;
                }
                array<uint64_t, 3> new_array = {new_counter, new_number, modifier};
                q.push_back(new_array);
            }
        }
        res += minimal_presses;
    }
    // for (auto but: buttons[0]) {
    //     print("{},", but);
    // }
    return res;
}

ResultType Day10::solvePart2() {
    vector<vector<uint16_t>> joules_set = {};
    vector<vector<uint64_t>> buttons = {};
    ranges::for_each(input_data | views::split('\n'),
        [&](auto line) {
            auto str = string(line.begin(), line.end());
            size_t pos = (size_t)str.find(' ');

            auto rest_str = string(str.substr(pos + 1));
            auto rest = rest_str | views::split(' ');
            vector<vector<uint64_t>> res_strings = rest
                | views::transform([](auto&& rng) {
                    auto len = rng.size();
                    auto bracketless = rng
                    | views::drop(1)
                    | views::take(len - 2);

                    auto button_num = (size_t)0;
                    return bracketless
                        | views::split(',')
                        | views::transform(
                            [&](auto but_id) {
                                auto but_id_str = string(but_id.begin(), but_id.end());
                                return static_cast<uint64_t>(stoll(but_id_str));
                        }) | ranges::to<vector>();
                }) | ranges::to<vector>();
            auto req_joules64 = res_strings.back();
            vector<uint16_t> req_joules = req_joules64
                | views::transform([](uint64_t val) { return static_cast<uint16_t>(val); })
                | ranges::to<vector>();
            joules_set.push_back(req_joules); // need joules
            res_strings.pop_back(); // dont need joules here
            vector<uint64_t> res_v = res_strings
                | views::transform([&](auto but_ids) {
                    uint64_t button_num = (uint64_t)0;
                    ranges::for_each(but_ids,
                        [&](auto but_id) {
                            button_num |= 1LL << (but_id);
                        });
                    return button_num;
                }) | ranges::to<vector>();
            buttons.push_back(res_v);
        });
    uint64_t res = (uint64_t)0;
    for (auto [i, joules]: joules_set | views::enumerate) {
        println("=============== {} ROW ================", i + 1);
        pmr::set<vector<uint16_t>> seen_traffic = {};
        size_t minimal_presses = (size_t)0;
        auto cur_buttons = buttons[i];
        auto joules_size = joules.size();
        pmr::deque<tuple<uint64_t, vector<uint16_t>>> q = {}; // counter, traffic
        vector<uint16_t> starting_traffic(joules_size);
        tuple<uint64_t, vector<uint16_t>> first = {0, starting_traffic};
        seen_traffic.insert(starting_traffic);
        q.push_back(first);
        while (minimal_presses == 0) {
            auto [cur_counter, cur_traffic] = q.front();
            if (i == 0) {
                print("traffic: ");
                ranges::for_each(cur_traffic, [](auto traffic_num) {
                    print("{},", traffic_num);
                });
                println(" cur_counter = {}", cur_counter);
            }
            q.pop_front();
            auto new_counter = cur_counter + 1;
            if (new_counter % 10 == 0) {
                println("counter is = {}", new_counter);
            }
            for (auto modifier: cur_buttons) {
                // auto new_number = cur_number ^ modifier;
                auto new_traffic = cur_traffic;
                int i = 0;
                auto is_skipped = false;
                while (modifier > 0) {
                    if (modifier & 1) {
                        new_traffic[i]++;
                        if (new_traffic[i] > joules[i]) {
                            is_skipped = true;
                            break;
                        }
                    }
                    modifier >>= 1;
                    i++;
                }
                if (seen_traffic.contains(new_traffic)) {
                    is_skipped = true;
                }
                if (is_skipped) {
                    continue;
                }
                seen_traffic.insert(new_traffic);
                if (new_traffic == joules) {
                    minimal_presses = new_counter;
                    println("minimal presses = {}", minimal_presses);
                    break;
                }
                tuple<uint64_t, vector<uint16_t>> new_array = {new_counter, new_traffic};
                q.push_back(new_array);
            }
        }
        res += minimal_presses;
    }
    // for (auto but: buttons[0]) {
    //     print("{},", but);
    // }
    return res;
}

}
