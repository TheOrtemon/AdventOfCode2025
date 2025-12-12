#include "day12.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <string>
#include <string_view>
#include <cstdlib>
#include <ranges>
#include <sys/types.h>
#include <ranges>
#include <vector>
#include <print>


using namespace std;
namespace adventofcode
{
Day12::Day12() : DaySolver(12) {
    // Data for tests
    test_data = R"(0:
###
##.
##.

1:
###
##.
.##

2:
.##
###
##.

3:
##.
###
##.

4:
###
#..
###

5:
###
.#.
###

4x4: 0 0 0 0 2 0
12x5: 1 0 1 0 2 2
12x5: 1 0 1 0 3 2)";
    test_results = { 2, 0 };
}

ResultType Day12::solvePart1() {
    string input_string = string(input_data.begin(), input_data.end());
    vector<string> blocks = input_data
    | views::split("\n\n"sv)
    | views::transform([](auto block) {
        return string(block.begin(), block.end());
    }) | ranges::to<vector>();
    vector<size_t> block_areas = blocks
        | views::transform([](auto block) {
            return static_cast<size_t>(count(block.begin(), block.end(), '#'));
        })
        | ranges::to<vector>();

    string regions_block = blocks.back();
    auto res_counter = 0LL;
    ranges::for_each(regions_block | views::split('\n'),
        [&](auto line) {
            auto line_str = string(line.begin(), line.end());
            size_t x_pos = line_str.find('x');
            auto len1_str = string(line_str.substr(0, x_pos));
            auto len1 = stoll(len1_str);
            size_t colon_pos = line_str.find(':');
            auto len2_str = string(line_str.substr(x_pos + 1, colon_pos - x_pos - 1));
            auto len2 = stoll(len2_str);
            auto shapes_line = string(line_str.substr(colon_pos + 2));
            vector<size_t> shapes = shapes_line
                | views::split(' ')
                | views::transform([&](auto num_str) {
                    return (size_t)stoll(string(num_str.begin(), num_str.end()));
                }) | ranges::to<vector>();
            auto area = len1 * len2;
            auto enumerated_shapes = shapes | views::enumerate;
            auto pseudo_area = accumulate(
                enumerated_shapes.begin(),
                enumerated_shapes.end(),
                0,
                [&](auto acc, auto pair ) {
                    auto [i, cv] = pair;
                    return acc + static_cast<decltype(acc)>(cv * block_areas[i]);
                }
            );
            if (pseudo_area < area) {
                res_counter += 1;
            }
        }
    );
    return res_counter;
}

ResultType Day12::solvePart2() {
    return 0;
}

}
