#include "day5.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <sys/types.h>
#include <ranges>
#include <vector>
#include <algorithm>
#include <print>
#include <iostream>
#include <iterator>


using namespace std;
namespace adventofcode
{
Day5::Day5() : DaySolver(5) {
    // Data for tests
    test_data = R"(3-5
10-14
16-20
12-18

1
5
8
11
17
32)";
    test_results = { 3, 14 };
}

ResultType Day5::solvePart1() {
    auto input_str = (string)input_data;
    auto paragraphs = input_str
        | views::all
        | views::split((string_view)"\n\n");
    auto it = paragraphs.begin();
    auto ranges_it = *it;
    ++it;
    auto ids_it = *it;
    auto ranges = string(ranges_it.begin(), ranges_it.end()) | views::split('\n');
    auto ids = string(ids_it.begin(), ids_it.end()) | views::split('\n');
    vector<array<uint64_t, 2>> ranges_vec = {};
    vector<uint64_t> ids_vec = {};
    auto counter = 0;
    for (auto line : ranges) {
        auto str = string(line.begin(), line.end());
        int pos = (int)str.find('-');
        auto start = stoull(string(str.substr(0, pos)));
        auto finish = stoull(string(str.substr(pos + 1)));
        // println("start = {}", start);
        ranges_vec.push_back({start, finish});
    };
    for (auto line: ids) {
        auto id = stoull(string(line.begin(), line.end()));
        for (auto [start, finish]: ranges_vec) {
            // println("{}, {}, {}", id, start, finish);
            if (id >= start && id <= finish) {
                counter += 1;
                break;
            }
        }
    }



    return counter;
}

ResultType Day5::solvePart2() {
    auto input_str = (string)input_data;
    auto paragraphs = input_str
        | views::all
        | views::split((string_view)"\n\n");
    auto it = paragraphs.begin();
    auto ranges_it = *it;
    // ++it;
    // auto ids_it = *it;
    auto ranges = string(ranges_it.begin(), ranges_it.end()) | views::split('\n');
    vector<array<int64_t, 2>> ranges_vec = {};
    // vector<uint64_t> ids_vec = {};
    int64_t counter = 0;
    for (auto line : ranges) {
        auto str = string(line.begin(), line.end());
        int pos = (int)str.find('-');
        auto start = stoll(string(str.substr(0, pos)));
        auto finish = stoll(string(str.substr(pos + 1)));
        ranges_vec.push_back({start, finish});
    };
    sort(ranges_vec.begin(), ranges_vec.end());

    // auto last_upper = ranges_vec[0][1];
    vector<array<int64_t, 2>> comb_ranges_vec = {};
    for (const auto& arr: ranges_vec) {
        if (comb_ranges_vec.size() == 0 || arr[0] > comb_ranges_vec.back()[1]) {
            comb_ranges_vec.push_back(arr);
        } else if (arr[1] > comb_ranges_vec.back()[1]) {
            comb_ranges_vec.back()[1] = arr[1];
        }
    }
    for (const auto& arr: comb_ranges_vec) {
        counter += arr[1] - arr[0] + 1;
        // cout << arr[0] << " " << arr[1] << ", ";
    }
    // for (auto [start, finish]: ranges_vec) {
    //     counter += finish - start + 1;
    // }



    return counter;
}

}
